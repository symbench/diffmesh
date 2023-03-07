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

#ifndef MESH2D_HPP
#define MESH2D_HPP

#include "diffreal.hpp"
#include "object2d.hpp"

#include <vector>
#include <tuple>
#include <limits>

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_vertex_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>

class Mesh2d
{
public:
    Mesh2d(const Object2d &object);

    void refine_delaunay(double aspect_bound = 0.125, double size_bound = 0.0);
    void lloyd_optimize(int max_iteration_number = 0);

    std::size_t num_vertices() const { return triangulation.number_of_vertices(); }
    std::size_t num_faces() const { return d_num_faces; }

    std::vector<std::tuple<DiffReal, DiffReal>> vertices() const;
    std::vector<std::tuple<std::size_t, std::size_t, std::size_t>> faces() const;

protected:
    static const std::size_t UNSET = std::numeric_limits<std::size_t>::max();

    struct VertexInfo
    {
        std::size_t index;
    };

    struct FaceInfo
    {
        std::size_t depth;

        bool inside() const { return depth != UNSET && depth % 2 == 1; }
    };

    typedef CGAL::Triangulation_vertex_base_with_info_2<VertexInfo, Kernel>
        Triangulation_vertex_base_with_info_2;
    typedef CGAL::Delaunay_mesh_vertex_base_2<Kernel, Triangulation_vertex_base_with_info_2>
        Delaunay_mesh_vertex_base_2;

    typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo, Kernel>
        Triangulation_face_base_with_info_2;
    typedef CGAL::Constrained_triangulation_face_base_2<Kernel, Triangulation_face_base_with_info_2>
        Constrained_triangulation_face_base_2;
    typedef CGAL::Constrained_Delaunay_triangulation_face_base_2<Kernel, Constrained_triangulation_face_base_2>
        Constrained_Delaunay_triangulation_face_base_2;
    typedef CGAL::Delaunay_mesh_face_base_2<Kernel, Constrained_Delaunay_triangulation_face_base_2>
        Delaunay_mesh_face_base_2;

    typedef CGAL::Triangulation_data_structure_2<Delaunay_mesh_vertex_base_2, Delaunay_mesh_face_base_2>
        Triangulation_data_structure_2;
    typedef CGAL::Constrained_Delaunay_triangulation_2<Kernel, Triangulation_data_structure_2, CGAL::No_constraint_intersection_tag>
        Constrained_Delaunay_triangulation_2;

    typedef CGAL::Point_2<Kernel> Point_2;
    typedef Constrained_Delaunay_triangulation_2::Vertex_handle Vertex_handle;
    typedef Constrained_Delaunay_triangulation_2::Face_handle Face_handle;
    typedef Constrained_Delaunay_triangulation_2::Edge Edge;
    typedef CGAL::Delaunay_mesh_size_criteria_2<Constrained_Delaunay_triangulation_2>
        Delaunay_mesh_size_criteria_2;

    void set_extra_info();

    Constrained_Delaunay_triangulation_2 triangulation;
    std::vector<Point_2> seeds;

    size_t d_num_vertices;
    size_t d_num_faces;
};

#endif // MESH2D_HPP
