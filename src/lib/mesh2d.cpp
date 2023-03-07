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
#include <CGAL/lloyd_optimize_mesh_2.h>

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

        set_extra_info();
}

void Mesh2d::refine_delaunay(double aspect_bound, double size_bound)
{
        CGAL::refine_Delaunay_mesh_2(
            triangulation,
            seeds.begin(), seeds.end(),
            Delaunay_mesh_size_criteria_2(aspect_bound = aspect_bound, size_bound = size_bound));

        set_extra_info();
}

void Mesh2d::lloyd_optimize(int max_iteration_number)
{
        throw std::logic_error("not implemented");
        // CGAL::lloyd_optimize_mesh_2(triangulation, CGAL::parameters::max_iteration_number = max_iteration_number);
}

void Mesh2d::set_extra_info()
{
        for (auto &v : triangulation.all_vertex_handles())
                v->info().index = UNSET;

        for (auto &f : triangulation.all_face_handles())
                f->info().depth = UNSET;

        d_num_vertices = 0;
        d_num_faces = 0;

        std::vector<Face_handle> faces;
        Face_handle face = triangulation.infinite_face();
        face->info().depth = 0;
        faces.push_back(face);

        while (!faces.empty())
        {
                face = faces.back();
                faces.pop_back();

                for (int i = 0; i < 3; i++)
                {
                        Face_handle next = face->neighbor(i);
                        if (next->info().depth != UNSET)
                                continue;

                        bool constrained = triangulation.is_constrained(Edge(face, i));
                        next->info().depth = face->info().depth + constrained;
                        faces.push_back(next);

                        if (!next->info().inside())
                                continue;

                        d_num_faces += 1;
                        for (int j = 0; j < 3; j++)
                        {
                                auto &info = next->vertex(j)->info();
                                if (info.index == UNSET)
                                        info.index = d_num_vertices++;
                        }
                }
        }

        if (false)
        {
                std::cout << "vertices " << d_num_vertices << std::endl;
                for (auto &v : triangulation.all_vertex_handles())
                        std::cout << v->info().index << std::endl;

                std::cout << "faces " << d_num_faces << std::endl;
                for (auto &f : triangulation.all_face_handles())
                {
                        auto v0 = f->vertex(0)->info().index;
                        auto v1 = f->vertex(1)->info().index;
                        auto v2 = f->vertex(2)->info().index;
                        std::cout << f->info().depth << " " << v0 << " " << v1 << " " << v2 << std::endl;
                }
        }
}

std::vector<std::tuple<DiffReal, DiffReal>> Mesh2d::vertices() const
{
        std::size_t count = 0;
        std::vector<std::tuple<DiffReal, DiffReal>> result(d_num_vertices);
        for (auto &v : triangulation.all_vertex_handles())
        {
                std::size_t index = v->info().index;
                if (index < d_num_vertices)
                {
                        auto p = v->point();
                        result[index] = {p.x(), p.y()};
                        count += 1;
                }
        }
        assert(count == d_num_vertices);
        return result;
}

std::vector<std::tuple<std::size_t, std::size_t, std::size_t>> Mesh2d::faces() const
{
        std::vector<std::tuple<std::size_t, std::size_t, std::size_t>> result;
        result.reserve(d_num_faces);
        for (auto &f : triangulation.all_face_handles())
        {
                if (!f->info().inside())
                        continue;

                auto v0 = f->vertex(0)->info().index;
                auto v1 = f->vertex(1)->info().index;
                auto v2 = f->vertex(2)->info().index;
                result.emplace_back(v0, v1, v2);
        }

        assert(result.size() == d_num_faces);
        return result;
}
