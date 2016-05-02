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

#ifndef BIOMETRY_VERIFIER_H_
#define BIOMETRY_VERIFIER_H_

#include <biometry/do_not_copy_or_move.h>
#include <biometry/operation.h>
#include <biometry/progress.h>

namespace biometry
{
/// @cond
class Application;
class Reason;
class User;
/// @endcond

/// @brief Verification bundles the types passed to an observer of verification operations.
struct BIOMETRY_DLL_PUBLIC Verification
{
    /// @brief Result enumerates all possible results of a verification operation.
    enum class Result
    {
        verified, ///< The given user could be verified.
        not_verified ///< The given user could not be verified.
    };

    typedef biometry::Progress Progress; ///< Progress information about the completion status of an operation.
    typedef std::string Reason;          ///< Details about cancelation of an operation.
    typedef std::string Error;           ///< Describes error conditions.
    typedef Result Result;               ///< Describes the result of a verification operation.
};

/// @brief Verifier abstracts verification of a user.
class BIOMETRY_DLL_PUBLIC Verifier : private DoNotCopyOrMove
{
public:
    // Safe us some typing
    typedef std::shared_ptr<Verifier> Ptr;

    /// @brief verify_user returns an operation that represents the verification of 'user' for 'reason'.
    virtual Operation<Verification>::Ptr verify_user(const Application& app, const User& user, const Reason& reason) = 0;

protected:
    /// @cond
    Verifier() = default;
    /// @endcond
};
}

#endif // BIOMETRY_VERIFIER_H_
