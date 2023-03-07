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

from diffmesh import Object2d, DiffReal, Mesh2d

width = DiffReal(10, [1, 0])
height = DiffReal(10, [0, 1])

r = Object2d.rectangle(width, height)
s = r.join(r.rotate(0.4).translate(2, 0))
s = s.difference(Object2d.circle(DiffReal(3.0), 30))
s.plt_plot()

m = Mesh2d(s)
print("refining")
m.refine_delaunay()
print("plotting")
# m.lloyd_optimize()
m.plt_plot()
