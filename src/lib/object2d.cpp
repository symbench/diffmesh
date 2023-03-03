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

#include "object2d.hpp"

#include <sstream>

typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Vector_2<Kernel> Vector_2;
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2;

Object2d Object2d::polygon(const std::vector<std::array<float, 2>> &points)
{
        Polygon_2 polygon;
        for (auto p : points)
                polygon.push_back(Point_2(p[0], p[1]));

        if (!polygon.is_simple())
                throw std::invalid_argument("polygon is not simple");

        Object2d object;
        object.components.emplace_back(polygon);
        return object;
}

Object2d Object2d::rectangle(float width, float height)
{
        width *= 0.5f;
        height *= 0.5f;
        std::vector<std::array<float, 2>> points = {
            {-width, -height},
            {+width, -height},
            {+width, +height},
            {-width, +height},
        };
        return polygon(points);
}

std::size_t Object2d::num_components() const { return components.size(); }

std::size_t Object2d::num_polygons() const
{
        std::size_t n = 0;
        for (auto &p : components)
                n += 1 + p.number_of_holes();
        return n;
}

std::size_t Object2d::num_points() const
{
        std::size_t n = 0;
        for (auto &c : components)
        {
                n += c.outer_boundary().container().size();
                for (auto &h : c.holes())
                        n += h.container().size();
        }
        return n;
}

Object2d Object2d::get_component(std::size_t index) const
{
        if (index >= components.size())
                throw std::invalid_argument("invalid index");

        Object2d object;
        object.components.emplace_back(components[index]);
        return object;
}

Object2d Object2d::transform(Aff_Transformation_2 trans) const
{
        Object2d object;
        for (auto &c : components)
        {
                Polygon_with_holes_2 polygon(CGAL::transform(trans, c.outer_boundary()));
                for (auto &h : c.holes())
                        polygon.holes().push_back(CGAL::transform(trans, h));
                object.components.push_back(polygon);
        }
        return object;
}

Object2d Object2d::translate(float xdiff, float ydiff) const
{
        return transform(Aff_Transformation_2(CGAL::TRANSLATION, Vector_2(xdiff, ydiff)));
}

Object2d Object2d::rotate(float angle) const
{
        return transform(Aff_Transformation_2(CGAL::ROTATION, std::sin(angle), std::cos(angle)));
}

Object2d Object2d::scale(float scale) const
{
        return transform(Aff_Transformation_2(CGAL::SCALING, scale));
}

std::string Object2d::repr() const
{
        std::stringstream str;
        str << "Object2d";
        for (auto &c : components)
                str << " [" << c << "]";
        return str.str();
}
