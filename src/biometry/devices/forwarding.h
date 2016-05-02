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

#ifndef BIOMETRYD_DEVICES_FORWARDING_H_
#define BIOMETRYD_DEVICES_FORWARDING_H_

#include <biometry/device.h>

namespace biometry
{
namespace devices
{
/// @brief Forwarding is a biometry::Device that dispatches calls to a second biometry::Device implementation.
class BIOMETRY_DLL_PUBLIC Forwarding : public biometry::Device
{
public:
    /// @brief Forwarding creates a new instance, forwarding calls to device.
    /// @throws std::runtime_error if device is null.
    Forwarding(const std::shared_ptr<Device>& device);

    // From biometry::Device
    TemplateStore& template_store() override;
    Identifier& identifier() override;
    Verifier& verifier() override;

private:
    std::shared_ptr<Device> impl; ///< The actual device we are forwarding to.
};
}
}

#endif // BIOMETRYD_DEVICES_FORWARDING_H_
