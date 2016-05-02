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

#include <biometry/geometry.h>

#include <iostream>

bool biometry::operator==(const biometry::Point& lhs, const biometry::Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool biometry::operator!=(const biometry::Point& lhs, const biometry::Point& rhs)
{
    return not (lhs == rhs);
}

bool biometry::operator==(const Rectangle& lhs, const Rectangle& rhs)
{
    return lhs.top_left == rhs.top_left && lhs.bottom_right == rhs.bottom_right;
}

bool biometry::operator!=(const Rectangle& lhs, const Rectangle& rhs)
{
    return not (lhs == rhs);
}

std::ostream& biometry::operator<<(std::ostream& out, const biometry::Point& point)
{
    return out << "(" << point.x << "," << point.y << ")";
}

/// @brief operator<< inserts rectangle into out.
std::ostream& biometry::operator<<(std::ostream& out, const biometry::Rectangle& rectangle)
{
    return out << "(" << rectangle.top_left << "," << rectangle.bottom_right << ")";
}
