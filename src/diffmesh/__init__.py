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
)

from .object2d_ext import plt_path, plt_arrows, plt_plot
Object2d.plt_path = plt_path
Object2d.plt_arrows = plt_arrows
Object2d.plt_plot = plt_plot

__all__ = [
    "CGAL_VERSION_STR",
    "DiffReal",
    "Object2d",
]
