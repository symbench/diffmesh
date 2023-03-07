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

#ifndef OBJECT2D_HPP
#define OBJECT2D_HPP

#include "diffreal.hpp"

#include <vector>
#include <tuple>

#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Aff_transformation_2.h>

typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2;
typedef CGAL::Aff_transformation_2<Kernel> Aff_Transformation_2;

class Object2d
{
public:
        static Object2d polygon(const std::vector<std::tuple<DiffReal, DiffReal>> &points);
        static Object2d rectangle(const DiffReal &width, const DiffReal &height);
        static Object2d circle(const DiffReal &radius, std::size_t segments = 60);

        std::size_t num_components() const;
        std::size_t num_polygons() const;
        std::size_t num_vertices() const;
        std::tuple<double, double, double, double> bbox() const;

        Object2d get_component(std::size_t index) const;
        Object2d get_polygon(std::size_t index) const;
        std::vector<std::tuple<DiffReal, DiffReal>> get_vertices() const;

        Object2d translate(const DiffReal &xdiff, const DiffReal &ydiff) const;
        Object2d translate(double xdiff, double ydiff) const
        {
                return translate(DiffReal(xdiff), DiffReal(ydiff));
        }

        Object2d rotate(const DiffReal &angle) const;
        Object2d rotate(double angle) const
        {
                return rotate(DiffReal(angle));
        }

        Object2d scale(const DiffReal &scale) const;
        Object2d scale(double scale) const
        {
                return this->scale(DiffReal(scale));
        }

        Object2d join(const Object2d &other) const;
        Object2d intersection(const Object2d &other) const;
        Object2d difference(const Object2d &other) const;

        int contains(const DiffReal &xpos, const DiffReal &ypos) const;

        std::string repr() const;

        const std::vector<Polygon_with_holes_2> &get_components() const
        {
                return components;
        }

protected:
        Object2d transform(Aff_Transformation_2 trans) const;

        std::vector<Polygon_with_holes_2> components;
};

#endif // OBJECT2D_HPP
