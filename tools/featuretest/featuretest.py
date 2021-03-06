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
import os
import subprocess

def get_boards(app):
    res = subprocess.check_output(["make", "-C", app, "info-boards-supported"])
    return res.decode('ASCII').split("\n")[1].split()

def main(args):
    app = os.path.join(os.path.abspath(args.riotbase), args.app)
    env = os.environ.copy()
    count = 0

    for board in get_boards(app):
        env['BOARD'] = board;
        res = subprocess.check_output(("make", "-C", app, "info-cpu"), env=env)
        line = res.decode('ASCII').split('\n')

        a = sorted(line[0].split())
        b = sorted(line[1].split())

        if len(a) != len(set(a)):
            print('{}: duplicates found ({})'.format(board, len(a) - len(set(a))))
            print(a)
            count += 1

    print("found {} diffs".format(count))



    # tests = os.path.join(os.path.abspath(args.riotbase), "tests")
    # for file in sorted(os.listdir(tests)):
    #     test = os.path.join(tests, file)
    #     if os.path.isdir(test):
    #         out = subprocess.check_output(("make", "-C", test, "info-boards-supported"))
    #         print(out.decode('ASCII'))

if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("app", default="examples/hello-world", nargs="?", help="app")
    p.add_argument("riotbase", default="../../../RIOT", nargs="?", help="RIOT base directory")
    args = p.parse_args()
    main(args)
