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


#ifndef TESTING_ECHO_SERVICE_H_
#define TESTING_ECHO_SERVICE_H_

#include <biometry/dbus/codec.h>

#include <core/dbus/fixture.h>
#include <core/dbus/macros.h>
#include <core/dbus/message.h>
#include <core/dbus/object.h>
#include <core/dbus/traits/service.h>

namespace testing
{
struct EchoService
{
    static const std::string& name()
    {
        static const std::string s{"just.an.echo.Service"};
        return s;
    }

    static core::dbus::types::ObjectPath path()
    {
        return core::dbus::types::ObjectPath{"/"};
    }

    struct Methods
    {
        DBUS_CPP_METHOD_DEF(Application, EchoService)
        DBUS_CPP_METHOD_DEF(Progress, EchoService)
        DBUS_CPP_METHOD_DEF(Point, EchoService)
        DBUS_CPP_METHOD_DEF(Rectangle, EchoService)
        DBUS_CPP_METHOD_DEF(Reason, EchoService)
        DBUS_CPP_METHOD_DEF(User, EchoService)
        DBUS_CPP_METHOD_DEF(None, EchoService)
        DBUS_CPP_METHOD_DEF(Variant, EchoService)
        DBUS_CPP_METHOD_DEF(Void, EchoService)
    };

    virtual ~EchoService() = default;

    virtual void application(const biometry::Application&) = 0;
    virtual void progress(const biometry::Progress&) = 0;
    virtual void point(const biometry::Point&) = 0;
    virtual void rectangle(const biometry::Rectangle&) = 0;
    virtual void reason(const biometry::Reason&) = 0;
    virtual void user(const biometry::User&) = 0;
    virtual void none(const biometry::Variant::None&) = 0;
    virtual void variant(const biometry::Variant&) = 0;
    virtual void void_(const biometry::Void&) = 0;
};
}

namespace core { namespace dbus { namespace traits {
template<> struct Service< ::testing::EchoService >
{
    static inline const std::string& interface_name()
    {
        return ::testing::EchoService::name();
    }
};
}}}

namespace testing
{
class SkeletonEchoService : public testing::EchoService
{
public:
    SkeletonEchoService(const core::dbus::Bus::Ptr& bus, const core::dbus::Object::Ptr& object, const std::shared_ptr<EchoService>& impl)
        : bus{bus},
          object{object},
          impl{impl}
    {
        object->install_method_handler<Methods::Application>([this](const core::dbus::Message::Ptr& msg)
        {
            auto value = biometry::Application::system();
            msg->reader() >> value;
            application(value);
            auto reply = core::dbus::Message::make_method_return(msg);
            reply->writer() << value;
            this->bus->send(reply);
        });

        object->install_method_handler<Methods::Progress>([this](const core::dbus::Message::Ptr& msg)
        {
            auto value = biometry::Progress::none();
            msg->reader() >> value;
            progress(value);
            auto reply = core::dbus::Message::make_method_return(msg);
            reply->writer() << value;
            this->bus->send(reply);
        });

        object->install_method_handler<Methods::Point>([this](const core::dbus::Message::Ptr& msg)
        {
            biometry::Point value;
            msg->reader() >> value;
            point(value);
            auto reply = core::dbus::Message::make_method_return(msg);
            reply->writer() << value;
            this->bus->send(reply);
        });

        object->install_method_handler<Methods::Rectangle>([this](const core::dbus::Message::Ptr& msg)
        {
            biometry::Rectangle value;
            msg->reader() >> value;
            rectangle(value);
            auto reply = core::dbus::Message::make_method_return(msg);
            reply->writer() << value;
            this->bus->send(reply);
        });

        object->install_method_handler<Methods::Reason>([this](const core::dbus::Message::Ptr& msg)
        {
            auto value = biometry::Reason::unknown();
            msg->reader() >> value;
            reason(value);
            auto reply = core::dbus::Message::make_method_return(msg);
            reply->writer() << value;
            this->bus->send(reply);
        });

        object->install_method_handler<Methods::User>([this](const core::dbus::Message::Ptr& msg)
        {
            auto value = biometry::User::current();
            msg->reader() >> value;
            user(value);
            auto reply = core::dbus::Message::make_method_return(msg);
            reply->writer() << value;
            this->bus->send(reply);
        });

