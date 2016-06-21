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

#include <biometry/devices/plugin/enumerator.h>
#include <biometry/devices/plugin/loader.h>
#include <biometry/devices/plugin/verifier.h>

namespace plugin = biometry::devices::plugin;

namespace
{
struct PluginDeviceDescriptor : public biometry::Device::Descriptor
{
    typedef std::shared_ptr<PluginDeviceDescriptor> Ptr;

    PluginDeviceDescriptor(const boost::filesystem::path& path, const plugin::Descriptor& desc)
        : path{path},
          desc(desc)
    {
    }

    std::shared_ptr<biometry::Device> create(const biometry::util::Configuration&) override
    {
        return plugin::ElfDescriptorVerifierLoader{}.verify_and_load(biometry::util::glibc::dl_api(), path);
    }

    std::string name() const override
    {
        return desc.name;
    }

    std::string author() const override
    {
        return desc.author;
    }

    std::string description() const override
    {
        return desc.description;
    }

    boost::filesystem::path path;
    biometry::devices::plugin::Descriptor desc;
};
}

plugin::DirectoryEnumerator::DirectoryEnumerator(const std::set<boost::filesystem::path>& directories)
    : directories{directories}
{
}

std::size_t plugin::DirectoryEnumerator::enumerate(const Functor& f) const
{
    std::size_t invocations{0};

    for (const auto& directory : directories)
    {
        if (not boost::filesystem::is_directory(directory))
            return 0;

        MajorVersionVerifier verifier;
        ElfDescriptorLoader loader;

        for (boost::filesystem::directory_iterator it{directory}, itE; it !=  itE; ++it)
        {
            try
            {
                auto desc = verifier.verify(loader.load_with_name(it->path(), BIOMETRYD_DEVICES_PLUGIN_DESCRIPTOR_SECTION));
                f(std::make_shared<PluginDeviceDescriptor>(it->path(), desc));
                ++invocations;
            }
            catch(const ElfDescriptorLoader::FailedToInitializeElf&)
            {
                // We silently ignore the exception here as we expect to encounter
                // files missing a section describing a biometryd plugin. All other
                // exceptions will be propagated, though.
            }
            catch(const ElfDescriptorLoader::NotAnElfObject&)
            {
                // We silently ignore the exception here as we expect to encounter
                // files missing a section describing a biometryd plugin. All other
                // exceptions will be propagated, though.
            }
            catch(const ElfDescriptorLoader::NoSuchSection&)
            {
                // We silently ignore the exception here as we expect to encounter
                // files missing a section describing a biometryd plugin. All other
                // exceptions will be propagated, though.
            }
            catch(const MajorVersionVerifier::MajorVersionMismatch&)
            {
                // We silently ignore major version mismatches as we expect to
                // encounter plugins of different versions routinely. All other
                // exceptions will be propagated, though.
            }
        }
    }

    return invocations;
}
