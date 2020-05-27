#!/usr/bin/env python2
# -*- coding: utf-8 -*-

# Copyright (C) 2015  Hauke Petersen <dev@haukepetersen.de>
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

import argparse
import md5

if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("eps", nargs='+', help="URI paths to encode")
    args = p.parse_args()

    for ep in args.eps:
        m = md5.new(ep.replace('/', ''))
        print('Hash: 0x%s - ' % m.hexdigest()[-8:]),
        print('Endpoint: %s' % ep)
