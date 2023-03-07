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
#include <CGAL/Triangulation_conformer_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <CGAL/lloyd_optimize_mesh_2.h>

typedef Constrained_Delaunay_triangulation_2::Vertex_handle Vertex_handle;
typedef Constrained_Delaunay_triangulation_2::Face_handle Face_handle;
typedef Constrained_Delaunay_triangulation_2::Edge Edge;
typedef CGAL::Delaunay_mesh_size_criteria_2<Constrained_Delaunay_triangulation_2>
    Delaunay_mesh_size_criteria_2;

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

        for (auto &f : triangulation.finite_face_handles())
        {
                auto p0 = f->vertex(0)->point();
                auto p1 = f->vertex(1)->point();
                auto p2 = f->vertex(2)->point();
                auto c = CGAL::centroid(p0, p1, p2);

                if (object.contains(c.x(), c.y()) < 0)
                        seeds.push_back(c);
        }
}

void Mesh2d::refine_delaunay(double aspect_bound, double size_bound)
{
        CGAL::refine_Delaunay_mesh_2(
            triangulation,
            seeds.begin(), seeds.end(),
            Delaunay_mesh_size_criteria_2(aspect_bound = aspect_bound, size_bound = size_bound));
}

void Mesh2d::lloyd_optimize(int max_iteration_number)
{
        throw std::logic_error("not implemented");
        // CGAL::lloyd_optimize_mesh_2(triangulation, CGAL::parameters::max_iteration_number = max_iteration_number);
}

void set_face_info2(Constrained_Delaunay_triangulation_2 &triangulation,
                    Face_handle face, int level, std::vector<Edge> &border)
{
        /*
                std::vector<Face_handle> faces;
                faces.push_back(face);

                while (!faces.empty())
                {
                        face = faces.back();
                        faces.pop_back();
                        if (face->info().level >= 0)
                                continue;

                        face->info().level = level;
                        for (int i = 0; i < 3; i++)
                        {
                                Edge edge(face, i);
                                Face_handle next = face->neighbor(i);
                        }
                }
        */
}

void Mesh2d::set_extra_info()
{
        /*
                for (auto &f : triangulation.all_face_handles())
                        f->info().level = -1;

                Face_handle face = triangulation.infinite_face();
                face->info().level = 0;
                std::vector<Face_handle> faces;
                faces.push_back(face);

                while (!faces.empty())
                {
                        face = faces.back();
                        faces.pop_back();

                        for (int i = 0; i < 0; i++)
                        {
                                Face_handle next = face->neighbor(i);
                                if (next->info().level >= 0)
                                        continue;
                        }
                }

                std::vector<Edge> border;
                set_face_info2(triangulation, triangulation.infinite_face(), 0, border);
                while (!border.empty())
                {
                        Edge edge = border.back();
                        border.pop_back();
                        Face_handle face = edge.first->neighbor(edge.second);
                        set_face_info2(triangulation, face, edge.first->info().level + 1, border);
                }

                std::size_t index = 0;
                for (auto &v : triangulation.finite_vertex_handles())
                        v->info().index = index++;
        */
}

std::vector<std::tuple<DiffReal, DiffReal>> Mesh2d::vertices() const
{
        std::vector<std::tuple<DiffReal, DiffReal>> result;
        result.reserve(triangulation.number_of_vertices());
        for (auto &v : triangulation.finite_vertex_handles())
        {
                assert(v->info().index == result.size());
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
