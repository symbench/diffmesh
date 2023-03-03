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

from diffmesh import Object2d

import matplotlib.pyplot as plt
from matplotlib.patches import PathPatch


r = Object2d.rectangle(10, 10)
c = Object2d.circle(5)

s = Object2d()
s = s.join(r.translate(6, 0))
s = s.join(c.translate(0, 6))
s = s.join(c.translate(-7, 0))
s = s.join(r.translate(0, -7))
s = s.difference(c.translate(12, -5))
s = s.join(c.translate(13, -6))
s = s.intersection(r.scale(2.0))

path = s.plt_path()

_, ax = plt.subplots()
ax.set_xlim(-30, 30)
ax.set_ylim(-30, 30)
ax.add_patch(PathPatch(path))
plt.show()
