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

#ifndef BIOMETRY_UTIL_JSON_CONFIGURATION_BUILDER_H_
#define BIOMETRY_UTIL_JSON_CONFIGURATION_BUILDER_H_

#include <biometry/util/configuration.h>
#include <biometry/visibility.h>

#include <iosfwd>
#include <map>
#include <string>

namespace biometry
{
namespace util
{
/// @brief JsonConfigurationBuilder implements ConfigurationBuilder for sources in JSON format.
class BIOMETRY_DLL_PUBLIC JsonConfigurationBuilder : public ConfigurationBuilder
{
public:
    /// @brief JsonConfigurationBuilder initializes a new instance reading from the given input stream.
    JsonConfigurationBuilder(std::istream& in);
    /// build_configuration returns a Configuration assembled from a JSON-formatted file.
    Configuration build_configuration() override;

private:
    std::istream& in;
};
}
}

#endif // BIOMETRY_UTIL_CONFIGURATION_H_
