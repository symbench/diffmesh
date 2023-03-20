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

import matplotlib.pyplot as plt
from matplotlib.tri import Triangulation
import numpy

diameter = DiffReal(5, [1, 0])
length = DiffReal(3, [0, 1])
derivs = numpy.array([2, -2])

object = Object2d.rectangle(length, diameter)
circle = Object2d.circle(diameter * DiffReal(0.5), segments=24)
object = object.join(circle.translate(length * DiffReal(0.5), DiffReal(0)))
object = object.join(circle.translate(
    length * DiffReal(-0.5), DiffReal(0)))
object = object.join(circle.scale(0.5).translate(7, 0))
object = Object2d.rectangle(DiffReal(15), DiffReal(10)).difference(object)
object = object.simplify()
object.plt_plot(derivs)

mesh = Mesh2d(object)
mesh.refine_delaunay(size_bound=2.0)
# mesh.plt_plot(derivs)

num_vertices = mesh.num_vertices()
matrix = numpy.zeros((num_vertices, num_vertices))
for (v0, v1, v2) in mesh.faces():
    matrix[v0, v1] = 1
    matrix[v0, v2] = 1
    matrix[v1, v0] = 1
    matrix[v1, v2] = 1
    matrix[v2, v0] = 1
    matrix[v2, v1] = 1

for v, p in enumerate(mesh.vertices()):
    if object.contains(p) == 0:
        matrix[v, :] = 0
    matrix[v, v] = 1

matrix /= numpy.sum(matrix, axis=1, keepdims=True)


points = numpy.zeros((num_vertices, 2, 1 + len(derivs)), dtype=float)
for v, p in enumerate(mesh.vertices()):
    for i in [0, 1]:
        points[v, i, 0] = p[i].value()
        points[v, i, 1:] = p[i].derivs(len(derivs))

# diffuse everything
for _ in range(100):
    points = numpy.einsum("ij,jkl->ikl", matrix, points)


triangles = Triangulation(points[:, 0, 0], points[:, 1, 0], mesh.faces())

_, ax = plt.subplots()
ax.set_aspect('equal')
ax.tripcolor(triangles,
             facecolors=[0.3] * len(triangles.triangles),
             vmin=0.0, vmax=1.0, alpha=0.2)
ax.triplot(triangles, 'b.-', lw=1.0)
ax.quiver(points[:, 0, 0], points[:, 1, 0],
          numpy.einsum("j,ij->i", derivs, points[:, 0, 1:]),
          numpy.einsum("j,ij->i", derivs, points[:, 1, 1:]),
          angles='xy', scale=1.0, units='xy')
plt.show()

assert object.num_polygons() == 2
object_outer = object.get_polygon(0)
object_inner = object.get_polygon(1)
boundary = numpy.zeros((num_vertices, ), dtype=int)
for v, p in enumerate(mesh.vertices()):
    if object_inner.contains(p) == 0:
        assert boundary[v] in [0, 1]
        boundary[v] = 1
    if object_outer.contains(p) == 0:
        assert boundary[v] in [0, 2]
        boundary[v] = 2

faces = numpy.array(mesh.faces(), dtype=int)

numpy.savez("diffuse.npz", points=points,
            faces=mesh.faces(), boundary=boundary)
