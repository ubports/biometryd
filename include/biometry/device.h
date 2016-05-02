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

#ifndef BIOMETRY_DEVICE_H_
#define BIOMETRY_DEVICE_H_

#include <biometry/do_not_copy_or_move.h>
#include <biometry/visibility.h>

#include <iosfwd>
#include <memory>
#include <string>

namespace biometry
{
/// @cond
class Identifier;
class TemplateStore;
class Verifier;
/// @endcond

/// @brief Device models a biometric device.
class BIOMETRY_DLL_PUBLIC Device : public DoNotCopyOrMove
{
public:
    /// @brief Id is the unique name of a device.
    typedef std::string Id;

    /// @brief Descriptor bundles details about a device.
    class Descriptor : public DoNotCopyOrMove
    {
    public:
        // Safe us some typing.
        typedef std::shared_ptr<Descriptor> Ptr;

        /// @brief create returns an instance of the device.
        virtual std::shared_ptr<Device> create() = 0;
        /// @brief name returns the human-readable name of the device.
        virtual std::string name() const = 0;
        /// @brief author returns the name of the author of the device implementation.
        virtual std::string author() const = 0;
        /// @brief description returns a one-line summary of the device implementation.
        virtual std::string description() const = 0;

    protected:
        /// @cond
        Descriptor() = default;
        /// @endcond
    };

    /// @brief enroller returns a device-specific template_store implementation.
    virtual TemplateStore& template_store() = 0;
    /// @brief identifier returns a device-specific Identifier implementation.
    virtual Identifier& identifier() = 0;
    /// @brief verifier returns a device-specific Verifier implementation.
    virtual Verifier& verifier() = 0;

protected:
    /// @cond
    Device() = default;
    /// @endcond
};
}

#endif // BIOMETRY_DEVICE_H_
