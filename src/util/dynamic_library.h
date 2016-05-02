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

#ifndef UTIL_DYNAMIC_LIBRARY_H_
#define UTIL_DYNAMIC_LIBRARY_H_

#include <biometry/do_not_copy_or_move.h>
#include <biometry/visibility.h>

#include <boost/filesystem.hpp>

#include <string>

namespace util
{
/// @brief DynamicLibrary abstracts dl* operations, and enables integration testing
/// of components relying on the respective functionality.
class BIOMETRY_DLL_PUBLIC DynamicLibrary : public biometry::DoNotCopyOrMove
{
public:
    /// @brief OpaqueTypeIsEmpty is thrown if trying to access an instance of
    /// TaggedOpaqueType<T> that contains a nullptr.
    struct OpaqueTypeIsEmpty : public std::runtime_error
    {
        OpaqueTypeIsEmpty();
    };

    template<typename Tag> class TaggedOpaqueType
    {
    public:
        TaggedOpaqueType() : p{nullptr} {}
        template<typename T>
        explicit TaggedOpaqueType(T t) : p{reinterpret_cast<void*>(t)} {}
        template<typename T=void*> T as() const { if (not p) throw OpaqueTypeIsEmpty{}; return (T) p; }

    private:
        void* p;
    };

    struct Tag
    {
        struct Handle {};
        struct Symbol {};
    };

    typedef TaggedOpaqueType<Tag::Handle> Handle;
    typedef TaggedOpaqueType<Tag::Symbol> Symbol;

    class Api : public DoNotCopyOrMove
    {
    public:

        // Exposes dlerror to the exception hierarchy.
        class Error : public std::runtime_error
        {
        public:
            // Queries dlerror() for a human-readable error message.
            Error(const Api& api);
        };

        // See man dlopen.
        virtual Handle open(const boost::filesystem::path& path) const = 0;
        // See man dlclose.
        virtual void close(const Handle&) const = 0;
        // See man dlsym.
        virtual Symbol sym(const Handle&, const std::string& symbol) const = 0;
        // See man dlerror.
        virtual std::string error() const = 0;
    };

    /// @brief NoSuchSymbol is thrown if resolving a symbol from a dynamic library fails.
    class NoSuchSymbol : public std::runtime_error
    {
    public:
        /// @brief NoSuchSymbol creates a new instance, noting down the symbol that failed resolving.
        NoSuchSymbol(const std::string& symbol, const Api::Error& error);
    };

    // DynamicLibrary opens the dynamic library located at path, relying on api to do so.
    // Throws Api::Error in case of issues.
    DynamicLibrary(const std::shared_ptr<Api>& api, const boost::filesystem::path& path);
    ~DynamicLibrary();

    Symbol resolve_symbol_or_throw(const std::string& symbol) const;
private:
    std::shared_ptr<Api> api;
    Handle handle;
};

namespace glibc
{
std::shared_ptr<DynamicLibrary::Api> dl_api();
}

namespace bionic
{
std::shared_ptr<DynamicLibrary::Api> dl_api();
}
}

#endif // UTIL_DYNAMIC_LIBRARY_H_