        object->install_method_handler<Methods::None>([this](const core::dbus::Message::Ptr& msg)
        {
            biometry::Variant::None value;
            msg->reader() >> value;
            none(value);
            auto reply = core::dbus::Message::make_method_return(msg);
            reply->writer() << value;
            this->bus->send(reply);
        });

        object->install_method_handler<Methods::Variant>([this](const core::dbus::Message::Ptr& msg)
        {
            biometry::Variant value;
            msg->reader() >> value;
            variant(value);
            auto reply = core::dbus::Message::make_method_return(msg);
            reply->writer() << value;
            this->bus->send(reply);
        });

        object->install_method_handler<Methods::Void>([this](const core::dbus::Message::Ptr& msg)
        {
            biometry::Void value;
            msg->reader() >> value;
            void_(value);
            auto reply = core::dbus::Message::make_method_return(msg);
            reply->writer() << value;
            this->bus->send(reply);
        });
    }

    void application(const biometry::Application& value)
    {
        impl->application(value);
    }

    void progress(const biometry::Progress& value)
    {
        impl->progress(value);
    }

    void point(const biometry::Point& value)
    {
        impl->point(value);
    }

    void rectangle(const biometry::Rectangle& value)
    {
        impl->rectangle(value);
    }

    void reason(const biometry::Reason& value)
    {
        impl->reason(value);
    }

    void user(const biometry::User& value)
    {
        impl->user(value);
    }

    void none(const biometry::Variant::None& value)
    {
        impl->none(value);
    }

    void variant(const biometry::Variant& value)
    {
        impl->variant(value);
    }

    void void_(const biometry::Void& value)
    {
        impl->void_(value);
    }

private:
    core::dbus::Bus::Ptr bus;
    core::dbus::Object::Ptr object;
    std::shared_ptr<EchoService> impl;
};

class StubEchoService : public testing::EchoService
{
public:
    StubEchoService(const core::dbus::Object::Ptr& object) : object{object}
    {
    }

    void application(const biometry::Application& value)
    {
        auto result = object->transact_method<EchoService::Methods::Application, biometry::Application>(value);
        if (not(value == result.value()))
            throw std::logic_error{"mismatch"};
    }

    void progress(const biometry::Progress& value)
    {
        auto result = object->transact_method<EchoService::Methods::Progress, biometry::Progress>(value);
        if (not(value == result.value()))
            throw std::logic_error{"mismatch"};
    }

    void point(const biometry::Point& value)
    {
        auto result = object->transact_method<EchoService::Methods::Point, biometry::Point>(value);
        if (not(value == result.value()))
            throw std::logic_error{"mismatch"};
    }

    void rectangle(const biometry::Rectangle& value)
    {
        auto result = object->transact_method<EchoService::Methods::Rectangle, biometry::Rectangle>(value);
        if (value != result.value())
            throw std::logic_error{"mismatch"};
    }

    void reason(const biometry::Reason& value)
    {
        auto result = object->transact_method<EchoService::Methods::Reason, biometry::Reason>(value);
        if (!(value == result.value()))
            throw std::logic_error{"mismatch"};
    }

    void user(const biometry::User& value)
    {
        auto result = object->transact_method<EchoService::Methods::User, biometry::User>(value);
        if (!(value == result.value()))
            throw std::logic_error{"mismatch"};
    }

    void none(const biometry::Variant::None& value)
    {
        auto result = object->transact_method<EchoService::Methods::None, biometry::Variant::None>(value);
        if (!(value == result.value()))
            throw std::logic_error{"mismatch"};
    }

    void variant(const biometry::Variant& value)
    {
        auto msg = core::dbus::Message::make_method_call(EchoService::name(), EchoService::path(), EchoService::name(), "Variant");
        auto result = object->transact_method<EchoService::Methods::Variant, biometry::Variant>(value);
        if (!(value == result.value()))
            throw std::logic_error{"mismatch"};
    }

    void void_(const biometry::Void& value)
    {
        auto result = object->transact_method<EchoService::Methods::Void, biometry::Void>(value);
        if (!(value == result.value()))
            throw std::logic_error{"mismatch"};
    }

private:
    core::dbus::Object::Ptr object;
};
}

#endif // TESTING_ECHO_SERVICE_H_
