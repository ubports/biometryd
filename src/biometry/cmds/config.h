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

#ifndef BIOMETRYD_CMDS_CONFIG_H_
#define BIOMETRYD_CMDS_CONFIG_H_

#include <biometry/optional.h>
#include <biometry/visibility.h>

#include <biometry/util/cli.h>

#include <iosfwd>
#include <memory>

namespace biometry
{
namespace cmds
{
/// @brief Config queries configuration options of biometryd.
class BIOMETRY_DLL_PUBLIC Config : public util::cli::CommandWithFlagsAndAction
{
public:
    /// @brief Flag enumerates all known configuration flags queryable by the command.
    enum class Flag
    {
        default_plugin_directory ///< The default plugin installation directory.
    };

    /// @brief Config configures a new instance.
    Config();

private:
    biometry::Optional<Flag> flag_;
};

/// @brief operator<< inserts flag into out and returns out.
BIOMETRY_DLL_PUBLIC std::ostream& operator<<(std::ostream& out, Config::Flag flag);
/// @brief operator>> extracts flag from in, and returns in.
BIOMETRY_DLL_PUBLIC std::istream& operator>>(std::istream& in, Config::Flag& flag);
}
}

#endif // BIOMETRYD_CMDS_CONFIG_H_
