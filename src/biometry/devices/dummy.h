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

#ifndef BIOMETRYD_DEVICES_DUMMY_H_
#define BIOMETRYD_DEVICES_DUMMY_H_

#include <biometry/device.h>

namespace biometry
{
namespace devices
{
/// @brief Dummy is a biometry::Device.
class BIOMETRY_DLL_PUBLIC Dummy : public biometry::Device
{
public:
    static constexpr const char* id{"Dummy"};

    /// @brief Dummy initializes a new instance.
    Dummy();

    // From biometry::Device
    TemplateStore& template_store() override;
    Identifier& identifier() override;
    Verifier& verifier() override;
};
}
}

#endif // BIOMETRYD_DEVICES_DUMMY_H_
