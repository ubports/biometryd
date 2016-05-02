/*
 * Copyright (C) 2016 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Thomas Voß <thomas.voss@canonical.com>
 *
 */

#include <biometry/percent.h>

#include <iostream>
#include <stdexcept>

namespace
{
double throw_if_out_of_range(double value)
{
    if (value < 0 || value > 1)
        throw std::runtime_error{std::to_string(value) + " not in [0,1]"};
    return value;
}
}

biometry::Percent::Percent() : value{-1}
{
}

biometry::Percent biometry::Percent::from_raw_value(double value)
{
    return biometry::Percent{value};
}

biometry::Percent::Percent(double value) : value{throw_if_out_of_range(value)}
{
}

double biometry::Percent::operator*() const
{
    return value;
}

std::ostream& biometry::operator<<(std::ostream& out, const biometry::Percent& percent)
{
    return out << *percent * 100 << "%";
}

bool biometry::operator==(const biometry::Percent& lhs, const biometry::Percent& rhs)
{
    return !(*lhs < *rhs) && !(*lhs > *rhs);
}
