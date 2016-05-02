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

#ifndef UTIL_CONFIGURATION_H_
#define UTIL_CONFIGURATION_H_

#include <biometry/do_not_copy_or_move.h>
#include <util/variant.h>

#include <boost/filesystem.hpp>

#include <map>
#include <string>

namespace util
{
class Configuration
{
public:
    /// @cond
    class Node;
    typedef std::map<std::string, Node> Children;
    /// @endcond

    /// @brief Node represents a single named configuration value.
    class Node
    {
    public:
        /// @brief Node constructs a new instance with the given name,
        /// initializing the contained value to value.
        Node(const Variant& value = Variant{});

        /// @brief value returns an immutable reference to the contained value.
        virtual const Variant& value() const;
        /// @brief value adjusts the contained value.
        virtual Node& value(const Variant& value);

        /// @brief children returns the set of all children of this node.
        virtual const Children& children() const;
        /// @brief children returns a mutable reference to the children of this node.
        virtual Children& children();
        /// @brief Returns a mutable reference to the child with the given name.
        Node& operator[](const std::string& name);

    private:
        Variant value_; ///< mutable value of the Node.
        Children children_; ///< mutable set of all children_ of this Node.
    };

    /// @brief children returns the set of all children of this node.
    virtual const Children& children() const;
    /// @brief children returns a mutable reference to the children of this node.
    virtual Children& children();
    /// @brief Returns a mutable reference to the child with the given name.
    Node& operator[](const std::string& name);
private:
    Children children_; ///< mutable set of all children_ of this Node.
};

/// @brief ConfigurationBuilder models loading of configuration from arbitrary sources.
class ConfigurationBuilder : public biometry::DoNotCopyOrMove
{
public:
    /// @brief build_configuration returns a Configuration instance.
    /// @throws in case of issues.
    virtual Configuration build_configuration() = 0;

protected:
    /// @cond
    ConfigurationBuilder() = default;
    /// @endcond
};

/// @brief JsonConfigurationBuilder implements ConfigurationBuilder for sources in JSON format.
class JsonConfigurationBuilder : public ConfigurationBuilder
{
public:
    /// @brief JsonConfigurationBuilder creates a new instance for the given path.
    JsonConfigurationBuilder(const boost::filesystem::path& path);
    /// build_configuration returns a Configuration assembled from a JSON-formatted file.
    Configuration build_configuration() override;

private:
    boost::filesystem::path path;
};
}

#endif // UTIL_CONFIGURATION_H_
