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


def test1():
    diameter = DiffReal(5, [1, 0])
    length = DiffReal(3, [0, 1])

    object = Object2d.rectangle(length, diameter)
    circle = Object2d.circle(diameter * DiffReal(0.5))
    object = object.join(circle.translate(length * DiffReal(0.5), DiffReal(0)))
    object = object.join(circle.translate(
        length * DiffReal(-0.5), DiffReal(0)))
    object = Object2d.rectangle(DiffReal(15), DiffReal(10)).difference(object)

    object = object.simplify()
    object.plt_plot([1, 0])

    mesh = Mesh2d(object)
    mesh.refine_delaunay(size_bound=1.0)
    mesh.plt_plot([1, 0])


def test2():
    width = DiffReal(10, [1, 0, 0, 0])
    height = DiffReal(10, [0, 1, 0, 0])
    scale = DiffReal(0.5, [0, 0, 1, 0])
    angle = DiffReal(0.5, [0, 0, 0, 1])

    r = Object2d.rectangle(width, height)
    s = r.difference(r.scale(scale))
    s = s.join(s.rotate(angle))
    # s.plt_plot()

    m = Mesh2d(s)
    m.refine_delaunay(size_bound=1.5)
    # print(m.vertices())
    # print(m.faces())
    m.plt_plot([1.0, 0, 0, 0])


test1()
