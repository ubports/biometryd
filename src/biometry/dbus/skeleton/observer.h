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

#ifndef BIOMETRYD_DBUS_SKELETON_OBSERVER_H_
#define BIOMETRYD_DBUS_SKELETON_OBSERVER_H_

#include <biometry/operation.h>

#include <biometry/dbus/interface.h>

#include <core/dbus/object.h>

#include <memory>

namespace biometry
{
namespace dbus
{
namespace skeleton
{
template<typename T>
class Observer : public Operation<T>::Observer,
                 public std::enable_shared_from_this<Observer<T>>
{
public:
    // Safe us some typing
    typedef std::shared_ptr<Observer<T>> Ptr;
    typedef typename biometry::Operation<T>::Observer Super;

    using typename Super::Progress;
    using typename Super::Reason;
    using typename Super::Error;
    using typename Super::Result;

    /// @brief create_for_object returns a new instance on the given object.
    static Ptr create_for_object(const core::dbus::Bus::Ptr& bus, const core::dbus::Object::Ptr& object, const typename Operation<T>::Observer::Ptr& impl);

    // From Operation<T>::Observer
    void on_started() override;
    void on_progress(const Progress&) override;
    void on_canceled(const Reason&) override;
    void on_failed(const Error&) override;
    void on_succeeded(const Result&) override;

private:
    /// @brief Observer initializes a new instance for the given remote object.
    Observer(const core::dbus::Bus::Ptr& bus, const core::dbus::Object::Ptr& object, const typename Operation<T>::Observer::Ptr& impl);
    /// @brief Finalize construction sets up message handlers.
    Ptr finalize_construction();

    /// @brief uninstall_method_handlers removes installed method handlers, preparing destruction of the object.
    void uninstall_method_handlers();

    typename Operation<T>::Observer::Ptr impl;

    core::dbus::Bus::Ptr bus;
    core::dbus::Object::Ptr object;
};
}
}
}

template<typename T>
typename biometry::dbus::skeleton::Observer<T>::Ptr biometry::dbus::skeleton::Observer<T>::create_for_object(
        const core::dbus::Bus::Ptr& bus,
        const core::dbus::Object::Ptr& object,
        const typename Operation<T>::Observer::Ptr& impl)
{
    return Ptr{new Observer<T>{bus, object, impl}}->finalize_construction();
}

template<typename T>
void biometry::dbus::skeleton::Observer<T>::on_started()
{
    impl->on_started();
}

template<typename T>
void biometry::dbus::skeleton::Observer<T>::on_progress(const typename Observer<T>::Progress& progress)
{
    impl->on_progress(progress);
}

template<typename T>
void biometry::dbus::skeleton::Observer<T>::on_canceled(const Reason& reason)
{
    impl->on_canceled(reason);
}

template<typename T>
void biometry::dbus::skeleton::Observer<T>::on_failed(const Error& error)
{
    impl->on_failed(error);
}

template<typename T>
void biometry::dbus::skeleton::Observer<T>::on_succeeded(const Result& result)
{
    impl->on_succeeded(result);
}

template<typename T>
biometry::dbus::skeleton::Observer<T>::Observer(
        const core::dbus::Bus::Ptr& bus,
        const core::dbus::Object::Ptr& object,
        const typename Operation<T>::Observer::Ptr& impl)
    : impl{impl},
      bus{bus},
      object{object}
{
}

template<typename T>
void biometry::dbus::skeleton::Observer<T>::uninstall_method_handlers()
{
    object->uninstall_method_handler<biometry::dbus::interface::Operation::Observer::Methods::OnStarted>();
    object->uninstall_method_handler<biometry::dbus::interface::Operation::Observer::Methods::OnProgress>();
    object->uninstall_method_handler<biometry::dbus::interface::Operation::Observer::Methods::OnCancelled>();
    object->uninstall_method_handler<biometry::dbus::interface::Operation::Observer::Methods::OnFailed>();
    object->uninstall_method_handler<biometry::dbus::interface::Operation::Observer::Methods::OnSucceeded>();
}

template<typename T>
typename biometry::dbus::skeleton::Observer<T>::Ptr biometry::dbus::skeleton::Observer<T>::finalize_construction()
{
    auto thiz = std::enable_shared_from_this<Observer<T>>::shared_from_this();

    object->install_method_handler<biometry::dbus::interface::Operation::Observer::Methods::OnStarted>([thiz, this](const core::dbus::Message::Ptr& msg)
    {
        on_started();
        bus->send(core::dbus::Message::make_method_return(msg));
    });

    object->install_method_handler<biometry::dbus::interface::Operation::Observer::Methods::OnProgress>([thiz, this](const core::dbus::Message::Ptr& msg)
    {
        auto progress = Progress::none(); msg->reader() >> progress;
        on_progress(progress);
        bus->send(core::dbus::Message::make_method_return(msg));
    });

    object->install_method_handler<biometry::dbus::interface::Operation::Observer::Methods::OnCancelled>([thiz, this](const core::dbus::Message::Ptr& msg)
    {
        Reason reason; msg->reader() >> reason;
        on_canceled(reason);
        bus->send(core::dbus::Message::make_method_return(msg));

        uninstall_method_handlers();
    });

    object->install_method_handler<biometry::dbus::interface::Operation::Observer::Methods::OnFailed>([thiz, this](const core::dbus::Message::Ptr& msg)
    {
        Error error; msg->reader() >> error;
        on_failed(error);
        bus->send(core::dbus::Message::make_method_return(msg));

        uninstall_method_handlers();
    });

    object->install_method_handler<biometry::dbus::interface::Operation::Observer::Methods::OnSucceeded>([thiz, this](const core::dbus::Message::Ptr& msg)
    {
        Result result; msg->reader() >> result;
        on_succeeded(result);
        bus->send(core::dbus::Message::make_method_return(msg));

        uninstall_method_handlers();
    });

    return thiz;
}

#endif // BIOMETRYD_DBUS_SKELETON_OBSERVER_H_
