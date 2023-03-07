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


def plt_triangulation(mesh: 'Mesh2d', derivs: List[float] = []) -> 'Triangulation':
    """
    Converts a mesh to a matplotlib triangulation.
    """
    from matplotlib.tri import Triangulation

    xs = []
    ys = []

    def value(x):
        v = x.value()
        for (a, b) in zip(derivs, x.derivs()):
            v += a * b
        return v

    for v in mesh.vertices():
        xs.append(value(v[0]))
        ys.append(value(v[1]))

    return Triangulation(xs, ys, mesh.faces())


def plt_arrows(mesh: 'Mesh2d', derivs: List[float] = []) \
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

    def arrow(v):
        xs.append(v[0].value())
        ys.append(v[1].value())
        us.append(delta(v[0]))
        vs.append(delta(v[1]))

    for v in mesh.vertices():
        arrow(v)

    return xs, ys, us, vs


def plt_plot(mesh: 'Mesh2d', derivs: None | List[float] = None):
    """
    Plots the given mesh using matplotlib. If derivs are specified
    the the partial derivatives are plotted as arrows.
    """
    import matplotlib.pyplot as plt

    triangles = plt_triangulation(mesh)

    _, ax = plt.subplots()
    ax.set_aspect('equal')
    ax.tripcolor(triangles,
                 facecolors=[0.3] * len(triangles.triangles),
                 vmin=0.0, vmax=1.0, alpha=0.2)
    ax.triplot(triangles, 'b.-', lw=1.0)
    if derivs is not None:
        xs, ys, us, vs = plt_arrows(mesh, derivs)
        ax.quiver(xs, ys, us, vs, angles='xy', scale=1.0,
                  units='xy')
    plt.show()
