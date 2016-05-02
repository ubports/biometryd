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

#ifndef BIOMETRY_PERCENT_H_
#define BIOMETRY_PERCENT_H_

#include <biometry/visibility.h>

#include <iosfwd>

namespace biometry
{
/// @brief Percent represents a value from the interval [0,1].
class BIOMETRY_DLL_PUBLIC Percent
{
public:
    /// @brief from_raw_value returns a new Percent instance, initialized to the given value.
    /// Throws std::runtime_error if value is not in [0,1].
    static Percent from_raw_value(double value);

    /// @brief Percent initializes the instance to undefined.
    Percent();

    /// @brief operator* returns the raw value.
    double operator*() const;

private:
    explicit Percent(double value);
    double value;
};

/// @brief operator<< inserts percent into out.
BIOMETRY_DLL_PUBLIC std::ostream& operator<<(std::ostream& out, const Percent& percent);
/// @brief operator== compares lhs and rhs for equality.
BIOMETRY_DLL_PUBLIC bool operator==(const Percent& lhs, const Percent& rhs);
}

#endif // BIOMETRY_PERCENT_H_
