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

std::vector<double> DiffReal::get_derivs() const
{
        std::vector<double> result;
        result.reserve(derivs.size());
        for (std::size_t i = 0; i < derivs.size(); i++)
                result.emplace_back(CGAL::to_double(derivs[i]));
        return result;
}

DiffReal DiffReal::operator-() const
{
        DiffReal result;
        result.value = -value;
        result.derivs.reserve(derivs.size());
        for (size_t i = 0; i < derivs.size(); i++)
                result.derivs.emplace_back(-derivs[i]);
        return result;
}

DiffReal DiffReal::operator+(const DiffReal &other) const
{
        DiffReal result(*this);
        result += other;
        return result;
}

DiffReal DiffReal::operator-(const DiffReal &other) const
{
        DiffReal result(*this);
        result -= other;
        return result;
}

DiffReal DiffReal::operator*(const DiffReal &other) const
{
        DiffReal result(*this);
        result *= other;
        return result;
}

DiffReal DiffReal::operator/(const DiffReal &other) const
{
        DiffReal result(*this);
        result /= other;
        return result;
}

DiffReal &DiffReal::operator+=(const DiffReal &other)
{
        value += other.value;
        std::size_t min_size = std::min(derivs.size(), other.derivs.size());
        for (std::size_t i = 0; i < min_size; i++)
                derivs[i] += other.derivs[i];
        for (std::size_t i = min_size; i < other.derivs.size(); i++)
                derivs.emplace_back(other.derivs[i]);
        return *this;
}

DiffReal &DiffReal::operator-=(const DiffReal &other)
{
        value -= other.value;
        std::size_t min_size = std::min(derivs.size(), other.derivs.size());
        for (std::size_t i = 0; i < min_size; i++)
                derivs[i] -= other.derivs[i];
        for (std::size_t i = min_size; i < other.derivs.size(); i++)
                derivs.emplace_back(-other.derivs[i]);
        return *this;
}

DiffReal &DiffReal::operator*=(const DiffReal &other)
{
        double temp1 = CGAL::to_double(other.value);
        double temp2 = CGAL::to_double(value);
        value *= other.value;
        std::size_t min_size = std::min(derivs.size(), other.derivs.size());
        for (std::size_t i = 0; i < derivs.size(); i++)
                derivs[i] *= temp1;
        for (std::size_t i = 0; i < min_size; i++)
                derivs[i] += temp2 * other.derivs[i];
        for (std::size_t i = min_size; i < other.derivs.size(); i++)
                derivs.emplace_back(temp2 * other.derivs[i]);
        return *this;
}

DiffReal &DiffReal::operator/=(const DiffReal &other)
{
        double temp1 = 1.0 / CGAL::to_double(other.value);
        double temp2 = CGAL::to_double(value) * temp1 * temp1;
        value /= other.value;
        std::size_t min_size = std::min(derivs.size(), other.derivs.size());
        for (std::size_t i = 0; i < derivs.size(); i++)
                derivs[i] *= temp1;
        for (std::size_t i = 0; i < min_size; i++)
                derivs[i] -= temp2 * other.derivs[i];
        for (std::size_t i = min_size; i < other.derivs.size(); i++)
                derivs.emplace_back(-temp2 * other.derivs[i]);
        return *this;
}

DiffReal DiffReal::cos() const
{
        DiffReal result;
        double temp1 = CGAL::to_double(value);
        result.value = std::cos(temp1);
        double temp2 = -std::sin(temp1);
        result.derivs.reserve(derivs.size());
        for (size_t i = 0; i < derivs.size(); i++)
                result.derivs.emplace_back(derivs[i] * temp2);
        return result;
}

DiffReal DiffReal::sin() const
{
        DiffReal result;
        double temp1 = CGAL::to_double(value);
        result.value = std::sin(temp1);
        double temp2 = std::cos(temp1);
        result.derivs.reserve(derivs.size());
        for (size_t i = 0; i < derivs.size(); i++)
                result.derivs.emplace_back(derivs[i] * temp2);
        return result;
}

std::string DiffReal::repr() const
{
        std::stringstream str;
        str << CGAL::to_double(value);
        return str.str();
}

DiffReal operator*(double left, const DiffReal &right)
{
        return DiffReal(left) * right;
}

DiffReal operator/(double left, const DiffReal &right)
{
        return DiffReal(left) * right;
}

std::ostream &operator<<(std::ostream &out, const DiffReal &x)
{
        out << x.value;
        return out;
}

std::istream &operator>>(std::istream &in, DiffReal &x)
{
        in >> x.value;
        return in;
}
