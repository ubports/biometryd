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

#include <biometry/dbus/service.h>
#include <biometry/dbus/stub/service.h>

#include <biometry/runtime.h>

#include <core/dbus/bus.h>
#include <core/dbus/asio/executor.h>

std::shared_ptr<biometry::Service> biometry::dbus::Service::create_stub()
{    
    auto bus = std::make_shared<core::dbus::Bus>(core::dbus::WellKnownBus::system);
    bus->install_executor(core::dbus::asio::make_executor(bus));

    return biometry::dbus::stub::Service::create_for_bus(bus);
}
