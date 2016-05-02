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

#include <biometry/dbus/skeleton/device.h>

#include <biometry/dbus/interface.h>
#include <biometry/dbus/skeleton/template_store.h>

#include <boost/format.hpp>

namespace
{
struct PrefixedPath
{
    PrefixedPath(const std::string& suffix) : suffix{suffix}
    {
    }

    core::dbus::types::ObjectPath prefix(const core::dbus::types::ObjectPath& path) const
    {
        return core::dbus::types::ObjectPath
        {
            (boost::format{"%1%/%2%"} % path.as_string() % suffix).str()
        };
    }

    std::string suffix;
};
}

/// @brief create_for_bus returns a new skeleton::Device instance connected to bus, forwarding calls to impl.
biometry::dbus::skeleton::Device::Ptr biometry::dbus::skeleton::Device::create_for_service_and_object(
        const core::dbus::Bus::Ptr& bus,
        const core::dbus::Service::Ptr& service,
        const core::dbus::Object::Ptr& object,
        const std::shared_ptr<biometry::Device>& impl)
{
    return Ptr{new Device{bus, service, object, impl}};
}

/// @brief Frees up resources and removes routes to message handlers.
biometry::dbus::skeleton::Device::~Device()
{
    object_->uninstall_method_handler<biometry::dbus::interface::Device::Methods::TemplateStore>();
    object_->uninstall_method_handler<biometry::dbus::interface::Device::Methods::Identifier>();
}

// From biometry::Device
biometry::TemplateStore& biometry::dbus::skeleton::Device::template_store()
{
    return impl_->template_store();
}

biometry::Identifier& biometry::dbus::skeleton::Device::identifier()
{
    return impl_->identifier();
}

biometry::Verifier& biometry::dbus::skeleton::Device::verifier()
{
    return impl_->verifier();
}

biometry::dbus::skeleton::Device::Device(
        const core::dbus::Bus::Ptr& bus,
        const core::dbus::Service::Ptr& service,
        const core::dbus::Object::Ptr& object,
        const std::shared_ptr<biometry::Device>& impl)
    : impl_{impl},
      bus_{bus},
      service_{service},
      object_{object}
{
    object_->install_method_handler<biometry::dbus::interface::Device::Methods::TemplateStore>([this](const core::dbus::Message::Ptr& msg)
    {
        auto path = PrefixedPath{"template_store"}.prefix(object_->path());

        template_store_([this, &path]()
        {
            return TemplateStore::create_for_service_and_object(bus_, service_, service_->add_object_for_path(path), std::ref(template_store()));
        });

        auto reply = core::dbus::Message::make_method_return(msg);
        reply->writer() << path;
        this->bus_->send(reply);
    });

    object_->install_method_handler<biometry::dbus::interface::Device::Methods::Identifier>([this](const core::dbus::Message::Ptr& msg)
    {
        auto path = PrefixedPath{"identifier"}.prefix(object_->path());

        identifier_([this, &path]()
        {
            return Identifier::create_for_service_and_object(bus_, service_, service_->add_object_for_path(path), std::ref(identifier()));
        });

        auto reply = core::dbus::Message::make_method_return(msg);
        reply->writer() << path;
        this->bus_->send(reply);
    });
}
