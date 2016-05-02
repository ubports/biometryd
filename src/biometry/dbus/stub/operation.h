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

#ifndef BIOMETRYD_DBUS_STUB_OPERATION_H_
#define BIOMETRYD_DBUS_STUB_OPERATION_H_

#include <biometry/operation.h>
#include <biometry/tracing_operation_observer.h>

#include <biometry/dbus/interface.h>
#include <biometry/dbus/skeleton/observer.h>

#include <core/dbus/object.h>
#include <core/dbus/service.h>

#include <boost/format.hpp>

namespace biometry
{
namespace dbus
{
namespace stub
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

    /// @brief create_for_object_and_service returns a new instance on the given object.
    static Ptr create_for_object_and_service(const core::dbus::Bus::Ptr& bus, const core::dbus::Service::Ptr& service, const core::dbus::Object::Ptr& object);

    /// @brief Frees up resources and uninstall message handlers.
    ~Operation();

    // From biometry::Operation<T>
    void start_with_observer(const typename Observer::Ptr& observer) override;
    void cancel() override;

private:
    /// @brief Operation creates a new instance for the given remote service and object.
    Operation(const core::dbus::Bus::Ptr& bus, const core::dbus::Service::Ptr& service, const core::dbus::Object::Ptr& object);

    core::dbus::Bus::Ptr bus;
    core::dbus::Service::Ptr service;
    core::dbus::Object::Ptr object;
};
}
}
}

template<typename T>
typename biometry::dbus::stub::Operation<T>::Ptr biometry::dbus::stub::Operation<T>::create_for_object_and_service(
        const core::dbus::Bus::Ptr& bus,
        const core::dbus::Service::Ptr& service,
        const core::dbus::Object::Ptr& object)
{
    return Ptr{new Operation<T>{bus, service, object}};
}

template<typename T>
biometry::dbus::stub::Operation<T>::~Operation()
{
}

// From biometry::Operation<T>
template<typename T>
void biometry::dbus::stub::Operation<T>::start_with_observer(const typename Observer::Ptr& observer)
{
    auto path = core::dbus::types::ObjectPath
    {
        (boost::format("%1%/observer") % object->path().as_string()).str()
    };

    auto obs = biometry::dbus::skeleton::Observer<T>::create_for_object(bus, service->add_object_for_path(path), observer);

    auto result = object->transact_method<
            biometry::dbus::interface::Operation::Methods::StartWithObserver,
            biometry::dbus::interface::Operation::Methods::StartWithObserver::ResultType
    >(path);

    if (result.is_error())
        throw std::runtime_error{result.error().print()};


}

template<typename T>
void biometry::dbus::stub::Operation<T>::cancel()
{
    auto result = object->transact_method<
            biometry::dbus::interface::Operation::Methods::Cancel,
            biometry::dbus::interface::Operation::Methods::Cancel::ResultType
    >();
}

template<typename T>
biometry::dbus::stub::Operation<T>::Operation(
        const core::dbus::Bus::Ptr& bus,
        const core::dbus::Service::Ptr& service,
        const core::dbus::Object::Ptr& object)
    : bus{bus},
      service{service},
      object{object}
{
}

#endif // BIOMETRYD_DBUS_STUB_OPERATION_H_
