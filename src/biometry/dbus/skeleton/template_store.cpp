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

#include <biometry/util/atomic_counter.h>
#include <biometry/util/synchronized.h>

#include <boost/format.hpp>

namespace
{
core::dbus::Message::Ptr not_permitted_in_reply_to(const core::dbus::Message::Ptr& msg)
{
    return core::dbus::Message::make_error(msg, biometry::dbus::interface::Errors::NotPermitted::name(), "");
}

bool verify(const biometry::dbus::skeleton::RequestVerifier::Credentials& requested, const biometry::dbus::skeleton::RequestVerifier::Credentials& provided)
{
    // In case of a match: good to go. Please note that we
    // will dispatch to the trust-store in the future, once we
    // open up the service to all applications.
    if (requested == provided)
        return true;

    // Unconfinded apps are allowed to request operations on behalf of other applications.
    // We still limit them to their effective uid.
    if (provided.app.as_string() == "unconfined" && requested.user == provided.user)
        return true;

    // All other cases are forbidden.
    return false;
}
}

bool biometry::dbus::skeleton::TemplateStore::RequestVerifier::verify_size_request(const Credentials& requested, const Credentials& provided)
{
    return verify(requested, provided);
}

bool biometry::dbus::skeleton::TemplateStore::RequestVerifier::verify_list_request(const Credentials& requested, const Credentials& provided)
{
    return verify(requested, provided);
}

bool biometry::dbus::skeleton::TemplateStore::RequestVerifier::verify_enroll_request(const Credentials& requested, const Credentials& provided)
{
    return verify(requested, provided);
}

bool biometry::dbus::skeleton::TemplateStore::RequestVerifier::verify_remove_request(const Credentials& requested, const Credentials& provided)
{
    return verify(requested, provided);
}

bool biometry::dbus::skeleton::TemplateStore::RequestVerifier::verify_clear_request(const Credentials& requested, const Credentials& provided)
{
    return verify(requested, provided);
}

biometry::dbus::skeleton::TemplateStore::Ptr biometry::dbus::skeleton::TemplateStore::create_for_service_and_object(
        const core::dbus::Bus::Ptr& bus,
        const core::dbus::Service::Ptr& service,
        const core::dbus::Object::Ptr& object,
        const std::reference_wrapper<biometry::TemplateStore>& impl,
        const std::shared_ptr<RequestVerifier>& request_verifier,
        const std::shared_ptr<CredentialsResolver>& credentials_resolver)
{
    return Ptr{new TemplateStore{bus, service, object, impl, request_verifier, credentials_resolver}};
}

biometry::Operation<biometry::TemplateStore::SizeQuery>::Ptr biometry::dbus::skeleton::TemplateStore::size(const biometry::Application& app, const biometry::User& user)
{
    return impl.get().size(app, user);
}

biometry::Operation<biometry::TemplateStore::List>::Ptr biometry::dbus::skeleton::TemplateStore::list(const biometry::Application& app, const biometry::User& user)
{
    return impl.get().list(app, user);
}

biometry::Operation<biometry::TemplateStore::Enrollment>::Ptr biometry::dbus::skeleton::TemplateStore::enroll(const biometry::Application& app, const biometry::User& user)
{
    return impl.get().enroll(app, user);
}

biometry::Operation<biometry::TemplateStore::Removal>::Ptr biometry::dbus::skeleton::TemplateStore::remove(const biometry::Application& app, const biometry::User& user, biometry::TemplateStore::TemplateId id)
{
    return impl.get().remove(app, user, id);
}

biometry::Operation<biometry::TemplateStore::Clearance>::Ptr biometry::dbus::skeleton::TemplateStore::clear(const biometry::Application& app, const biometry::User& user)
{
    return impl.get().clear(app, user);
}

