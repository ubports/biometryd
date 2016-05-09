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

#ifndef BIOMETRYD_DEVICES_PLUGIN_VERIFIER_H_
#define BIOMETRYD_DEVICES_PLUGIN_VERIFIER_H_

#include <biometry/do_not_copy_or_move.h>
#include <biometry/visibility.h>

#include <biometry/devices/plugin/interface.h>

#include <cstdint>
#include <stdexcept>

namespace biometry
{
namespace devices
{
namespace plugin
{
/// @brief A Verifier verifies that a plugin fits with biometryd.
class BIOMETRY_DLL_PUBLIC Verifier : public DoNotCopyOrMove
{
public:
    /// @brief verify returns the given descriptor if the corresponding plugin is likely to fit with the currently
    /// running biometryd version.
    virtual Descriptor verify(const Descriptor& descriptor) const = 0;

protected:
    /// @cond
    Verifier() = default;
    /// @endcond
};

struct BIOMETRY_DLL_PUBLIC MajorVersionVerifier : public Verifier
{
    /// @brief MajorVersionMismatch is thrown if the major version of host and plugin do not match.
    struct BIOMETRY_DLL_PUBLIC MajorVersionMismatch : public std::runtime_error
    {
        MajorVersionMismatch(std::uint32_t host, std::uint32_t plugin);

        const std::uint32_t host;
        const std::uint32_t plugin;
    };

    // From MajorVersionVerifier
    Descriptor verify(const Descriptor &descriptor) const override;
};

}
}
}

#endif // BIOMETRYD_DEVICES_PLUGIN_VERIFIER_H_
