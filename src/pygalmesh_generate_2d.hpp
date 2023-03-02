/*
 * Copyright 2023 Nico Schlömer.
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

#include <memory>
#include <vector>

std::tuple<std::vector<std::array<double, 2>>, std::vector<std::array<int, 3>>>
pygalmesh_generate_2d(
    const std::vector<std::array<double, 2>> &points,
    const std::vector<std::array<int, 2>> &constraints,
    const double max_circumradius_shortest_edge_ratio,
    const double max_edge_size,
    const int num_lloyd_steps);
