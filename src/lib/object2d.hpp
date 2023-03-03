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

#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_with_holes_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2;
typedef CGAL::Aff_transformation_2<Kernel> Aff_Transformation_2;

class Object2d
{
public:
        static Object2d polygon(const std::vector<std::array<float, 2>> &points);
        static Object2d rectangle(float width, float height);

        std::size_t num_components() const;
        std::size_t num_polygons() const;
        std::size_t num_points() const;

        Object2d get_component(std::size_t index) const;

        Object2d translate(float xdiff, float ydiff) const;
        Object2d rotate(float angle) const;
        Object2d scale(float scale) const;

        std::string repr() const;

protected:
        Object2d transform(Aff_Transformation_2 trans) const;

        std::vector<Polygon_with_holes_2> components;
};

#endif // OBJECT2d_HPP
