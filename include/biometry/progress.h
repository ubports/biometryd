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

#ifndef BIOMETRY_PROGRESS_H_
#define BIOMETRY_PROGRESS_H_

#include <biometry/dictionary.h>
#include <biometry/percent.h>
#include <biometry/visibility.h>

#include <iosfwd>

namespace biometry
{
/// @brief Progress bundles information about the current progress of an operation.
struct BIOMETRY_DLL_PUBLIC Progress
{
    /// @brief none returns a Progress instance representing no progress.
    static Progress none();

    Percent percent;         ///< Percent completed.
    Dictionary details;      ///< Extended information about the current state of the operation.
};

/// @brief operator<< inserts progress into out.
BIOMETRY_DLL_PUBLIC std::ostream& operator<<(std::ostream& out, const Progress& progress);

/// @brief operator== returns true if lhs equals rhs component-wise.
BIOMETRY_DLL_PUBLIC bool operator==(const Progress& lhs, const Progress& rhs);
}

#endif // BIOMETRY_PROGRESS_H_
