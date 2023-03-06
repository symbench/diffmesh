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

from typing import List


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


def plt_plot(mesh: 'Mesh2d'):
    """
    Plots the given mesh using matplotlib.
    """
    import matplotlib.pyplot as plt

    triangles = plt_triangulation(mesh)

    _, ax = plt.subplots()
    ax.set_aspect('equal')
    ax.tripcolor(triangles,
                 facecolors=[0.3] * len(triangles.triangles),
                 vmin=0.0, vmax=1.0, alpha=0.2)
    ax.triplot(triangles, 'bo-', lw=1.0)
    plt.show()
