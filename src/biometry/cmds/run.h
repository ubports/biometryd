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

#ifndef BIOMETRYD_CMDS_RUN_H_
#define BIOMETRYD_CMDS_RUN_H_

#include <biometry/daemon.h>

#include <core/dbus/bus.h>

#include <functional>
#include <iostream>
#include <memory>

namespace biometry
{
namespace cmds
{
class Run : public biometry::Daemon::Command
{
public:
    /// @brief BusFactory models creation of bus instances.
    typedef std::function<core::dbus::Bus::Ptr()> BusFactory;

    /// @brief system_bus_factory returns a BusFactory creating connections to the system bus.
    static BusFactory system_bus_factory();

    Run(const BusFactory& bus_factory = system_bus_factory());

    Info info() const override;
    int run() override;

private:
    BusFactory bus_factory;
    TypedFlag<std::string>::Ptr config;
};
}
}

#endif // BIOMETRYD_CMDS_RUN_H_
