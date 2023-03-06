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
from matplotlib.patches import PathPatch
from diffmesh import Object2d, DiffReal

rwidth = DiffReal(10, [1, 0, 0, 0])
rheight = DiffReal(10, [0, 1, 0, 0])
cradius = DiffReal(5, [0, 0, 1, 0])
angle = DiffReal(0.1, [0, 0, 0, 1])

r = Object2d.rectangle(rwidth, rheight)
c = Object2d.circle(cradius)

s = Object2d()
s = s.join(r.translate(6, 0))
s = s.join(c.translate(0, 6))
s = s.join(c.translate(-7, 0))
s = s.join(r.translate(0, -7))
s = s.difference(c.translate(12, -5))
s = s.join(c.translate(13, -6))
s = s.intersection(r.scale(2.0).rotate(angle))

_, ax = plt.subplots()
xmin, ymin, xmax, ymax = s.bbox()
ax.set_xlim(1.1 * xmin - 0.1 * xmax, 1.1 * xmax - 0.1 * xmin)
ax.set_ylim(1.1 * ymin - 0.1 * ymax, 1.1 * ymax - 0.1 * ymin)
ax.add_patch(PathPatch(s.plt_path()))
xs, ys, us, vs = s.plt_arrows([0.0, 0.0, 1.0, 0.0])
ax.quiver(xs, ys, us, vs, angles='xy', scale=1.0,
          units='xy', headwidth=2.0)
plt.show()
