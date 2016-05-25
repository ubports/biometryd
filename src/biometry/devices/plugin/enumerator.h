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

#ifndef BIOMETRYD_DEVICES_PLUGIN_ENUMERATOR_H_
#define BIOMETRYD_DEVICES_PLUGIN_ENUMERATOR_H_

#include <biometry/device.h>
#include <biometry/do_not_copy_or_move.h>
#include <biometry/visibility.h>

#include <boost/filesystem.hpp>

#include <functional>

namespace biometry
{
namespace devices
{
namespace plugin
{
/// @brief Enumerator enumerates known plugins.
class BIOMETRY_DLL_PUBLIC Enumerator : public DoNotCopyOrMove
{
public:
    /// @brief Functor models a function invoked with a plugin descriptor.
    typedef std::function<void(const Device::Descriptor::Ptr&)> Functor;

    /// @brief enumerate invokes f for every plugin known to the Enumerator.
    /// @return the number of invocations of f.
    virtual std::size_t enumerate(const Functor& f) const = 0;

protected:
    /// @cond
    Enumerator() = default;
    /// @endcond
};

/// @brief DirectoryEnumerator implements Enumerator, enumerating all plugins located in a directory.
class BIOMETRY_DLL_PUBLIC DirectoryEnumerator : public Enumerator
{
public:
    /// @brief DirectoryEnumerator initializes a new instance with the given directory.
    explicit DirectoryEnumerator(const boost::filesystem::path& directory);

    // From Enumerator.
    std::size_t enumerate(const Functor& f) const override;

private:
    boost::filesystem::path directory;
};
}
}
}

#endif // BIOMETRYD_DEVICES_PLUGIN_ENUMERATOR_H_
