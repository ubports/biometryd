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

#ifndef BIOMETRYD_DBUS_SKELETON_IDENTIFIER_H_
#define BIOMETRYD_DBUS_SKELETON_IDENTIFIER_H_

#include <biometry/identifier.h>

#include <biometry/dbus/skeleton/credentials_resolver.h>

#include <core/dbus/object.h>
#include <core/dbus/service.h>

#include <biometry/util/synchronized.h>

#include <unordered_map>

namespace biometry
{
namespace dbus
{
namespace skeleton
{
/// @cond
class Device;
/// @endcond

// Identifier is the dbus SKELETON implementation of biometry::Service.
class Identifier : public biometry::Identifier
{
public:
    // Safe us some typing.
    typedef std::shared_ptr<Identifier> Ptr;

    /// @brief RequestVerifier models verification of incoming requests.
    class RequestVerifier : public biometry::dbus::skeleton::RequestVerifier
    {
    public:
        /// @brief verify_identify_user_request returns true if the requesting app identified by provided
        /// is allowed to run the request described by requested.
        virtual bool verify_identify_user_request(const Application& requested, const Credentials& provided);
    };

    /// @brief create_for_bus returns a new skeleton::Identifier instance connected to bus, forwarding calls to impl.
    static Ptr create_for_service_and_object(const core::dbus::Bus::Ptr& bus,
                                             const core::dbus::Service::Ptr& service,
                                             const core::dbus::Object::Ptr& object,
                                             const std::reference_wrapper<biometry::Identifier>& impl,
                                             const std::shared_ptr<RequestVerifier>& request_verifier,
                                             const std::shared_ptr<CredentialsResolver>& credentials_resolver);

    /// @brief Frees up resources and uninstalls method handlers.
    ~Identifier();

    // From biometry::Identifier.
    Operation<Identification>::Ptr identify_user(const Application& app, const Reason& reason) override;

private:
    typedef biometry::util::Synchronized<std::unordered_map<core::dbus::types::ObjectPath, Operation<Identification>::Ptr>> IdentificationOps;

    /// @brief Service creates a new instance for the given remote service and object.
    Identifier(const core::dbus::Bus::Ptr& bus,
               const core::dbus::Service::Ptr& service,
               const core::dbus::Object::Ptr& object,
               const std::reference_wrapper<biometry::Identifier>& impl,
               const std::shared_ptr<RequestVerifier>& request_verifier,
               const std::shared_ptr<CredentialsResolver>& credentials_resolver);

    std::reference_wrapper<biometry::Identifier> impl;
    std::shared_ptr<RequestVerifier> request_verifier;
    std::shared_ptr<CredentialsResolver> credentials_resolver;

    core::dbus::Bus::Ptr bus;
    core::dbus::Service::Ptr service;
    core::dbus::Object::Ptr object;

    IdentificationOps ops;
};
}
}
}

#endif // BIOMETRYD_DBUS_SKELETON_IDENTIFIER_H_
