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

#include "mesh2d.hpp"

#include <map>

typedef Constrained_Delaunay_triangulation_2::Vertex_handle Vertex_handle;

Mesh2d::Mesh2d(const Object2d &object)
{
        const std::vector<Polygon_with_holes_2> &components = object.get_components();
        for (auto &c : components)
        {
                auto &p = c.outer_boundary();
                triangulation.insert_constraint(p.vertices_begin(), p.vertices_end(), true);
                for (auto &p : c.holes())
                        triangulation.insert_constraint(p.vertices_begin(), p.vertices_end(), true);
        }
}

std::vector<std::tuple<DiffReal, DiffReal>> Mesh2d::vertices() const
{
        std::vector<std::tuple<DiffReal, DiffReal>> result;
        result.reserve(triangulation.number_of_vertices());
        for (auto &v : triangulation.finite_vertex_handles())
        {
                auto &p = v->point();
                result.emplace_back(p.x(), p.y());
        }
        return result;
}

std::vector<std::tuple<std::size_t, std::size_t, std::size_t>> Mesh2d::faces() const
{
        std::map<Vertex_handle, std::size_t> vertex_index;
        for (auto &v : triangulation.finite_vertex_handles())
        {
                std::size_t i = vertex_index.size();
                vertex_index[v] = i;
        }

        std::vector<std::tuple<std::size_t, std::size_t, std::size_t>> result;
        result.reserve(triangulation.number_of_faces());
        for (auto &f : triangulation.finite_face_handles())
        {
                auto v0 = vertex_index[f->vertex(0)];
                auto v1 = vertex_index[f->vertex(1)];
                auto v2 = vertex_index[f->vertex(2)];
                result.emplace_back(v0, v1, v2);
        }
        return result;
}
