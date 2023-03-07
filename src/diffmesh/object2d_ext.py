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

from typing import List, Tuple


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
        addit(obj.get_polygon(0).get_vertices())
        for i in range(1, obj.num_polygons()):
            addit(obj.get_polygon(i).get_vertices()[::-1])

    return Path(points, codes)


def plt_arrows(object: 'Object2d', derivs: List[float] = []) \
        -> Tuple[List[float], List[float], List[float], List[float]]:
    xs = []
    ys = []
    us = []
    vs = []

    def delta(x):
        d = 0.0
        for (a, b) in zip(derivs, x.derivs()):
            d += a * b
        return d

    def arrow(pt):
        xs.append(pt[0].value())
        ys.append(pt[1].value())
        us.append(delta(pt[0]))
        vs.append(delta(pt[1]))

    def addit(pts):
        assert len(pts) >= 3
        for pt in pts:
            arrow(pt)

    for i in range(object.num_components()):
        obj = object.get_component(i)
        assert obj.num_components() == 1
        addit(obj.get_polygon(0).get_vertices())
        for i in range(1, obj.num_polygons()):
            addit(obj.get_polygon(i).get_vertices()[::-1])

    return xs, ys, us, vs


def plt_plot(object: 'Object2d', derivs: None | List[float] = None):
    """
    Plots the given object using matplotlib. If derivs are specified
    the the partial derivatives are plotted as arrows.
    """
    import matplotlib.pyplot as plt
    from matplotlib.patches import PathPatch

    path = plt_path(object)
    xmin, ymin, xmax, ymax = object.bbox()

    _, ax = plt.subplots()
    ax.set_aspect('equal')
    ax.set_xlim(1.1 * xmin - 0.1 * xmax, 1.1 * xmax - 0.1 * xmin)
    ax.set_ylim(1.1 * ymin - 0.1 * ymax, 1.1 * ymax - 0.1 * ymin)
    ax.add_patch(PathPatch(path))
    if derivs is not None:
        xs, ys, us, vs = plt_arrows(object, derivs)
        ax.quiver(xs, ys, us, vs, angles='xy', scale=1.0,
                  units='xy')
    plt.show()
