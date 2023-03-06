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

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_vertex_base_2.h>

typedef CGAL::Delaunay_mesh_vertex_base_2<Kernel> Delaunay_mesh_vertex_base_2;
typedef CGAL::Delaunay_mesh_face_base_2<Kernel> Delaunay_mesh_face_base_2;
typedef CGAL::Triangulation_data_structure_2<Delaunay_mesh_vertex_base_2, Delaunay_mesh_face_base_2>
    Triangulation_data_structure_2;
typedef CGAL::Constrained_Delaunay_triangulation_2<Kernel, Triangulation_data_structure_2>
    Constrained_Delaunay_triangulation_2;

class Mesh2d
{
public:
        Mesh2d(const Object2d &object);

        std::size_t num_vertices() const { return triangulation.number_of_vertices(); }
        std::size_t num_faces() const { return triangulation.number_of_faces(); }

        std::vector<std::tuple<DiffReal, DiffReal>> vertices() const;
        std::vector<std::tuple<std::size_t, std::size_t, std::size_t>> faces() const;

protected:
        Constrained_Delaunay_triangulation_2 triangulation;
};

#endif // MESH2D_HPP
