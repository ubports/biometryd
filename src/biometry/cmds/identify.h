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

#ifndef BIOMETRYD_CMDS_IDENTIFY_H_
#define BIOMETRYD_CMDS_IDENTIFY_H_

#include <biometry/daemon.h>
#include <biometry/user.h>

#include <functional>
#include <iostream>
#include <memory>

namespace biometry
{
namespace cmds
{
/// @brief Identify requests identification of the user.
class Identify : public biometry::Daemon::Command
{
public:
    /// @brief Enroll creates a new instance, initializing flags to default values.
    Identify();

    // From Daemon::Command.
    Info info() const override;
    int run() override;

private:
    TypedFlag<std::string>::Ptr device;
};
}
}

#endif // BIOMETRYD_CMDS_IDENTIFY_H_
