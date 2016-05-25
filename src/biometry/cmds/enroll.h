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

#ifndef BIOMETRYD_CMDS_ENROLL_H_
#define BIOMETRYD_CMDS_ENROLL_H_

#include <biometry/util/cli.h>

#include <biometry/optional.h>
#include <biometry/user.h>

#include <boost/filesystem.hpp>

#include <functional>
#include <iostream>
#include <memory>

namespace biometry
{
namespace cmds
{
/// @brief Enroll requests enrollment of a new template to a biometric device.
class Enroll : public util::cli::CommandWithFlagsAndAction
{
public:
    /// @brief Enroll creates a new instance, initializing flags to default values.
    Enroll();

private:
    std::string device;
    Optional<boost::filesystem::path> config;
    User user;
};
}
}

#endif // BIOMETRYD_CMDS_ENROLL_H_
