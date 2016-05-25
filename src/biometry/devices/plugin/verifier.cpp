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

#include <biometry/devices/plugin/verifier.h>

#include <boost/format.hpp>

namespace plugin = biometry::devices::plugin;

plugin::MajorVersionVerifier::MajorVersionMismatch::MajorVersionMismatch(std::uint32_t host, std::uint32_t plugin)
    : std::runtime_error{(boost::format("Major version mismatch on host: %1% vs. %2%") % host % plugin).str()},
      host{host},
      plugin{plugin}
{
}

plugin::Descriptor plugin::MajorVersionVerifier::verify(const Descriptor &descriptor) const
{
    if (descriptor.version.host.major != biometry::build::version_major)
        throw MajorVersionMismatch{descriptor.version.host.major, biometry::build::version_major};

    return descriptor;
}
