cmake_minimum_required(VERSION 3.16)
project(diffmesh CXX)
set(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE(pybind11 REQUIRED)

set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE "TRUE")
find_package(CGAL REQUIRED)

pybind11_add_module(_diffmesh
    src/pybind11.cpp)

target_link_libraries(_diffmesh PRIVATE CGAL::CGAL)

install(TARGETS _diffmesh LIBRARY DESTINATION diffmesh)

add_executable(test1
    src/test1.cpp)
target_link_libraries(test1 CGAL::CGAL)

add_executable(test2
    src/test2.cpp)
target_link_libraries(test2 CGAL::CGAL)
