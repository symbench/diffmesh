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
#include <CGAL/Polygon_set_2.h>
#include <CGAL/Bbox_2.h>

typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Vector_2<Kernel> Vector_2;
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef CGAL::Polygon_set_2<Kernel> Polygon_set_2;
typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2;

Object2d Object2d::polygon(const std::vector<std::tuple<DiffReal, DiffReal>> &points)
{
        Polygon_2 polygon;
        for (auto p : points)
                polygon.push_back(Point_2(std::get<0>(p), std::get<1>(p)));

        if (!polygon.is_simple())
                throw std::invalid_argument("polygon is not simple");

        Object2d object;
        object.components.emplace_back(polygon);
        return object;
}

Object2d Object2d::rectangle(const DiffReal &width, const DiffReal &height)
{
        DiffReal width2(width * 0.5);
        DiffReal height2(height * 0.5);
        std::vector<std::tuple<DiffReal, DiffReal>> points = {
            {-width2, -height2},
            {+width2, -height2},
            {+width2, +height2},
            {-width2, +height2},
        };
        return polygon(points);
}

Object2d Object2d::circle(const DiffReal &radius, std::size_t segments)
{
        if (radius <= 0.0 || segments < 3)
                throw std::invalid_argument("invalid radius or segments");

        double step = 2.0 * 3.14159265358979323846 / segments;
        std::vector<std::tuple<DiffReal, DiffReal>> points;
        for (std::size_t i = 0; i < segments; ++i)
                points.emplace_back(radius * std::cos(i * step), radius * std::sin(i * step));

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

std::size_t Object2d::num_vertices() const
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

std::tuple<double, double, double, double> Object2d::bbox() const
{
        CGAL::Bbox_2 bbox;
        for (auto &c : components)
                bbox += c.bbox();

        return {bbox.xmin(), bbox.ymin(), bbox.xmax(), bbox.ymax()};
}

Object2d Object2d::get_component(std::size_t index) const
{
        if (index >= components.size())
                throw std::invalid_argument("invalid component index");

        Object2d object;
        object.components.emplace_back(components[index]);
        return object;
}

Object2d Object2d::get_polygon(std::size_t index) const
{
        if (components.size() != 1)
                throw std::invalid_argument("must have a single component");

        if (index > components[0].number_of_holes())
                throw std::invalid_argument("invalid polygon index");

        Polygon_2 polygon;

        if (index == 0)
                polygon = components[0].outer_boundary();
        else
        {
                polygon = components[0].holes()[index - 1];
                polygon.reverse_orientation();
        }
        assert(polygon.is_simple() && polygon.is_counterclockwise_oriented());

        Object2d object;
        object.components.emplace_back(polygon);
        return object;
}

std::vector<std::tuple<DiffReal, DiffReal>> Object2d::get_vertices() const
{
        if (components.size() != 1)
                throw std::invalid_argument("must have a single component");

        if (components[0].number_of_holes() != 0)
                throw std::invalid_argument("must have no holes");

        Polygon_2 polygon = components[0].outer_boundary();

        std::vector<std::tuple<DiffReal, DiffReal>> result;
        for (auto &v : polygon.container())
                result.push_back({v.x(), v.y()});
        return result;
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

Object2d Object2d::translate(const DiffReal &xdiff, const DiffReal &ydiff) const
{
        return transform(Aff_Transformation_2(CGAL::TRANSLATION, Vector_2(xdiff, ydiff)));
}

Object2d Object2d::rotate(const DiffReal &angle) const
{
        return transform(Aff_Transformation_2(CGAL::ROTATION, angle.sin(), angle.cos()));
}

Object2d Object2d::scale(const DiffReal &scale) const
{
        return transform(Aff_Transformation_2(CGAL::SCALING, scale));
}

Object2d Object2d::join(const Object2d &other) const
{
        Polygon_set_2 set1;
        for (auto &c : components)
                set1.insert(c);

        Polygon_set_2 set2;
        for (auto &c : other.components)
                set2.insert(c);

        set1.join(set2);
        Object2d object;
        set1.polygons_with_holes(std::back_inserter(object.components));
        return object;
}

Object2d Object2d::intersection(const Object2d &other) const
{
        Polygon_set_2 set1;
        for (auto &c : components)
                set1.insert(c);

        Polygon_set_2 set2;
        for (auto &c : other.components)
                set2.insert(c);

        set1.intersection(set2);
        Object2d object;
        set1.polygons_with_holes(std::back_inserter(object.components));
        return object;
}

Object2d Object2d::difference(const Object2d &other) const
{
        Polygon_set_2 set1;
        for (auto &c : components)
                set1.insert(c);

        Polygon_set_2 set2;
        for (auto &c : other.components)
                set2.insert(c);

        set1.difference(set2);
        Object2d object;
        set1.polygons_with_holes(std::back_inserter(object.components));
        return object;
}

std::string Object2d::repr() const
{
        std::stringstream str;
        str << "Object2d";
        for (auto &c : components)
                str << " [" << c << "]";
        return str.str();
}
