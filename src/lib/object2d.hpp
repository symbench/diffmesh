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

// #include <CGAL/Gmpq.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_with_holes_2.h>

// typedef CGAL::Cartesian<CGAL::Gmpq> Kernel;
typedef CGAL::Cartesian<DiffReal> Kernel;

typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes_2;
typedef CGAL::Aff_transformation_2<Kernel> Aff_Transformation_2;

class Object2d
{
public:
        static Object2d polygon(const std::vector<std::tuple<double, double>> &points);
        static Object2d rectangle(double width, double height);
        static Object2d circle(double radius, std::size_t segments = 60);

        std::size_t num_components() const;
        std::size_t num_polygons() const;
        std::size_t num_points() const;

        Object2d get_component(std::size_t index) const;
        Object2d get_polygon(std::size_t index) const;
        std::vector<std::tuple<double, double>> get_points() const;

        Object2d translate(double xdiff, double ydiff) const;
        Object2d rotate(double angle) const;
        Object2d scale(double scale) const;

        Object2d join(const Object2d &other) const;
        Object2d intersection(const Object2d &other) const;
        Object2d difference(const Object2d &other) const;

        std::string repr() const;

protected:
        Object2d transform(Aff_Transformation_2 trans) const;

        std::vector<Polygon_with_holes_2> components;
};

#endif // OBJECT2d_HPP
