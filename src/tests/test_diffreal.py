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

from diffmesh import DiffReal

a = DiffReal(2.0, [1.0, 0.0])
print(a.value(), a.derivs())

b = DiffReal(3.0, [1.0, -4.0])
print(b.value(), b.derivs())

c = a + b
print(c.value(), c.derivs())

d = a - b
print(d.value(), d.derivs())

e = a * b
print(e.value(), e.derivs())

f = a / b
print(f.value(), f.derivs())

g = DiffReal(1.0) / b
print(g.value(), g.derivs())

h = a * g
print(h.value(), h.derivs())
