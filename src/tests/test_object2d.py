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
from typing import List
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


# s.plt_plot()


def plt_path(object: 'Object2d', derivs: List[float] = []) -> 'Path':
    """
    Converts an object (multiple polygons with holes) to a
    matplotlib path object that can be displayed as a patch.
    """
    from matplotlib.path import Path

    points = []
    codes = []

    def value(x):
        v = x.value()
        for (a, b) in zip(derivs, x.derivs()):
            v += a * b
        return v

    def coord(pt):
        return (value(pt[0]), value(pt[1]))

    def addit(pts):
        assert len(pts) >= 3
        points.append(coord(pts[0]))
        codes.append(Path.MOVETO)
        for i in range(1, len(pts)):
            points.append(coord(pts[i]))
            codes.append(Path.LINETO)
        points.append((0.0, 0.0))
        codes.append(Path.CLOSEPOLY)

    for i in range(object.num_components()):
        obj = object.get_component(i)
        assert obj.num_components() == 1
        addit(obj.get_polygon(0).get_points())
        for i in range(1, obj.num_polygons()):
            addit(obj.get_polygon(i).get_points()[::-1])

    return Path(points, codes)


_, ax = plt.subplots()
xmin, ymin, xmax, ymax = s.bbox()
ax.set_xlim(1.1 * xmin - 0.1 * xmax, 1.1 * xmax - 0.1 * xmin)
ax.set_ylim(1.1 * ymin - 0.1 * ymax, 1.1 * ymax - 0.1 * ymin)
for i in range(-10, 0):
    ax.add_patch(PathPatch(plt_path(s, [0, 0, 0, 0.01 * i]), facecolor='none'))
plt.show()
