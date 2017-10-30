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

def native_set_up(riotbase):
    # check if there is already a TAP bridge (THIS IS NOT PORTABLE TO BSD/OSX)
    if subprocess.call(["ifconfig", "tapbr0"], stdout=subprocess.DEVNULL) != 0:
        try:
            print("Need sudo to create TAPs")
            subprocess.check_output(
                os.path.join(riotbase, "dist", "tools", "tapsetup", "tapsetup"))
        except:
            raise SystemError("Can not set up TAP bridge for native test")

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


def dumpfortest(test, file):
    out = subprocess.check_output(("make", "-C", test, "info-boards-supported"))
    print(out.decode('ASCII'))

def main(args):
    tests = os.path.join(os.path.abspath(args.riotbase), "tests")
    for file in sorted(os.listdir(tests)):
        test = os.path.join(tests, file)
        if os.path.isdir(test):
            dumpfortest(test, file)





if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("riotbase", default="../../../RIOT", nargs="?", help="RIOT base directory")
    args = p.parse_args()
    main(args)
