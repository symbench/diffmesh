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

def plt_path(object: 'Object2d') -> 'Path':
    """
    Converts an object (multiple polygons with holes) to a
    matplotlib path object that can be displayed as a patch.
    """
    from matplotlib.path import Path

    points = []
    codes = []

    def addit(pts):
        assert len(pts) >= 3
        points.extend(pts)
        codes.append(Path.MOVETO)
        for i in range(len(pts) - 1):
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
