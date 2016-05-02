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

#include <biometry/dbus/skeleton/template_store.h>

#include <biometry/application.h>
#include <biometry/user.h>
#include <biometry/dbus/codec.h>
#include <biometry/dbus/interface.h>
#include <biometry/dbus/skeleton/operation.h>

#include <util/atomic_counter.h>
#include <util/synchronized.h>

#include <boost/format.hpp>

biometry::dbus::skeleton::TemplateStore::Ptr biometry::dbus::skeleton::TemplateStore::create_for_service_and_object(
        const core::dbus::Bus::Ptr& bus,
        const core::dbus::Service::Ptr& service,
        const core::dbus::Object::Ptr& object,
        const std::reference_wrapper<biometry::TemplateStore>& impl)
{
    return Ptr{new TemplateStore{bus, service, object, impl}};
}

biometry::Operation<biometry::TemplateStore::SizeQuery>::Ptr biometry::dbus::skeleton::TemplateStore::size(const biometry::Application& app, const biometry::User& user)
{
    return impl.get().size(app, user);
}

biometry::Operation<biometry::TemplateStore::Enrollment>::Ptr biometry::dbus::skeleton::TemplateStore::enroll(const biometry::Application& app, const biometry::User& user)
{
    return impl.get().enroll(app, user);
}

biometry::Operation<biometry::TemplateStore::Clearance>::Ptr biometry::dbus::skeleton::TemplateStore::clear(const biometry::Application& app, const biometry::User& user)
{
    return impl.get().clear(app, user);
}

biometry::dbus::skeleton::TemplateStore::TemplateStore(
        const core::dbus::Bus::Ptr& bus,
        const core::dbus::Service::Ptr& service,
        const core::dbus::Object::Ptr& object,
        const std::reference_wrapper<biometry::TemplateStore>& impl)
    : impl{impl},
      bus{bus},
      service{service},
      object{object}
{
    object->install_method_handler<biometry::dbus::interface::TemplateStore::Methods::Size>([this](const core::dbus::Message::Ptr& msg)
    {
        biometry::User user; biometry::Application app = biometry::Application::system();
        auto reader = msg->reader(); reader >> app >> user;
        auto op = size(app, user);

        core::dbus::types::ObjectPath op_path
        {
            (boost::format{"%1%/operation/size/%2%"} % this->object->path().as_string() % util::counter<TemplateStore>().increment()).str()
        };

        ops.size.synchronized([this, op_path, op](SizeOps::ValueType& ops)
        {
            ops[op_path] = skeleton::Operation<SizeQuery>::create_for_object(this->bus, this->service->add_object_for_path(op_path), op);
        });

        auto reply = core::dbus::Message::make_method_return(msg);
        reply->writer() << op_path;
        this->bus->send(reply);
    });

    object->install_method_handler<biometry::dbus::interface::TemplateStore::Methods::Enroll>([this](const core::dbus::Message::Ptr& msg)
    {
        biometry::User user; biometry::Application app = biometry::Application::system();
        auto reader = msg->reader(); reader >> app >> user;
        auto op = enroll(app, user);

        core::dbus::types::ObjectPath op_path
        {
            (boost::format{"%1%/operation/enroll/%2%"} % this->object->path().as_string() % util::counter<TemplateStore>().increment()).str()
        };

        ops.enroll.synchronized([this, op_path, op](EnrollOps::ValueType& ops)
        {
            ops[op_path] = skeleton::Operation<Enrollment>::create_for_object(this->bus, this->service->add_object_for_path(op_path), op);
        });

        auto reply = core::dbus::Message::make_method_return(msg);
        reply->writer() << op_path;
        this->bus->send(reply);
    });

    object->install_method_handler<biometry::dbus::interface::TemplateStore::Methods::Clear>([this](const core::dbus::Message::Ptr& msg)
    {
        biometry::User user; biometry::Application app = biometry::Application::system();
        auto reader = msg->reader(); reader >> app >> user;
        auto op = clear(app, user);

        core::dbus::types::ObjectPath op_path
        {
            (boost::format{"%1%/operation/clear/%2%"} % this->object->path().as_string() % util::counter<TemplateStore>().increment()).str()
        };

        ops.clear.synchronized([this, op_path, op](ClearOps::ValueType& ops)
        {
            ops[op_path] = skeleton::Operation<Clearance>::create_for_object(this->bus, this->service->add_object_for_path(op_path), op);
        });

        auto reply = core::dbus::Message::make_method_return(msg);
        reply->writer() << op_path;
        this->bus->send(reply);
    });
}

biometry::dbus::skeleton::TemplateStore::~TemplateStore()
{
    object->uninstall_method_handler<biometry::dbus::interface::TemplateStore::Methods::Size>();
    object->uninstall_method_handler<biometry::dbus::interface::TemplateStore::Methods::Enroll>();
    object->uninstall_method_handler<biometry::dbus::interface::TemplateStore::Methods::Clear>();
}
