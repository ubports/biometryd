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

#ifndef BIOMETRY_TEMPLATE_STORE_H_
#define BIOMETRY_TEMPLATE_STORE_H_

#include <biometry/operation.h>
#include <biometry/progress.h>
#include <biometry/void.h>

namespace biometry
{
/// @cond
class Application;
class User;
/// @endcond
///
/// @brief TemplateStore models maintenance of a device-specific template store in way that
/// ensures that no template data ever crosses over the wire (or would need to be extracted from a TEE).
class TemplateStore : public DoNotCopyOrMove
{
public:
    /// @brief TemplateId is a numeric uniquely identifying a biometric template.
    typedef std::uint64_t TemplateId;

    /// @brief SizeQuery bundles the types passed to an observer of a size operation.
    struct SizeQuery
    {
        typedef biometry::Progress Progress; ///< Progress information about the completion status of an operation.
        typedef std::string Reason;          ///< Details about cancelation of an operation.
        typedef std::string Error;           ///< Describes error conditions.
        typedef std::uint32_t Result;        ///< Describes the result of a SizeQuery operation.
    };

    /// @brief List bundles the types passed to an observer of a size operation.
    struct List
    {
        typedef biometry::Progress Progress;    ///< Progress information about the completion status of an operation.
        typedef std::string Reason;             ///< Details about cancelation of an operation.
        typedef std::string Error;              ///< Describes error conditions.
        typedef std::vector<TemplateId >Result; ///< Describes the result of a List operation.
    };

    /// @brief Enrollment bundles the types passed to an observer of enrollment operations.
    struct Enrollment
    {
        typedef biometry::Progress Progress; ///< Progress information about the completion status of an operation.
        typedef std::string Reason;          ///< Details about cancelation of an operation.
        typedef std::string Error;           ///< Describes error conditions.
        typedef TemplateId Result;           ///< Describes the result of an Enrollment operation.
    };

    /// @brief Remove bundles the types passed to an observer of a removal operation.
    struct Removal
    {
        typedef biometry::Progress Progress; ///< Progress information about the completion status of an operation.
        typedef std::string Reason;          ///< Details about cancelation of an operation.
        typedef std::string Error;           ///< Describes error conditions.
        typedef TemplateId Result;           ///< Describes the result of an Enrollment operation.
    };

    /// @brief Clearance bundles the types passed to an observer of clearance operations.
    struct Clearance
    {
        typedef biometry::Progress Progress; ///< Progress information about the completion status of an operation.
        typedef std::string Reason;          ///< Details about cancelation of an operation.
        typedef std::string Error;           ///< Describes error conditions.
        typedef Void Result;                 ///< Describes the result of a Clearance operation.
    };

    /// @brief size() returns the number of templates known for user.
    /// @param app The application requesting the information.
    /// @param user The user for which we want to query the number of known templates.
    virtual Operation<SizeQuery>::Ptr size(const Application& app, const User& user) = 0;

    /// @brief list returns an operation that yields the list of all templates enrolled for app and user.
    /// @param app The application requesting the information.
    /// @param user The user for which we want to query all enrolled templates.
    virtual Operation<List>::Ptr list(const Application& app, const User& user) = 0;

    /// @brief enroll returns an operation that represents the enrollment of a new template for a user.
    /// @param app The application requesting the enrollment operation.
    /// @param user The user for which we want to enroll the new template.
    virtual Operation<Enrollment>::Ptr enroll(const Application& app, const User& user) = 0;

    /// @brief remove returns an operation that represents the removal of an individual template.
    /// @param app The application requesting the removal operation.
    /// @param user The user for which we want to remove a specific template.
    /// @param id The id of the template that should be removed.
    virtual Operation<Removal>::Ptr remove(const Application& app, const User& user, TemplateId id) = 0;

    /// @brief clear returns an operation that represents removal of all templates associated to user.
    /// @param app The application requesting the clear operation.
    /// @param user The user for which we want to clear templates for.
    virtual Operation<Clearance>::Ptr clear(const Application& app, const User& user) = 0;
};
}

#endif // BIOMETRY_TEMPLATE_STORE_H_
