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

#include <biometry/dbus/skeleton/daemon_credentials_resolver.h>

#include <biometry/application.h>
#include <biometry/user.h>

#include <core/dbus/macros.h>
#include <core/dbus/object.h>

#include <core/posix/this_process.h>

namespace
{
bool is_running_in_a_testing_environment()
{
    return core::posix::this_process::env::get("BIOMETRYD_DBUS_SKELETON_IS_RUNNING_UNDER_TESTING", "0") == "1";
}

struct DBus
{
    static const std::string& name()
    {
        static const std::string s = "org.freedesktop.DBus";
        return s;
    }

    // Gets the AppArmor confinement string associated with the unique connection name. If
    // D-Bus is not performing AppArmor mediation, the
    // org.freedesktop.DBus.Error.AppArmorSecurityContextUnknown error is returned.
    DBUS_CPP_METHOD_DEF(GetConnectionAppArmorSecurityContext, DBus)

    // Gets the unix user id of the user that the requesting party is running under.
    DBUS_CPP_METHOD_DEF(GetConnectionUnixUser, DBus)

    struct Stub
    {
        // Creates a new stub instance for the given object to access
        // DBus functionality.
        Stub(const core::dbus::Object::Ptr& object) : object{object}
        {
        }

        // Creates a new stub instance for the given bus connection
        Stub(const core::dbus::Bus::Ptr& bus)
            : object
              {
                  core::dbus::Service::use_service<DBus>(bus)
                      ->object_for_path(core::dbus::types::ObjectPath{"/org/freedesktop/DBus"})
              }
        {
        }

        // Gets the AppArmor confinement string associated with the unique connection name. If
        // D-Bus is not performing AppArmor mediation, the
        // org.freedesktop.DBus.Error.AppArmorSecurityContextUnknown error is returned.
        //
        // Invokes the given handler on completion.
        void get_connection_app_armor_security_async(const std::string& name, std::function<void(const biometry::Optional<std::string>&)> handler)
        {
            object->invoke_method_asynchronously_with_callback<GetConnectionAppArmorSecurityContext, std::string>([handler](const core::dbus::Result<std::string>& result)
            {
                biometry::Optional<std::string> label; handler((not result.is_error() ?
                                                                        label = result.value() :
                                                                        is_running_in_a_testing_environment() ? label = "unconfined" : label));
            }, name);
        }

        // Gets the unix user id of the sender identified by name, invoking handler on completion.
        void get_connection_unix_user_async(const std::string& name, const std::function<void(biometry::Optional<std::uint32_t>)>& handler)
        {
            object->invoke_method_asynchronously_with_callback<GetConnectionUnixUser, std::uint32_t>([handler](const core::dbus::Result<std::uint32_t>& result)
            {
                biometry::Optional<std::uint32_t> uid; handler((not result.is_error() ? uid = result.value() : uid));
            }, name);
        }

        core::dbus::Object::Ptr object;
    };
};
}

biometry::dbus::skeleton::DaemonCredentialsResolver::DaemonCredentialsResolver(const core::dbus::Bus::Ptr& bus) : bus{bus}
{
}

void biometry::dbus::skeleton::DaemonCredentialsResolver::resolve_credentials(
        const core::dbus::Message::Ptr& msg,
        const std::function<void(const Optional<RequestVerifier::Credentials>&)>& then)
{
    DBus::Stub stub{bus};
    stub.get_connection_unix_user_async(msg->sender(), [stub, msg, then](biometry::Optional<std::uint32_t> uid) mutable
    {
        stub.get_connection_app_armor_security_async(msg->sender(), [stub, msg, then, uid](const biometry::Optional<std::string>& label) mutable
        {
            Optional<RequestVerifier::Credentials> credentials;
            RequestVerifier::Credentials cred = {biometry::Application{label.get()}, biometry::User{uid.get()}};
            then((label && uid ? credentials = cred : credentials));
        });
    });
}
