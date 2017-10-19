#!/usr/bin/python3
# -*- coding: utf-8 -*-

# Copyright (C) 2017 Freie Universität Berlin
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301 USA
#
# Author: Hauke Petersen <hauke.petersen@fu-berlin.de>

import argparse
import sys
import os
import re
import subprocess

skip = list()
nobuild = list()
notest = list()
failed = list()
ok = list()


def pc(str):
    sys.stdout.write(str)
    sys.stdout.flush()

def run_test(env, test, name):
    try:
        subprocess.check_output(["make", "-C", test, "-n", "test"],
                                env=env,
                                stderr=subprocess.STDOUT)
    except:
        notest.append(name)
        print(" [NO TARGET 'test']")
        return;

    pc(" building ")
    if subprocess.call(["make", "-C", test, "all"],
                       env=env,
                       stdout=subprocess.DEVNULL,
                       stderr=subprocess.DEVNULL) != 0:
        nobuild.append(name)
        print("[FAILED]")
        return
    else:
        pc("[OK]")

    pc(" flashing ")
    if subprocess.call(["make", "-C", test, "flash"],
                       env=env,
                       stdout=subprocess.DEVNULL,
                       stderr=subprocess.DEVNULL) != 0:
        print("[FAILED]")
        return
    else:
        pc("[OK]")

    pc(" testing ")
    try:
        out = subprocess.check_output(["make", "-C", test, "test"],
                                      env=env,
                                      stderr=subprocess.STDOUT)
        ok.append(name)
        print("[OK]")
    except subprocess.CalledProcessError as err:
        if re.match(".+No rule to make target `test'\..+", str(err.output)):
            notest.append(name)
            print("[NO TARGET 'test']")
        else:
            failed.append(name)
            print("[FAILED]")


def check_test(test, board):
    env = os.environ.copy()
    env['BOARD'] = board
    name = os.path.basename(test)

    print('{:<30}'.format(name), end='')
    foo = subprocess.check_output(["make", "-C", test, "info-boards-supported"],
                                  env=env)
    if board in str(foo).split():
        run_test(env, test, name)
    else:
        print(" [SKIP]")
        skip.append(name)


def main(args):
    print("Running tests for '{}'".format(args.board))
    tests = os.path.join(os.path.abspath(args.riotbase), "tests")
    for file in sorted(os.listdir(tests)):
        test = os.path.join(tests, file)
        if os.path.isdir(test):
            check_test(test, args.board)

    print("[OK]")
    for name in ok:
        print("-", name)
    print("\n[TEST FAILED]")
    for name in failed:
        print("-", name)
    print("\n[BUILD FAILED]")
    for name in nobuild:
        print("-", name)
    print("\n[NO 'test' TARGET]")
    for name in notest:
        print("-", name)
    for name in skip:
        print("-", name)


if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("board", default="native", nargs="?",help="Target board")
    p.add_argument("riotbase", default="../../../RIOT", nargs="?", help="RIOT base directory")
    args = p.parse_args()
    main(args)
