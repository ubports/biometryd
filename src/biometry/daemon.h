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

#ifndef BIOMETRYD_DEVICE_DAEMON_H_
#define BIOMETRYD_DEVICE_DAEMON_H_

#include <biometry/device_registrar.h>
#include <biometry/do_not_copy_or_move.h>
#include <biometry/visibility.h>

#include <boost/any.hpp>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>

#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

namespace biometry
{
template<std::size_t max>
class SizeConstrainedString
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
bool operator<(const SizeConstrainedString<max>& lhs, const SizeConstrainedString<max>& rhs)
{
    return lhs.as_string() < rhs.as_string();
}

template<std::size_t max>
bool operator==(const SizeConstrainedString<max>& lhs, const SizeConstrainedString<max>& rhs)
{
    return lhs.as_string() == rhs.as_string();
}


class BIOMETRY_DLL_PUBLIC Daemon : public DoNotCopyOrMove
{
public:
    struct Configuration
    {
        static boost::filesystem::path default_plugin_directory();
    };

    /// @brief Command abstracts an individual command available from the daemon.
    class Command : public DoNotCopyOrMove
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
            const boost::optional<T>& value() const
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
            boost::optional<T> value_;
        };

        /// @brief info returns Info about a command.
        virtual Info info() const = 0;
        /// @brief run puts the command to execution
        virtual int run() = 0;
    };

    /// @brief Daemon creates a new instance, populating the map of known commands.
    Daemon();

    /// @brief run executes the daemon.
    int run(const std::vector<std::string>& args);

private:
    void install_command(const Command::Ptr& command);

    DeviceRegistrar device_registrar;
    std::unordered_map<std::string, Command::Ptr> cmds;
    Command::Ptr help;
};

template<typename T>
BIOMETRY_DLL_PUBLIC typename Daemon::Command::TypedFlag<T>::Ptr make_command_flag(const Daemon::Command::Name& name, const Daemon::Command::Description& desc)
{
    return std::make_shared<Daemon::Command::TypedFlag<T>>(name, desc);
}

template<std::size_t max>
BIOMETRY_DLL_PUBLIC std::ostream& operator<<(std::ostream& out, const SizeConstrainedString<max>& scs)
{
    return out << scs.as_string();
}

}

#endif // BIOMETRYD_DEVICE_DAEMON_H_
