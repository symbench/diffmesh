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

from ._diffmesh import (
    CGAL_VERSION_STR,
    DiffReal,
    Object2d,
    Mesh2d,
)

from . import object2d_ext
from . import mesh2d_ext

Object2d.plt_path = object2d_ext.plt_path
Object2d.plt_arrows = object2d_ext.plt_arrows
Object2d.plt_plot = object2d_ext.plt_plot

Mesh2d.plt_triangulation = mesh2d_ext.plt_triangulation
Mesh2d.plt_arrows = mesh2d_ext.plt_arrows
Mesh2d.plt_plot = mesh2d_ext.plt_plot

__all__ = [
    "CGAL_VERSION_STR",
    "DiffReal",
    "Object2d",
    "Mesh2d",
]
