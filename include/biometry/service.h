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

#ifndef BIOMETRYD_SERVICE_H_
#define BIOMETRYD_SERVICE_H_

#include <biometry/do_not_copy_or_move.h>
#include <biometry/visibility.h>

#include <memory>

namespace biometry
{
/// @cond
class Device;
/// @endcond
///
/// @brief Service mediates/multiplexes access to the biometric devices known to the system.
class BIOMETRY_DLL_PUBLIC Service : public DoNotCopyOrMove
{
public:
    /// @brief default_device returns the system-wide default device that should be used
    /// for identification/verification purposes.
    virtual std::shared_ptr<Device> default_device() const = 0;

protected:
    Service() = default;
};
}

#endif // BIOMETRYD_SERVICE_H_
