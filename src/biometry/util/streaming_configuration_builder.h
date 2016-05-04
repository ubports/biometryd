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

#ifndef BIOMETRY_UTIL_STREAMING_CONFIGURATION_BUILDER_H_
#define BIOMETRY_UTIL_STREAMING_CONFIGURATION_BUILDER_H_

#include <biometry/util/configuration.h>
#include <biometry/visibility.h>

#include <boost/filesystem.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <string>

namespace biometry
{
namespace util
{
/// @brief JsonConfigurationBuilder implements ConfigurationBuilder for sources that take std::istream& on construction.
template<typename Builder>
class BIOMETRY_DLL_PUBLIC StreamingConfigurationBuilder : public ConfigurationBuilder
{
public:
    /// @brief Streamer abstracts the lifetime of a reference to a std::istream instance.
    class Streamer : public biometry::DoNotCopyOrMove
    {
    public:
        /// @brief in returns a mutable reference to the managed std::istream.
        virtual std::istream& in() = 0;
    protected:
        /// @cond
        Streamer() = default;
        /// @endcond
    };

    static std::shared_ptr<Streamer> make_streamer(std::istream& in)
    {
        return std::make_shared<RefStreamer>(in);
    }

    static std::shared_ptr<Streamer> make_streamer(const boost::filesystem::path& p)
    {
        return std::make_shared<IfStreamer>(p);
    }

    /// @brief JsonConfigurationBuilder initializes a new instance reading from the given input stream.
    StreamingConfigurationBuilder(const std::shared_ptr<Streamer>& streamer)
        : streamer{streamer},
          builder{streamer->in()}
    {
    }

    /// @brief JsonConfigurationBuilder initializes a new instance reading from the given input stream.
    StreamingConfigurationBuilder(std::istream& in)
        : StreamingConfigurationBuilder{make_streamer(in)}
    {
    }

    /// @brief JsonConfigurationBuilder initializes a new instance reading from the given file.
    StreamingConfigurationBuilder(const boost::filesystem::path& path)
        : StreamingConfigurationBuilder{make_streamer(path)}
    {
    }

    /// build_configuration returns a Configuration assembled from a JSON-formatted file.
    Configuration build_configuration() override
    {
        return builder.build_configuration();
    }

private:
    /// @cond
    class RefStreamer : public Streamer
    {
    public:
        RefStreamer(std::istream& in) : in_{in}
        {
            if(not in_) throw std::runtime_error
            {
                "Given input stream is not ready for reading."
            };
        }

        std::istream& in() override
        {
            return in_;
        }

    private:
        std::istream& in_;
    };

    class IfStreamer : public Streamer
    {
    public:
        IfStreamer(const boost::filesystem::path& path) : in_{path.string()}
        {
        }

        std::istream& in() override
        {
            return in_;
        }

    private:
        std::ifstream in_;
    };
    /// @endcond

    std::shared_ptr<Streamer> streamer;
    Builder builder;
};
}
}

#endif // BIOMETRY_UTIL_STREAMING_CONFIGURATION_BUILDER_H_
