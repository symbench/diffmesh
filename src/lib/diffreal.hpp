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

#ifndef DIFFREAL_HPP
#define DIFFREAL_HPP

#include <CGAL/Gmpq.h>
#include <CGAL/number_type_basic.h>
#include <CGAL/Coercion_traits.h>

class DiffReal
{
public:
        CGAL::Gmpq value;

        DiffReal() {}
        DiffReal(double value) : value(value) {}
        DiffReal(CGAL::Gmpq value) : value(value) {}
        DiffReal(const DiffReal &other) : value(other.value) {}

        double to_double() const { return CGAL::to_double(value); }

        DiffReal &operator=(const DiffReal &other)
        {
                value = other.value;
                return *this;
        }

        bool operator==(const DiffReal &other) const { return value == other.value; }
        bool operator!=(const DiffReal &other) const { return value != other.value; }
        bool operator<(const DiffReal &other) const { return value < other.value; }
        bool operator<=(const DiffReal &other) const { return value <= other.value; }
        bool operator>(const DiffReal &other) const { return value > other.value; }
        bool operator>=(const DiffReal &other) const { return value >= other.value; }

        DiffReal operator-() const { return -value; }

        DiffReal operator+(const DiffReal &other) const { return value + other.value; }
        DiffReal operator-(const DiffReal &other) const { return value - other.value; }
        DiffReal operator*(const DiffReal &other) const { return value * other.value; }
        DiffReal operator/(const DiffReal &other) const { return value / other.value; }

        DiffReal &operator+=(const DiffReal &other)
        {
                value += other.value;
                return *this;
        }
        DiffReal &operator-=(const DiffReal &other)
        {
                value -= other.value;
                return *this;
        }
        DiffReal &operator*=(const DiffReal &other)
        {
                value *= other.value;
                return *this;
        }
        DiffReal &operator/=(const DiffReal &other)
        {
                value /= other.value;
                return *this;
        }
};

std::ostream &operator<<(std::ostream &out, const DiffReal &x);
std::istream &operator>>(std::istream &in, DiffReal &x);

namespace CGAL
{
        CGAL_DEFINE_COERCION_TRAITS_FOR_SELF(DiffReal)
        CGAL_DEFINE_COERCION_TRAITS_FROM_TO(double, DiffReal)

        template <>
        class Algebraic_structure_traits<DiffReal> : public Algebraic_structure_traits_base<DiffReal, Field_tag>
        {
        public:
                typedef Tag_true Is_exact;
                typedef Tag_false Is_numerical_sensitive;

                class Is_zero
                    : public CGAL::cpp98::unary_function<Type, bool>
                {
                public:
                        inline bool operator()(const Type &x) const
                        {
                                return CGAL::is_zero(x.value);
                        }
                };

                class Is_square
                    : public CGAL::cpp98::binary_function<Type, Type &,
                                                          bool>
                {
                public:
                        inline bool operator()(const Type &x, Type &y) const
                        {
                                return CGAL::is_square(x.value, y.value);
                        }
                        inline bool operator()(const Type &x) const
                        {
                                return CGAL::is_square(x.value);
                        }
                };

                class Simplify
                    : public CGAL::cpp98::unary_function<Type &, void>
                {
                public:
                        inline void operator()(Type &x) const
                        {
                                CGAL::simplify(x.value);
                        }
                };
        };

        template <>
        class Real_embeddable_traits<DiffReal> : public INTERN_RET::Real_embeddable_traits_base<DiffReal, CGAL::Tag_true>
        {
                typedef Algebraic_structure_traits<Type> AST;

        public:
                typedef AST::Is_zero Is_zero;

                class Is_positive
                    : public CGAL::cpp98::unary_function<Type, bool>
                {
                public:
                        inline bool operator()(const Type &x) const
                        {
                                return CGAL::is_positive(x.value);
                        }
                };

                class Is_negative
                    : public CGAL::cpp98::unary_function<Type, bool>
                {
                public:
                        inline bool operator()(const Type &x) const
                        {
                                return CGAL::is_negative(x.value);
                        }
                };

                class Sgn
                    : public CGAL::cpp98::unary_function<Type, CGAL::Sign>
                {
                public:
                        inline CGAL::Sign operator()(const Type &x) const
                        {
                                return CGAL::sign(x.value);
                        }
                };

                class To_double
                    : public CGAL::cpp98::unary_function<Type, double>
                {
                public:
                        inline double operator()(const Type &x) const
                        {
                                return CGAL::to_double(x.value);
                        }
                };

                struct To_interval : public CGAL::cpp98::unary_function<Type, std::pair<double, double>>
                {
                        inline std::pair<double, double> operator()(const Type &x) const
                        {
                                return CGAL::to_interval(x.value);
                        };
                };
        };
}

#endif // DIFFREAL_HPP
