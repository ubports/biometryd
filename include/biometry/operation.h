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

#ifndef BIOMETRY_OPERATION_H_
#define BIOMETRY_OPERATION_H_

#include <biometry/do_not_copy_or_move.h>
#include <biometry/visibility.h>

#include <memory>

namespace biometry
{
/// @brief An Operation models an asynchronous operation that can be started and cancelled,
/// as well as observed.
template<typename T>
class BIOMETRY_DLL_PUBLIC Operation : private DoNotCopyOrMove
{
public:
    typedef std::shared_ptr<Operation<T>> Ptr;

    typedef typename T::Progress Progress;
    typedef typename T::Reason Reason;
    typedef typename T::Error Error;
    typedef typename T::Result Result;

    /// @brief An Observer enables client code to monitor an ongoing operation.
    class Observer : private DoNotCopyOrMove
    {
    public:
        // Safe us some typing.
        typedef std::shared_ptr<Observer> Ptr;

        typedef typename T::Progress Progress;
        typedef typename T::Reason Reason;
        typedef typename T::Error Error;
        typedef typename T::Result Result;

        /// @brief on_state_changed is called whenever the state of an operation changed,
        /// handing the current (new) and previous state to the observer.
        virtual void on_started() = 0;

        /// @brief on_progress is called whenever an operation advances.
        ///
        /// @param progress contains details describing the progress.
        virtual void on_progress(const Progress& progress) = 0;

        /// @brief on_canceled is called when an operation is cancelled.
        ///
        /// @param reason contains details explaing the reson for cancelling.
        virtual void on_canceled(const Reason& reason) = 0;

        /// @brief on_failed is called when an operation fails.
        ///
        /// @param error provides details describing the error condition.
        virtual void on_failed(const Error& error) = 0;

        /// @brief on_succeeded is called when an operation succeeds.
        ///
        /// @param result provides details handing the result of the operation to observers.
        virtual void on_succeeded(const Result& result) = 0;

    protected:
        /// @cond
        Observer() = default;
        /// @endcond
    };

    /// @brief start_with_observer starts the operation, handing updates to 'observer'.
    virtual void start_with_observer(const typename Observer::Ptr& observer) = 0;

    /// @brief cancel stops the operation, confirming cancellation to the installed observer.
    virtual void cancel() = 0;

protected:
    /// @cond
    Operation() = default;
    /// @endcond
};
}

#endif // BIOMETRY_OPERATION_H_
