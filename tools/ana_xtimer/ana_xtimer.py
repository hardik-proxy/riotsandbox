#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (C) 2017  Hauke Petersen <dev@haukepetersen.de>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import sys
from os import path
import argparse
import subprocess
import re

defines = {}
usage = {}
base = ""

def parse_exp(file, exp):
    print("parse", file, exp)


def find_define_val(file, define):
    # did we find it before?
    if define in defines:
        return defines[define]

    # if plain define
    local = subprocess.check_output(('git', 'grep', define, file), cwd=base).split('\n')
    for finding in local:
        source = re.search("#define +" + define + " +\(?(\d+.+)\)", finding)
        if source:
            print(source.group(1))

            sum = 1

            nums = re.finditer("(\d+)[uUlL]*|([_A-Z0-9]+)", source.group(1))
            for num in nums:
                if num.group(1):
                    sum *= int(num.group(1))
                elif num.group(2):
                    aaa = find_define_val(file, num.group(2))
                    if aaa:
                        print("AAA", aaa)
                        sum *= 1 * aaa


                print("found", num.group(0), num.group(1), num.group(2))


            print("SUM is", sum)
            return sum




    # search globally
    glob = subprocess.check_output(('git', 'grep', define), cwd=base).split('\n')
    # for finding in glob:

    return 0

def enlist(define, val):
    if define in defines:
        if defines[define] != val:
            sys.exit("ERROR: define with contradicting value:", define, val)
    else:
        defines[define] = val

def report(to):
    if to:
        if to in usage:
            usage[to] += 1
        else:
            usage[to] = 1


if __name__ == "__main__":
    # Define some command line args
    p = argparse.ArgumentParser()
    p.add_argument("appdir", default="../../RIOT", nargs="?", help="Full path to RIOT dir")
    args = p.parse_args()

    base = path.normpath(args.appdir)
    if not path.isdir(base):
        sys.exit("Error: RIOT folder '" + app + "' not found")

    xtimer = subprocess.check_output(('git', 'grep', 'xtimer_usleep'), cwd=base).split('\n')

    for finding in xtimer:
        m = re.search("xtimer_usleep\((.+)\)", finding)

        if m:
            file = re.search("^([a-z].+\.[ch])", finding).group(1)
            if re.search("^\d+$", m.group(1)):
                report(int(m.group(1)))
            elif re.search("^[A-Z][_A-Z0-9]+$", m.group(1)):
                report(find_define_val(file, m.group(1)))
            # else:
                # print("var found:", finding)


    print("DEFINES:")
    print(defines)

    print("USAGE STATS:")
    print("Timout\t\tTimes Used")
    for key in sorted(usage):
        print("%d\t\t%d" % (key, usage[key]))

