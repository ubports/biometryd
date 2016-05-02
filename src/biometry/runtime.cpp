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
#include <biometry/runtime.h>

#include <iostream>

namespace
{
// exception_safe_run runs service, catching all exceptions and
// restarting operation until an explicit shutdown has been requested.
//
// TODO(tvoss): Catching all exceptions is risky as they might signal unrecoverable
// errors. We should enable calling code to decide whether an exception should be considered
// fatal or not.
void exception_safe_run(boost::asio::io_service& service)
{
    while (true)
    {
        try
        {
            service.run();
            // a clean return from run only happens in case of
            // stop() being called (we are keeping the service alive with
            // a service::work instance).
            break;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what();
        }
        catch (...)
        {
            std::cerr << "Unknown exception caught while executing boost::asio::io_service";
        }
    }
}
}

std::shared_ptr<biometry::Runtime> biometry::Runtime::create(std::uint32_t pool_size)
{
    return std::shared_ptr<biometry::Runtime>(new biometry::Runtime(pool_size));
}

biometry::Runtime::Runtime(std::uint32_t pool_size)
    : pool_size_{pool_size},
      service_{pool_size_},
      strand_{service_},
      keep_alive_{service_}
{
}

biometry::Runtime::~Runtime()
{
    try
    {
        stop();
    } catch(...)
    {
        // Dropping all exceptions to satisfy the nothrow guarantee.
    }
}

void biometry::Runtime::start()
{
    for (unsigned int i = 0; i < pool_size_; i++)
        workers_.push_back(std::thread{exception_safe_run, std::ref(service_)});
}

void biometry::Runtime::stop()
{
    service_.stop();

    for (auto& worker : workers_)
        if (worker.joinable())
            worker.join();
}

std::function<void(std::function<void()>)> biometry::Runtime::to_dispatcher_functional()
{
    // We have to make sure that we stay alive for as long as
    // calling code requires the dispatcher to work.
    auto sp = shared_from_this();
    return [sp](std::function<void()> task)
    {
        sp->strand_.post(task);
    };
}

boost::asio::io_service& biometry::Runtime::service()
{
    return service_;
}
