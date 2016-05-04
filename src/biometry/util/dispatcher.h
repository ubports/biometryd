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

#ifndef BIOMETRY_UTIL_DISPATCHER_H_
#define BIOMETRY_UTIL_DISPATCHER_H_

#include <biometry/do_not_copy_or_move.h>
#include <biometry/runtime.h>
#include <biometry/visibility.h>

#include <memory>

namespace biometry
{
namespace util
{
/// @brief A Dispatcher enqueues tasks for execution.
class BIOMETRY_DLL_PUBLIC Dispatcher : public DoNotCopyOrMove
{
public:
    /// @brief A Task is dispatched by a dispatcher.
    typedef std::function<void()> Task;

    /// @brief dispatch enqueues the given task for execution.
    virtual void dispatch(const Task& task) = 0;

protected:
    /// @cond
    Dispatcher() = default;
    /// @endcond
};

/// @brief create_dispatcher_for_runtime creates a dispatcher enqueuing to the runtime's service.
BIOMETRY_DLL_PUBLIC std::shared_ptr<Dispatcher> create_dispatcher_for_runtime(const std::shared_ptr<Runtime>&);
}
}

#endif // BIOMETRY_UTIL_DISPATCHER_H_
