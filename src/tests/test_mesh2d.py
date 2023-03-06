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

import matplotlib.pyplot as plt
from diffmesh import Object2d, DiffReal, Mesh2d

width = DiffReal(10, [1, 0])
height = DiffReal(10, [0, 1])

r = Object2d.rectangle(width, height)
r = r.join(r.rotate(0.2).translate(3, 0))
r = r.difference(r.scale(0.5))

m = Mesh2d(r)
m.plt_plot()
