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

#include <CGAL/version.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(_diffmesh, m)
{
    m.doc() = "diffmesh C++ backend";
    m.attr("CGAL_VERSION_STR") = CGAL_VERSION_STR;

    py::class_<Object2d, std::shared_ptr<Object2d>>(m, "Object2d")
        .def(py::init())
        .def_static("polygon", &Object2d::polygon, py::arg("points"))
        .def_static("rectangle", &Object2d::rectangle, py::arg("width"), py::arg("height"))
        .def("num_components", &Object2d::num_components)
        .def("num_polygons", &Object2d::num_polygons)
        .def("num_points", &Object2d::num_points)
        .def("translate", &Object2d::translate, py::arg("xdiff"), py::arg("ydiff"))
        .def("rotate", &Object2d::rotate, py::arg("angle"))
        .def("scale", &Object2d::scale, py::arg("scale"))
        .def("__repr__", &Object2d::repr);
}
