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

#include <biometry/progress.h>

#include <iostream>

biometry::Progress biometry::Progress::none()
{
    return Progress{Percent::from_raw_value(0.), Dictionary{}};
}

std::ostream& biometry::operator<<(std::ostream& out, const biometry::Progress& progress)
{
    out << "Progress: " << progress.percent;
    for (const auto& pair : progress.details)
        out << std::endl << "  " << pair.first << ": " << pair.second;
    return out;
}

bool biometry::operator==(const biometry::Progress& lhs, const biometry::Progress& rhs)
{
    return lhs.percent == rhs.percent && lhs.details == rhs.details;
}
