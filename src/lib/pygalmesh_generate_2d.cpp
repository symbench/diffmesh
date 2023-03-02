/*
 * Copyright 2023 Nico Schlömer.
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

#include "pygalmesh_generate_2d.hpp"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_vertex_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <CGAL/lloyd_optimize_mesh_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_mesh_vertex_base_2<K> Vb;
typedef CGAL::Delaunay_mesh_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, Tds> CDT;
typedef CGAL::Delaunay_mesh_size_criteria_2<CDT> Criteria;
typedef CDT::Vertex_handle Vertex_handle;
typedef CDT::Point Point;

std::tuple<std::vector<std::array<double, 2>>, std::vector<std::array<int, 3>>>
pygalmesh_generate_2d(
    const std::vector<std::array<double, 2>> &points,
    const std::vector<std::array<int, 2>> &constraints,
    const double max_circumradius_shortest_edge_ratio,
    const double max_edge_size,
    const int num_lloyd_steps)
{
        CDT cdt;
        // construct a constrained triangulation
        std::vector<Vertex_handle> vertices(points.size());
        int k = 0;
        for (auto pt : points)
        {
                vertices[k] = cdt.insert(Point(pt[0], pt[1]));
                k++;
        }
        for (auto c : constraints)
        {
                cdt.insert_constraint(vertices[c[0]], vertices[c[1]]);
        }

        // create proper mesh
        CGAL::refine_Delaunay_mesh_2(
            cdt,
            Criteria(
                0.25 / (max_circumradius_shortest_edge_ratio * max_circumradius_shortest_edge_ratio),
                max_edge_size));

        if (num_lloyd_steps > 0)
        {
                CGAL::lloyd_optimize_mesh_2(
                    cdt,
                    CGAL::parameters::max_iteration_number = num_lloyd_steps);
        }

        // convert points to vector of arrays
        std::map<Vertex_handle, int> vertex_index;
        std::vector<std::array<double, 2>> out_points(cdt.number_of_vertices());
        k = 0;
        for (auto vit : cdt.finite_vertex_handles())
        {
                out_points[k][0] = vit->point()[0];
                out_points[k][1] = vit->point()[1];
                vertex_index[vit] = k;
                k++;
        }

        // https://github.com/CGAL/cgal/issues/5068#issuecomment-706213606
        auto nb_faces = 0u;
        for (auto fit : cdt.finite_face_handles())
        {
                if (fit->is_in_domain())
                        ++nb_faces;
        }
        std::vector<std::array<int, 3>> out_cells(nb_faces);
        k = 0;
        for (auto fit : cdt.finite_face_handles())
        {
                if (!fit->is_in_domain())
                        continue;
                out_cells[k][0] = vertex_index[fit->vertex(0)];
                out_cells[k][1] = vertex_index[fit->vertex(1)];
                out_cells[k][2] = vertex_index[fit->vertex(2)];
                k++;
        }

        return std::make_tuple(out_points, out_cells);
}
