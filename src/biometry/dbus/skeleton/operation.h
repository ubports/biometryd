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

#ifndef BIOMETRYD_DBUS_SKELETON_OPERATION_H_
#define BIOMETRYD_DBUS_SKELETON_OPERATION_H_

#include <biometry/operation.h>
#include <biometry/tracing_operation_observer.h>

#include <biometry/dbus/interface.h>

#include <biometry/dbus/stub/observer.h>

#include <core/dbus/object.h>
#include <core/dbus/service.h>

namespace biometry
{
namespace dbus
{
namespace skeleton
{
// Identifier is the dbus SKELETON implementation of biometry::Service.
template<typename T>
class Operation : public biometry::Operation<T>
{
public:
    // Safe us some typing
    typedef std::shared_ptr<Operation<T>> Ptr;
    typedef biometry::Operation<T> Super;

    using typename Super::Observer;
    using typename Super::Progress;
    using typename Super::Reason;
    using typename Super::Error;
    using typename Super::Result;

    /// @brief create_for_object returns a new instance on the given object.
    static Ptr create_for_object(const core::dbus::Bus::Ptr& bus, const core::dbus::Object::Ptr& object, const typename biometry::Operation<T>::Ptr& impl);

    /// @brief Frees up resources and uninstall message handlers.
    ~Operation();

    // From biometry::Operation<T>
    void start_with_observer(const typename Observer::Ptr& observer) override;
    void cancel() override;

private:
    /// @brief Service creates a new instance for the given remote service and object.
    Operation(const core::dbus::Bus::Ptr& bus, const core::dbus::Object::Ptr& object, const typename biometry::Operation<T>::Ptr& impl);

    typename biometry::Operation<T>::Ptr impl;
    core::dbus::Bus::Ptr bus;
    core::dbus::Object::Ptr object;
};
}
}
}

template<typename T>
typename biometry::dbus::skeleton::Operation<T>::Ptr biometry::dbus::skeleton::Operation<T>::create_for_object(
        const core::dbus::Bus::Ptr& bus,
        const core::dbus::Object::Ptr& object,
        const typename biometry::Operation<T>::Ptr& impl)
{
    return Ptr{new Operation<T>{bus, object, impl}};
}

template<typename T>
biometry::dbus::skeleton::Operation<T>::~Operation()
{
    object->uninstall_method_handler<biometry::dbus::interface::Operation::Methods::StartWithObserver>();
    object->uninstall_method_handler<biometry::dbus::interface::Operation::Methods::Cancel>();
}

template<typename T>
void biometry::dbus::skeleton::Operation<T>::start_with_observer(const typename Observer::Ptr& observer)
{
    impl->start_with_observer(observer);
}

template<typename T>
void biometry::dbus::skeleton::Operation<T>::cancel()
{
    impl->cancel();
}

template<typename T>
biometry::dbus::skeleton::Operation<T>::Operation(
        const core::dbus::Bus::Ptr& bus,
        const core::dbus::Object::Ptr& object,
        const typename biometry::Operation<T>::Ptr& impl)
    : impl{impl},
      bus{bus},
      object{object}
{
    object->install_method_handler<biometry::dbus::interface::Operation::Methods::StartWithObserver>([this](const core::dbus::Message::Ptr& msg)
    {
        core::dbus::types::ObjectPath path; msg->reader() >> path;
        auto object = core::dbus::Service::use_service(this->bus, msg->sender())->object_for_path(path);
        start_with_observer(biometry::dbus::stub::Observer<T>::create_for_object(object));

        this->bus->send(core::dbus::Message::make_method_return(msg));
    });

    object->install_method_handler<biometry::dbus::interface::Operation::Methods::Cancel>([this](const core::dbus::Message::Ptr& msg)
    {
        cancel();
        this->bus->send(core::dbus::Message::make_method_return(msg));
    });
}

#endif // BIOMETRYD_DBUS_SKELETON_OPERATION_H_
