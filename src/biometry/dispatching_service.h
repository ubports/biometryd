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

#ifndef BIOMETRYD_DISPATCHING_SERVICE_H_
#define BIOMETRYD_DISPATCHING_SERVICE_H_

#include <biometry/service.h>

#include <biometry/devices/dispatching.h>

#include <boost/asio.hpp>

#include <memory>

namespace biometry
{
/// @brief DispatchingService is biometry::Service wrapping device impls as a devices::Dispatching device.
class BIOMETRY_DLL_PUBLIC DispatchingService : public Service
{
public:
    /// @brief DispatchingService initializes a new instance with the given default_device.
    DispatchingService(const std::shared_ptr<biometry::util::Dispatcher>& dispatcher, const std::shared_ptr<Device>& default_device);

    // From Service.
    std::shared_ptr<Device> default_device() const override;

protected:
    devices::Dispatching::Ptr default_device_;
};
}

#endif // BIOMETRYD_DISPATCHING_SERVICE_H_