biometry::dbus::skeleton::TemplateStore::TemplateStore(
        const core::dbus::Bus::Ptr& bus,
        const core::dbus::Service::Ptr& service,
        const core::dbus::Object::Ptr& object,
        const std::reference_wrapper<biometry::TemplateStore>& impl,
        const std::shared_ptr<RequestVerifier>& request_verifier,
        const std::shared_ptr<CredentialsResolver>& credentials_resolver)
    : impl{impl},
      request_verifier{request_verifier},
      credentials_resolver{credentials_resolver},
      bus{bus},
      service{service},
      object{object}
{
    object->install_method_handler<biometry::dbus::interface::TemplateStore::Methods::Size>([this](const core::dbus::Message::Ptr& msg)
    {
        TemplateStore::credentials_resolver->resolve_credentials(msg, [this, msg](const Optional<RequestVerifier::Credentials>& credentials)
        {
            if (not credentials)
            {
                this->bus->send(not_permitted_in_reply_to(msg));
                return;
            }

            biometry::User user; biometry::Application app = biometry::Application::system();
            auto reader = msg->reader(); reader >> app >> user;

            if (not this->request_verifier->verify_size_request({app, user}, credentials.get()))
            {
                this->bus->send(not_permitted_in_reply_to(msg));
                return;
            }

            auto op = size(app, user);

            core::dbus::types::ObjectPath op_path
            {
                (boost::format{"%1%/operation/size/%2%/%3%/%4%"}
                    % this->object->path().as_string()
                    % credentials.get().app.as_string()
                    % credentials.get().user.id
                    % util::counter<TemplateStore>().increment()).str()
            };

            ops.size.synchronized([this, op_path, op](SizeOps::ValueType& ops)
            {
                ops[op_path] = skeleton::Operation<SizeQuery>::create_for_object(this->bus, this->service->add_object_for_path(op_path), op);
            });

            auto reply = core::dbus::Message::make_method_return(msg);
            reply->writer() << op_path;
            this->bus->send(reply);
        });
    });

    object->install_method_handler<biometry::dbus::interface::TemplateStore::Methods::List>([this](const core::dbus::Message::Ptr& msg)
    {
        TemplateStore::credentials_resolver->resolve_credentials(msg, [this, msg](const Optional<RequestVerifier::Credentials>& credentials)
        {
            if (not credentials)
            {
                this->bus->send(not_permitted_in_reply_to(msg));
                return;
            }

            biometry::User user; biometry::Application app = biometry::Application::system();
            auto reader = msg->reader(); reader >> app >> user;

            if (not this->request_verifier->verify_list_request({app, user}, credentials.get()))
            {
                this->bus->send(not_permitted_in_reply_to(msg));
                return;
            }

            auto op = list(app, user);

            core::dbus::types::ObjectPath op_path
            {
                (boost::format{"%1%/operation/list/%2%/%3%/%4%"}
                    % this->object->path().as_string()
                    % credentials.get().app.as_string()
                    % credentials.get().user.id
                    % util::counter<TemplateStore>().increment()).str()
            };

            ops.list.synchronized([this, op_path, op](ListOps::ValueType& ops)
            {
                ops[op_path] = skeleton::Operation<List>::create_for_object(this->bus, this->service->add_object_for_path(op_path), op);
            });

            auto reply = core::dbus::Message::make_method_return(msg);
            reply->writer() << op_path;
            this->bus->send(reply);
        });
    });

    object->install_method_handler<biometry::dbus::interface::TemplateStore::Methods::Enroll>([this](const core::dbus::Message::Ptr& msg)
    {
        TemplateStore::credentials_resolver->resolve_credentials(msg, [this, msg](const Optional<RequestVerifier::Credentials>& credentials)
        {
            if (not credentials)
            {
                this->bus->send(not_permitted_in_reply_to(msg));
                return;
            }

            biometry::User user; biometry::Application app = biometry::Application::system();
            auto reader = msg->reader(); reader >> app >> user;

            if (not this->request_verifier->verify_enroll_request({app, user}, credentials.get()))
            {
                this->bus->send(not_permitted_in_reply_to(msg));
                return;
            }

            auto op = enroll(app, user);

            core::dbus::types::ObjectPath op_path
            {
                (boost::format{"%1%/operation/enroll/%2%/%3%/%4%"}
                    % this->object->path().as_string()
                    % credentials.get().app.as_string()
                    % credentials.get().user.id
                    % util::counter<TemplateStore>().increment()).str()
            };

            ops.enroll.synchronized([this, op_path, op](EnrollOps::ValueType& ops)
            {
                ops[op_path] = skeleton::Operation<Enrollment>::create_for_object(this->bus, this->service->add_object_for_path(op_path), op);
            });

            auto reply = core::dbus::Message::make_method_return(msg);
            reply->writer() << op_path;
            this->bus->send(reply);
        });
    });

