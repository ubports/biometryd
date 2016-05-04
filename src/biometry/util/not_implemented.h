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

#ifndef BIOMETRY_UTIL_NOT_IMPLEMENTED_H_
#define BIOMETRY_UTIL_NOT_IMPLEMENTED_H_

#include <stdexcept>
#include <string>

namespace biometry
{
namespace util
{
/// @brief NotImplemented is thrown from not_implemented(), indicating
/// a missing implementation.
struct NotImplemented : public std::logic_error
{
    /// @brief NotImplemented initializes a new instance for the given function name.
    explicit NotImplemented(const std::string& function);
};

/// @brief not_implemented throws NotImplemented.
__attribute__((noreturn)) void not_implemented(const std::string& function);
}
}

#endif // BIOMETRY_UTIL_NOT_IMPLEMENTED_H_
