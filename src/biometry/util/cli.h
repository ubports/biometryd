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
#ifndef BIOMETRY_UTIL_CLI_H_
#define BIOMETRY_UTIL_CLI_H_

#include <biometry/do_not_copy_or_move.h>
#include <biometry/optional.h>
#include <biometry/visibility.h>

#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

namespace biometry
{
namespace util
{
namespace cli
{
template<std::size_t max>
class BIOMETRY_DLL_PUBLIC SizeConstrainedString
{
public:
    SizeConstrainedString(const std::string& s) : s{s}
    {
        if(s.size() > max)
            throw std::logic_error{"Max size exceeded " + std::to_string(max)};
    }

    const std::string& as_string() const
    {
        return s;
    }

    operator std::string() const
    {
        return s;
    }

private:
    std::string s;
};

template<std::size_t max>
BIOMETRY_DLL_PUBLIC bool operator<(const SizeConstrainedString<max>& lhs, const SizeConstrainedString<max>& rhs)
{
    return lhs.as_string() < rhs.as_string();
}

template<std::size_t max>
BIOMETRY_DLL_PUBLIC bool operator==(const SizeConstrainedString<max>& lhs, const SizeConstrainedString<max>& rhs)
{
    return lhs.as_string() == rhs.as_string();
}

template<std::size_t max>
BIOMETRY_DLL_PUBLIC std::ostream& operator<<(std::ostream& out, const SizeConstrainedString<max>& scs)
{
    return out << std::setw(max) << std::left << scs.as_string();
}

/// @brief Command abstracts an individual command available from the daemon.
class BIOMETRY_DLL_PUBLIC Command : public DoNotCopyOrMove
{
public:
    // Safe us some typing
    typedef std::shared_ptr<Command> Ptr;

    // We are imposing size constraints to ensure a consistent CLI layout.
    typedef SizeConstrainedString<15> Name;
    typedef SizeConstrainedString<60> Usage;
    typedef SizeConstrainedString<60> Description;

    /// @brief Flag models an input parameter to a command.
    class Flag : public DoNotCopyOrMove
    {
    public:
        // Safe us some typing.
        typedef std::shared_ptr<Flag> Ptr;

        /// @brief notify announces a new value to the flag.
        virtual void notify(const std::string& value) = 0;
        /// @brief name returns the name of the Flag.
        const Name& name() const;
        /// @brief description returns a human-readable description of the flag.
        const Description& description() const;

    protected:
        /// @brief Flag creates a new instance, initializing name and description
        /// from the given values.
        Flag(const Name& name, const Description& description);

    private:
        Name name_;
        Description description_;
    };

    /// @brief Info bundles details about a command.
    struct Info
    {
        Name name;  ///< The name of the command.
        Usage usage; ///< Short usage description of the command.
        Description description; /// More detailed description of the command.
        std::vector<Flag::Ptr> flags; /// Flags known to the command.
    };

    template<typename T>
    class TypedFlag : public Flag
    {
    public:
        typedef std::shared_ptr<TypedFlag<T>> Ptr;

        TypedFlag(const Name& name, const Description& description) : Flag{name, description}
        {
        }

        /// @brief value installs the given value in the flag.
        TypedFlag& value(const T& value)
        {
            value_ = value;
            return *this;
        }

        /// @brief value returns the optional value associated with the flag.
        const Optional<T>& value() const
        {
            return value_;
        }

        /// @brief notify tries to unwrap a value of type T from value.
        void notify(const std::string& s) override
        {
            std::stringstream ss{s};
            T value; ss >> value;
            value_ = value;
        }

    private:
        Optional<T> value_;
    };

    template<typename T>
    class TypedReferenceFlag : public Flag
    {
    public:
        typedef std::shared_ptr<TypedReferenceFlag<T>> Ptr;

        TypedReferenceFlag(const Name& name, const Description& description, T& value)
            : Flag{name, description},
              value_{value}
        {
        }

        /// @brief notify tries to unwrap a value of type T from value.
        void notify(const std::string& s) override
        {
            std::stringstream ss{s};
            ss >> value_.get();
        }

    private:
        std::reference_wrapper<T> value_;
    };

    /// @brief OptionalTypedReferenceFlag handles Optional<T> references, making sure that
    /// a value is always read on notify, even if the Optional<T> wasn't initialized previously.
    template<typename T>
    class OptionalTypedReferenceFlag : public Flag
    {
    public:
        typedef std::shared_ptr<OptionalTypedReferenceFlag<T>> Ptr;

        OptionalTypedReferenceFlag(const Name& name, const Description& description, Optional<T>& value)
            : Flag{name, description},
              value_{value}
        {
        }

        /// @brief notify tries to unwrap a value of type T from value.
        void notify(const std::string& s) override
        {
            std::stringstream ss{s}; T value; ss >> value;
            value_.get() = value;
        }

    private:
        std::reference_wrapper<Optional<T>> value_;
    };

    /// @brief info returns Info about a command.
    virtual Info info() const;
    /// @brief run puts the command to execution.
    virtual int run();

protected:
    /// @brief Command initializes a new instance with the given info and functor.
    Command(const Info& info, const std::function<int()>& run = std::function<int()>());

    /// @brief info returns a mutable reference to info_.
    Info& mutable_info();

    /// @brief run returns a mutable reference to run_.
    std::function<int()>& mutable_run();

private:
    Info info_;
    std::function<int()> run_;
};

template<typename T>
BIOMETRY_DLL_PUBLIC typename Command::TypedFlag<T>::Ptr make_flag(const Command::Name& name, const Command::Description& desc)
{
    return std::make_shared<Command::TypedFlag<T>>(name, desc);
}

template<typename T>
BIOMETRY_DLL_PUBLIC typename Command::TypedReferenceFlag<T>::Ptr make_flag(const Command::Name& name, const Command::Description& desc, T& value)
{
    return std::make_shared<Command::TypedReferenceFlag<T>>(name, desc, value);
}

template<typename T>
BIOMETRY_DLL_PUBLIC typename Command::OptionalTypedReferenceFlag<T>::Ptr make_flag(const Command::Name& name, const Command::Description& desc, Optional<T>& value)
{
    return std::make_shared<Command::OptionalTypedReferenceFlag<T>>(name, desc, value);
}
}
}
}

#endif // BIOMETRY_UTIL_CLI_H_
