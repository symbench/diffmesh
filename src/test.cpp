/*
 * Copyright 2023 Miklos Maroti.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include <CGAL/Exact_integer.h>
#include <CGAL/Homogeneous.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <cassert>

typedef CGAL::Homogeneous<CGAL::Exact_integer> Kernel;
typedef CGAL::Nef_polyhedron_3<Kernel> Nef_polyhedron;

int main()
{
        Nef_polyhedron N0(Nef_polyhedron::EMPTY);
        Nef_polyhedron N1(Nef_polyhedron::COMPLETE);
        assert(N0 == N1.complement());
        assert(N0 != N1);
        return 0;
}
