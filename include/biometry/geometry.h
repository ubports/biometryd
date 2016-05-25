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

#ifndef BIOMETRY_GEOMETRY_H_
#define BIOMETRY_GEOMETRY_H_

#include <biometry/visibility.h>

#include <cstdint>
#include <iosfwd>

namespace biometry
{
/// @brief Point models a point in a normalized space [0,1]^2.
struct BIOMETRY_DLL_PUBLIC Point
{
    /// @brief Point initializes a new instance with x, y.
    /// @ŧhrows std::runtime_error if x or y is not in [0,1].
    Point(double x = 0.f, double y = 0.f);

    double x;
    double y;
};

/// @brief Rectangle models a rectangular region in a 2d space,
/// defined by its top_left (t) and bottom_right (b) corners.
///
/// The origin of the coordinate system is located in the top-left corner
/// extending to the right and downwards.
///
/// o-------------------- [x]
/// |
/// |     (t)·····
/// |      ·     ·
/// |      ·     ·
/// |      ·····(b)
/// |
///[y]
///
struct BIOMETRY_DLL_PUBLIC Rectangle
{
    Point top_left; ///< 2d coordinate of the top-left corner.
    Point bottom_right; ///< 2d coordinate of the bottom-right corner.
};

/// @brief operator== returns true if lhs and rhs compare equal component-wise.
BIOMETRY_DLL_PUBLIC bool operator==(const Point&, const Point&);

/// @brief operator== returns true if lhs and rhs compare not equal in any of the components.
BIOMETRY_DLL_PUBLIC bool operator!=(const Point&, const Point&);

/// @brief operator== returns true if lhs and rhs compare equal component-wise.
BIOMETRY_DLL_PUBLIC bool operator==(const Rectangle&, const Rectangle&);

/// @brief operator== returns true if lhs and rhs compare not-equal in any of the components.
BIOMETRY_DLL_PUBLIC bool operator!=(const Rectangle&, const Rectangle&);

/// @brief operator<< inserts point into out.
BIOMETRY_DLL_PUBLIC std::ostream& operator<<(std::ostream& out, const Point& point);

/// @brief operator<< inserts rectangle into out.
BIOMETRY_DLL_PUBLIC std::ostream& operator<<(std::ostream& out, const Rectangle& rectangle);
}

#endif // BIOMETRY_GEOMETRY_H_
