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

#include "diffreal.hpp"
#include "object2d.hpp"

#include <CGAL/version.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(_diffmesh, m)
{
    m.doc() = "diffmesh C++ backend";
    m.attr("CGAL_VERSION_STR") = CGAL_VERSION_STR;

    py::class_<DiffReal, std::shared_ptr<DiffReal>>(m, "DiffReal")
        .def(py::init())
        .def(py::init<double>(), py::arg("value"))
        .def(py::init<double, std::vector<double>>(), py::arg("value"), py::arg("derivs"))
        .def("value", &DiffReal::get_value)
        .def("derivs", &DiffReal::get_derivs)
        .def("__eq__", &DiffReal::operator==, py::arg("other"))
        .def("__ne__", &DiffReal::operator!=, py::arg("other"))
        .def("__lt__", &DiffReal::operator<, py::arg("other"))
        .def("__le__", &DiffReal::operator<=, py::arg("other"))
        .def("__gt__", &DiffReal::operator>, py::arg("other"))
        .def("__ge__", &DiffReal::operator>=, py::arg("other"))
        .def("__neg__", static_cast<DiffReal (DiffReal::*)() const>(&DiffReal::operator-))
        .def("__add__", static_cast<DiffReal (DiffReal::*)(const DiffReal &) const>(&DiffReal::operator+), py::arg("other"))
        .def("__sub__", static_cast<DiffReal (DiffReal::*)(const DiffReal &) const>(&DiffReal::operator-), py::arg("other"))
        .def("__mul__", &DiffReal::operator*, py::arg("other"))
        .def("__truediv__", &DiffReal::operator/, py::arg("other"))
        .def("__iadd__", &DiffReal::operator+=, py::arg("other"))
        .def("__isub__", &DiffReal::operator-=, py::arg("other"))
        .def("__imul__", &DiffReal::operator*=, py::arg("other"))
        .def("__idiv__", &DiffReal::operator/=, py::arg("other"));

    py::class_<Object2d, std::shared_ptr<Object2d>>(m, "Object2d")
        .def(py::init())
        .def_static("polygon", &Object2d::polygon, py::arg("points"))
        .def_static("rectangle", &Object2d::rectangle, py::arg("width"), py::arg("height"))
        .def_static("circle", &Object2d::circle, py::arg("radius"), py::arg("segments") = 60)
        .def("num_components", &Object2d::num_components)
        .def("num_polygons", &Object2d::num_polygons)
        .def("num_points", &Object2d::num_points)
        .def("bbox", &Object2d::bbox)
        .def("get_component", &Object2d::get_component, py::arg("index"))
        .def("get_polygon", &Object2d::get_polygon, py::arg("index"))
        .def("get_points", &Object2d::get_points)
        .def("translate", &Object2d::translate, py::arg("xdiff"), py::arg("ydiff"))
        .def("rotate", &Object2d::rotate, py::arg("angle"))
        .def("scale", &Object2d::scale, py::arg("scale"))
        .def("join", &Object2d::join, py::arg("other"))
        .def("intersection", &Object2d::intersection, py::arg("other"))
        .def("difference", &Object2d::difference, py::arg("other"))
        .def("__repr__", &Object2d::repr);
}
