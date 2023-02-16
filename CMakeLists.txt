cmake_minimum_required(VERSION 3.16)
project(diffmesh CXX)
set (CMAKE_CXX_STANDARD 11)

set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE "TRUE")
find_package(CGAL QUIET REQUIRED COMPONENTS Core)

add_executable(test
    src/test.cpp)
target_link_libraries(test CGAL::CGAL)