    object->install_method_handler<biometry::dbus::interface::TemplateStore::Methods::Remove>([this](const core::dbus::Message::Ptr& msg)
    {
        TemplateStore::credentials_resolver->resolve_credentials(msg, [this, msg](const Optional<RequestVerifier::Credentials>& credentials)
        {
            if (not credentials)
            {
                this->bus->send(not_permitted_in_reply_to(msg));
                return;
            }

            biometry::User user; biometry::Application app = biometry::Application::system(); biometry::TemplateStore::TemplateId id{0};
            auto reader = msg->reader(); reader >> app >> user >> id;

            if (not this->request_verifier->verify_remove_request({app, user}, credentials.get()))
            {
                this->bus->send(not_permitted_in_reply_to(msg));
                return;
            }

            auto op = remove(app, user, id);

            core::dbus::types::ObjectPath op_path
            {
                (boost::format{"%1%/operation/remove/%2%/%3%/%4%"}
                    % this->object->path().as_string()
                    % credentials.get().app.as_string()
                    % credentials.get().user.id
                    % util::counter<TemplateStore>().increment()).str()
            };

            ops.remove.synchronized([this, op_path, op](RemoveOps::ValueType& ops)
            {
                ops[op_path] = skeleton::Operation<Removal>::create_for_object(this->bus, this->service->add_object_for_path(op_path), op);
            });

            auto reply = core::dbus::Message::make_method_return(msg);
            reply->writer() << op_path;
            this->bus->send(reply);
        });
    });

    object->install_method_handler<biometry::dbus::interface::TemplateStore::Methods::Clear>([this](const core::dbus::Message::Ptr& msg)
    {
        TemplateStore::credentials_resolver->resolve_credentials(msg, [this, msg](const Optional<RequestVerifier::Credentials>& credentials)
        {
            if (not credentials)
            {
                this->bus->send(not_permitted_in_reply_to(msg));
                return;
            }

            biometry::User user; biometry::Application app = biometry::Application::system();
            auto reader = msg->reader(); reader >> app >> user;

            if (not this->request_verifier->verify_clear_request({app, user}, credentials.get()))
            {
                this->bus->send(not_permitted_in_reply_to(msg));
                return;
            }

            auto op = clear(app, user);

            core::dbus::types::ObjectPath op_path
            {
                (boost::format{"%1%/operation/clear/%2%/%3%/%4%"}
                    % this->object->path().as_string()
                    % credentials.get().app.as_string()
                    % credentials.get().user.id
                    % util::counter<TemplateStore>().increment()).str()
            };

            ops.clear.synchronized([this, op_path, op](ClearOps::ValueType& ops)
            {
                ops[op_path] = skeleton::Operation<Clearance>::create_for_object(this->bus, this->service->add_object_for_path(op_path), op);
            });

            auto reply = core::dbus::Message::make_method_return(msg);
            reply->writer() << op_path;
            this->bus->send(reply);
        });
    });
}

biometry::dbus::skeleton::TemplateStore::~TemplateStore()
{
    object->uninstall_method_handler<biometry::dbus::interface::TemplateStore::Methods::Size>();
    object->uninstall_method_handler<biometry::dbus::interface::TemplateStore::Methods::Enroll>();
    object->uninstall_method_handler<biometry::dbus::interface::TemplateStore::Methods::Clear>();
}
