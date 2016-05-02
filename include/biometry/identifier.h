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

#ifndef BIOMETRY_IDENTIFIER_H_
#define BIOMETRY_IDENTIFIER_H_

#include <biometry/do_not_copy_or_move.h>
#include <biometry/operation.h>
#include <biometry/progress.h>

#include <set>

namespace biometry
{
/// @cond
class Application;
class Reason;
class User;
/// @endcond

/// @brief Verification bundles the types passed to an observer of verification operations.
struct BIOMETRY_DLL_PUBLIC Identification
{
    typedef biometry::Progress Progress; ///< Progress information about the completion status of an operation.
    typedef std::string Reason;          ///< Details about cancelation of an operation.
    typedef std::string Error;           ///< Describes error conditions.
    typedef User Result;                 ///< Describes the result of an identification operation.
};

/// @brief Verifier abstracts verification of a user.
class BIOMETRY_DLL_PUBLIC Identifier : private DoNotCopyOrMove
{
public:
    // Safe us some typing
    typedef std::shared_ptr<Identifier> Ptr;

    /// @brief identify_user returns an operation that represents the identification of a user given a set of candidates with the given reason.
    virtual Operation<Identification>::Ptr identify_user(const Application& app, const Reason& reason) = 0;

protected:
    /// @cond
    Identifier() = default;
    /// @endcond
};
}

#endif // BIOMETRY_IDENTIFIER_H_
