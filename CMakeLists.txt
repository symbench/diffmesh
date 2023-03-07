cmake_minimum_required(VERSION 3.16)
project(diffmesh CXX)
set(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE(pybind11 REQUIRED)

set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE "TRUE")
find_package(CGAL REQUIRED)

pybind11_add_module(_diffmesh
    src/lib/mesh2d.cpp
    src/lib/object2d.cpp
    src/lib/diffreal.cpp
    src/lib/pybind11.cpp)

target_link_libraries(_diffmesh PRIVATE CGAL::CGAL)

install(TARGETS _diffmesh LIBRARY DESTINATION diffmesh)
