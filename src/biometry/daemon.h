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

#ifndef BIOMETRYD_DEVICE_DAEMON_H_
#define BIOMETRYD_DEVICE_DAEMON_H_

#include <biometry/device_registrar.h>
#include <biometry/do_not_copy_or_move.h>
#include <biometry/visibility.h>

#include <biometry/util/cli.h>

#include <boost/filesystem.hpp>

#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

namespace biometry
{
/// @brief Daemon implements biometryd.
class BIOMETRY_DLL_PUBLIC Daemon : public DoNotCopyOrMove
{
public:
    /// @brief Configuration bundles compile time configuration options of the daemon.
    struct Configuration
    {
        /// @brief default_plugin_directory returns the path pointing to the default plugin directory.
        static boost::filesystem::path default_plugin_directory();
    };

    /// @brief Daemon creates a new instance, populating the map of known commands.
    Daemon();

    /// @brief run executes the daemon.
    int run(const std::vector<std::string>& args);

private:
    DeviceRegistrar device_registrar;
    util::cli::CommandWithSubcommands cmd;
};



}

#endif // BIOMETRYD_DEVICE_DAEMON_H_
