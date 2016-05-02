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

#ifndef BIOMETRYD_DBUS_STUB_OBSERVER_H_
#define BIOMETRYD_DBUS_STUB_OBSERVER_H_

#include <biometry/operation.h>

#include <biometry/dbus/interface.h>

#include <core/dbus/object.h>

namespace biometry
{
namespace dbus
{
namespace stub
{
template<typename T>
class Observer : public Operation<T>::Observer
{
public:
    // Safe us some typing
    typedef std::shared_ptr<Observer<T>> Ptr;
    typedef typename biometry::Operation<T>::Observer Super;

    using typename Super::Progress;
    using typename Super::Reason;
    using typename Super::Error;
    using typename Super::Result;

    /// @brief create_for_object_and_service returns a new instance on the given object.
    static Ptr create_for_object(const core::dbus::Object::Ptr& object);

    // From Operation<T>::Observer
    void on_started() override;
    void on_progress(const Progress&) override;
    void on_canceled(const Reason&) override;
    void on_failed(const Error&) override;
    void on_succeeded(const Result&) override;

private:
    /// @brief Observer initializes a new instance for the given remote object.
    Observer(const core::dbus::Object::Ptr& object);

    core::dbus::Object::Ptr object;
};
}
}
}

template<typename T>
typename biometry::dbus::stub::Observer<T>::Ptr biometry::dbus::stub::Observer<T>::create_for_object(const core::dbus::Object::Ptr& object)
{
    return Ptr{new Observer<T>{object}};
}

template<typename T>
void biometry::dbus::stub::Observer<T>::on_started()
{
    object->invoke_method_asynchronously_with_callback<
            biometry::dbus::interface::Operation::Observer::Methods::OnStarted,
            void
    >([](const core::dbus::Result<void>&)
    {
    });
}

template<typename T>
void biometry::dbus::stub::Observer<T>::on_progress(const typename Observer<T>::Progress& progress)
{
    object->invoke_method_asynchronously_with_callback<
            biometry::dbus::interface::Operation::Observer::Methods::OnProgress,
            void
    >([](const core::dbus::Result<void>&)
    {
    }, progress);
}

template<typename T>
void biometry::dbus::stub::Observer<T>::on_canceled(const Reason& reason)
{
    object->invoke_method_asynchronously_with_callback<
            biometry::dbus::interface::Operation::Observer::Methods::OnCancelled,
            void
    >([](const core::dbus::Result<void>&)
    {
    }, reason);
}

template<typename T>
void biometry::dbus::stub::Observer<T>::on_failed(const Error& error)
{
    object->invoke_method_asynchronously_with_callback<
            biometry::dbus::interface::Operation::Observer::Methods::OnFailed,
            void
    >([](const core::dbus::Result<void>&)
    {
    }, error);
}

template<typename T>
void biometry::dbus::stub::Observer<T>::on_succeeded(const Result& result)
{
    object->invoke_method_asynchronously_with_callback<
            biometry::dbus::interface::Operation::Observer::Methods::OnSucceeded,
            void
    >([](const core::dbus::Result<void>&)
    {
    }, result);
}

template<typename T>
biometry::dbus::stub::Observer<T>::Observer(const core::dbus::Object::Ptr& object)
    : object{object}
{
}

#endif // BIOMETRYD_DBUS_STUB_OBSERVER_H_
