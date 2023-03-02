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

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Delaunay_mesh_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, Tds> CDT;
typedef CGAL::Delaunay_mesh_size_criteria_2<CDT> Criteria;
typedef CDT::Vertex_handle Vertex_handle;
typedef CDT::Point Point;

int main()
{
        CDT cdt;
        Vertex_handle va = cdt.insert(Point(-4, 0));
        Vertex_handle vb = cdt.insert(Point(0, -1));
        Vertex_handle vc = cdt.insert(Point(4, 0));
        Vertex_handle vd = cdt.insert(Point(0, 1));
        cdt.insert(Point(2, 0.6));
        cdt.insert_constraint(va, vb);
        cdt.insert_constraint(vb, vc);
        cdt.insert_constraint(vc, vd);
        cdt.insert_constraint(vd, va);
        std::cout << "Number of vertices: " << cdt.number_of_vertices() << std::endl;
        std::cout << "Meshing the triangulation..." << std::endl;
        CGAL::refine_Delaunay_mesh_2(cdt, Criteria(0.125, 0.5));
        std::cout << "Number of vertices: " << cdt.number_of_vertices() << std::endl;
}
