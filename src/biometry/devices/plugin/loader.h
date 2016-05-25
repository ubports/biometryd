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

#ifndef BIOMETRYD_DEVICES_PLUGIN_LOADER_H_
#define BIOMETRYD_DEVICES_PLUGIN_LOADER_H_

#include <biometry/device.h>
#include <biometry/visibility.h>

#include <biometry/devices/plugin/interface.h>

#include <biometry/util/dynamic_library.h>

#include <boost/filesystem.hpp>

namespace biometry
{
namespace devices
{
namespace plugin
{
/// @brief Loader provides means to verify (prior to loading) and loading a biometry::Device impl from a dynamic library.
class BIOMETRY_DLL_PUBLIC Loader : public DoNotCopyOrMove
{
public:
    /// @brief verify_and_load returns a biometry::Device instance resolved from the dynamic library pointed to by path.
    /// @throws util::DynamicLibrary::Error in case of issues accessing the dynamic library.
    virtual std::shared_ptr<biometry::Device> verify_and_load(const std::shared_ptr<util::DynamicLibrary::Api>& api, const boost::filesystem::path& path) const = 0;

protected:
    /// @cond
    Loader() = default;
    /// @endcond
};

/// @brief NonVerifyingLoader implements Loader, skipping the verification step altogether.
struct BIOMETRY_DLL_PUBLIC NonVerifyingLoader : public Loader
{
    /// @brief verify_and_load tries to load a Device plugin from the dynamic library located at path, leveraging api to
    /// open the library and resolve symbols from it.
    std::shared_ptr<biometry::Device> verify_and_load(const std::shared_ptr<util::DynamicLibrary::Api>& api, const boost::filesystem::path& path) const override;
};

/// @brief ElfDescriptorLoader loads a plugin::Descriptor instance from a dynamic library.
struct BIOMETRY_DLL_PUBLIC ElfDescriptorLoader
{
public:
    /// @brief FailedToInitializeElf is thrown if the loader fails to initialize the elf access library for a given file.
    struct FailedToInitializeElf : public std::runtime_error
    {
        FailedToInitializeElf();
    };

    /// @brief NotAnElfObject is thrown if the loader is invoked on a file that is not an elf object.
    struct NotAnElfObject : public std::runtime_error
    {
        NotAnElfObject();
    };

    /// @brief NoSuchSection is thrown if a named section in an elf executable cannot be found.
    struct NoSuchSection : public std::runtime_error
    {
        NoSuchSection(const std::string& section);
    };

    /// @brief load_with_name opens the dynamic library located at p, tries to find section section, interpreting it as a plugin::Descriptor on return.
    /// @throws NoSuchSection if a section of the given name cannot be found.
    plugin::Descriptor load_with_name(const boost::filesystem::path& p, const std::string& section) const;
};

/// @brief ElfDescriptorVerifierLoader checks if the biometry::Device plugin contained in the library
/// located at path is binary compatible with the current runtime version of biometryd. If so, it tries
/// to load the library and create a biometry::Device instance from it.
struct BIOMETRY_DLL_PUBLIC ElfDescriptorVerifierLoader : public Loader
{
    std::shared_ptr<biometry::Device> verify_and_load(const std::shared_ptr<util::DynamicLibrary::Api>& api, const boost::filesystem::path& path) const override;
};
}
}
}

#endif // BIOMETRYD_DEVICES_PLUGIN_LOADER_H_
