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

#include <biometry/devices/plugin/loader.h>
#include <biometry/devices/plugin/verifier.h>
#include <biometry/devices/plugin/interface.h>

#include <boost/format.hpp>

#include <iostream>
#include <system_error>

#include <sys/types.h>
#include <fcntl.h>

#include <gelf.h>

namespace plugin = biometry::devices::plugin;

std::shared_ptr<biometry::Device> plugin::NonVerifyingLoader::verify_and_load(const std::shared_ptr<util::DynamicLibrary::Api>& api, const boost::filesystem::path& path) const
{
    auto dl = std::make_shared<util::DynamicLibrary>(api, path);

    util::DynamicLibrary::Symbol create = dl->resolve_symbol_or_throw(BIOMETRYD_DEVICES_PLUGIN_CREATE_SYMBOL_NAME);
    util::DynamicLibrary::Symbol destroy = dl->resolve_symbol_or_throw(BIOMETRYD_DEVICES_PLUGIN_DESTROY_SYMBOL_NAME);

    return std::shared_ptr<biometry::Device>{
        create.as<BiometrydPluginDeviceCreate>()(),
        // We are passing in dl to keep the instance alive for as long as the
        // shared plugin instance is alive.
        [dl, destroy](biometry::Device* dev)
        {
            if (dev)
                destroy.as<BiometrydPluginDeviceDestroy>()(dev);
        }
    };
}

namespace
{
/// @brief Fd performs resource handling of an opaque file descriptor.
struct Fd
{
    /// @brief Constructs a new instance, taking over ownership of handle.
    /// @throws std::system_error if the handle is < 0.
    Fd(int handle) : handle{handle}
    {
        if (handle < 0)
            throw std::system_error(errno, std::system_category());
    }

    ~Fd()
    {
        if (handle >= 0)
            ::close(handle);
    }

    int handle;
};
}

plugin::ElfDescriptorLoader::FailedToInitializeElf::FailedToInitializeElf()
    : std::runtime_error{(boost::format("Failed to initialize elf: %1%") % elf_errmsg(-1)).str()}
{
}

plugin::ElfDescriptorLoader::NotAnElfObject::NotAnElfObject()
    : std::runtime_error{"Not an elf object"}
{
}

plugin::ElfDescriptorLoader::NoSuchSection::NoSuchSection(const std::string& section)
    : std::runtime_error{"Failed to resolve section: " + section}
{
}

plugin::Descriptor plugin::ElfDescriptorLoader::load_with_name(const boost::filesystem::path& p, const std::string& section) const
{
    if (elf_version( EV_CURRENT ) == EV_NONE)
        throw std::runtime_error{"elf version mismatch"};

    Fd fd{::open(p.string().c_str(), O_RDONLY)};

    Elf* e{nullptr};
    if (not (e = elf_begin(fd.handle, ELF_C_READ, nullptr)))
        throw FailedToInitializeElf{};

    if (elf_kind(e) != ELF_K_ELF)
        throw NotAnElfObject{};

    std::size_t shstrndx{0};
    if (elf_getshdrstrndx(e, &shstrndx) != 0)
        throw std::runtime_error{"Failed to retrieve index of the section name string table."};

    Elf_Scn* sec{nullptr};
    while ((sec = elf_nextscn(e, sec)) != nullptr)
    {
        GElf_Shdr hdr;
        if (&hdr != gelf_getshdr(sec, &hdr))
            continue;

        char* name{nullptr};
        if ((name = elf_strptr(e, shstrndx, hdr.sh_name)) == nullptr)
            continue;

        if (name != section)
            continue;

        Elf_Data* data{nullptr};
        if ((data = elf_getdata(sec, data)) && data->d_size == sizeof(plugin::Descriptor))
            return *reinterpret_cast<plugin::Descriptor*>(data->d_buf);
    }

    throw NoSuchSection(section);
}

std::shared_ptr<biometry::Device> plugin::ElfDescriptorVerifierLoader::verify_and_load(const std::shared_ptr<util::DynamicLibrary::Api>& api, const boost::filesystem::path& path) const
{
    MajorVersionVerifier{}.verify(ElfDescriptorLoader{}.load_with_name(path, BIOMETRYD_DEVICES_PLUGIN_DESCRIPTOR_SECTION));
    return NonVerifyingLoader{}.verify_and_load(api, path);
}
