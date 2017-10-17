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

covered = set()
skip = set()
nobuild = set()

def run_test(test, name):
    if subprocess.call(["make", "-C", test, "all"],
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL) != 0:
        print("[BUILD FAILED]", name)
        nobuild.add(name)
        return

    try:
        test = subprocess.check_output(["make", "-C", test, "test"], stderr=subprocess.DEVNULL)
        # print(test)
        print("make test:", test)
    except:
        print("no make test")


def check_test(test):
    name = os.path.basename(test)
    covered.add(name)
    foo = subprocess.check_output(["make", "-C", test, "info-boards-supported"])
    if "native" in str(foo).split():
        print("[ RUN]", name)
        run_test(test, name)
    else:
        print("[SKIP]", name)
        skip.add(name)

def main(args):
    tests = os.path.join(os.path.abspath(args.riotbase), "tests")
    for file in sorted(os.listdir(tests)):
        test = os.path.join(tests, file)
        if os.path.isdir(test):
            check_test(test)

    for name in covered:
        if name in skip:
            print(name, "skipped")
        if name in nobuild:
            print(name, "build failed")


if __name__ == "__main__":
    p = argparse.ArgumentParser()
    # p.add_argument("board", default="native", nargs="?", help="RIOT base directory")
    p.add_argument("riotbase", default="../../../RIOT", nargs="?", help="RIOT base directory")
    args = p.parse_args()
    main(args)
