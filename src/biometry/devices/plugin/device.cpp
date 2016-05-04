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

#include <biometry/devices/plugin/device.h>

#include <biometry/util/configuration.h>

namespace plugin = biometry::devices::plugin;

std::shared_ptr<biometry::Device> plugin::load(const std::shared_ptr<util::DynamicLibrary::Api>& api, const boost::filesystem::path& path, const Loader& loader)
{
    return loader.verify_and_load(api, path);
}

#include <biometry/device_registry.h>

namespace
{
struct Descriptor : public biometry::Device::Descriptor
{
    std::shared_ptr<biometry::Device> create(const biometry::util::Configuration& config) override
    {
        auto api = biometry::util::glibc::dl_api();
        biometry::devices::plugin::ElfDescriptorVerifierLoader loader;
        return biometry::devices::plugin::load(api, config["path"].value().string(), loader);
    }

    std::string name() const override
    {
        return "Plugin";
    }

    std::string author() const override
    {
        return "Thomas Voß (thomas.voss@canonical.com)";
    }

    std::string description() const override
    {
        return "Plugin loads device implementations from shared modules.";
    }
};

bool register_descriptor()
{
    biometry::device_registry()[biometry::devices::plugin::id] = std::make_shared<Descriptor>();
    return true;
}

__attribute__((unused)) static const bool registered = register_descriptor();
}
