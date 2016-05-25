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

#include <biometry/util/dynamic_library.h>

#include <dlfcn.h>

#include <boost/format.hpp>

namespace
{
namespace glibc
{
struct DlApi : public biometry::util::DynamicLibrary::Api
{
    // See man dlopen.
    biometry::util::DynamicLibrary::Handle open(const boost::filesystem::path& path) const override
    {
        if (auto handle = ::dlopen(path.string().c_str(), RTLD_NOW | RTLD_LOCAL))
            return biometry::util::DynamicLibrary::Handle{handle};

        throw biometry::util::DynamicLibrary::Api::Error{*this};
    }

    // See man dlclose.
    void close(const biometry::util::DynamicLibrary::Handle& handle) const override
    {
        if (::dlclose(handle.as<>()) < 0)
            throw biometry::util::DynamicLibrary::Api::Error{*this};
    }

    // See man dlsym.
    biometry::util::DynamicLibrary::Symbol sym(const biometry::util::DynamicLibrary::Handle& handle, const std::string& symbol) const override
    {
        if (auto sh = ::dlsym(handle.as<>(), symbol.c_str()))
            return biometry::util::DynamicLibrary::Symbol{sh};

        throw biometry::util::DynamicLibrary::Api::Error{*this};
    }

    // See man dlerror.
    std::string error() const override
    {
        if (auto e = ::dlerror())
            return e;

        return std::string{};
    }
};
}
}

biometry::util::DynamicLibrary::OpaqueTypeIsEmpty::OpaqueTypeIsEmpty()
    : std::runtime_error{"Opaque type is empty."}
{
}

biometry::util::DynamicLibrary::Api::Error::Error(const Api& api) : std::runtime_error{api.error()}
{
}

biometry::util::DynamicLibrary::NoSuchSymbol::NoSuchSymbol(const std::string& symbol, const Api::Error& error)
    : std::runtime_error{(boost::format("Failed to resolve %1%: %2%.") % symbol % error.what()).str()}
{
}

// DynamicLibrary opens the dynamic library located at path, relying on api to do so.
// Throws Api::Error in case of issues.
biometry::util::DynamicLibrary::DynamicLibrary(const std::shared_ptr<Api>& api, const boost::filesystem::path& path)
    : api{api},
      handle{api->open(path)}
{
}

biometry::util::DynamicLibrary::~DynamicLibrary()
{
    api->close(handle);
}

biometry::util::DynamicLibrary::Symbol biometry::util::DynamicLibrary::resolve_symbol_or_throw(const std::string& symbol) const
{
    try
    {
        return api->sym(handle, symbol);
    }
    catch(const Api::Error& e)
    {
        throw NoSuchSymbol{symbol, e};
    }
}

std::shared_ptr<biometry::util::DynamicLibrary::Api> biometry::util::glibc::dl_api()
{
    return std::make_shared< ::glibc::DlApi >();
}
