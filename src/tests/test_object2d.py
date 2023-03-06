#!/usr/bin/env python3
# Copyright (C) 2023, Miklos Maroti
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

from diffmesh import Object2d, DiffReal

rwidth = DiffReal(10, [1, 0, 0, 0])
rheight = DiffReal(10, [0, 1, 0, 0])
cradius = DiffReal(5, [0, 0, 1, 0])
angle = DiffReal(0.1, [0, 0, 0, 1])

r = Object2d.rectangle(rwidth, rheight)
c = Object2d.circle(cradius)

s = Object2d()
s = s.join(r.translate(DiffReal(6), DiffReal(0)))
s = s.join(c.translate(DiffReal(0), DiffReal(6)))
s = s.join(c.translate(DiffReal(-7), DiffReal(0)))
s = s.join(r.translate(DiffReal(0), DiffReal(-7)))
s = s.difference(c.translate(DiffReal(12), DiffReal(-5)))
s = s.join(c.translate(DiffReal(13), DiffReal(-6)))
s = s.intersection(r.scale(DiffReal(2.0)).rotate(angle))

s.plt_plot()
