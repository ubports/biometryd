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

#ifndef BIOMETRYD_DEVICES_FINGERPRINT_READER_H_
#define BIOMETRYD_DEVICES_FINGERPRINT_READER_H_

#include <biometry/device.h>
#include <biometry/optional.h>
#include <biometry/percent.h>
#include <biometry/geometry.h>
#include <biometry/template_store.h>
#include <biometry/visibility.h>

namespace biometry
{
namespace devices
{
/// @brief FingerprintReader specializes a biometry::Device, providing
/// methods and structures to handle specific feedback during enrollment of
/// fingerprints.
class BIOMETRY_DLL_PUBLIC FingerprintReader : public Device
{
public:
    /// @brief Direction enumerates all known direction hints.
    enum class Direction
    {
        not_available   = 0,
        sout_west       = 1,
        south           = 2,
        south_east      = 3,
        north_west      = 4,
        north           = 5,
        north_east      = 6,
        east            = 7,
        west            = 8
    };

    /// @brief GuidedEnrollment describes a guided enrollment operation with the
    /// underlying hw/driver stack providing guidance data to users of the device
    /// such that enrollment can happen as fast as possible.
    struct GuidedEnrollment
    {
        struct Hints
        {
            /// @brief from_dictionary decodes guidance data from dict.
            void from_dictionary(const Dictionary& dict);
            /// @brief to_dictionary encodes guidance data to dict.
            Dictionary to_dictionary() const;

            Optional<bool> is_main_cluster_identified{}; ///< If set: true indicates that the main cluster of the fingerprint has been identified.
            Optional<Direction> suggested_next_direction{}; ///< If set: Direction of the next touch.
            Optional<std::vector<Rectangle>> masks{}; ///< If set: A vector of rectangles marking all the regions that have been scanned and accepted.
        };

        /// @brief ProgressWithGuidance bundles guidance data meant for visualization purposes
        /// to help the user when enrolling a new fingerprint.
        struct ProgressWithGuidance
        {
            Percent percent{Percent::from_raw_value(0.f)}; ///< Percentage completed.
            Hints guidance;
        };

        typedef ProgressWithGuidance                        Progress;
        typedef biometry::TemplateStore::Enrollment::Reason Reason;
        typedef biometry::TemplateStore::Enrollment::Error  Error;
        typedef biometry::TemplateStore::Enrollment::Result Result;
    };

    /// @brief TemplateStore is an implementation of biometry::TemplateStore providing
    /// additional guidance data during the enrollment process.
    class TemplateStore : public biometry::TemplateStore
    {
    public:
        /// @brief TemplateStore initializes a new instance with the given impl.
        explicit TemplateStore(const std::reference_wrapper<biometry::TemplateStore>& impl);

        /// @brief guided_enroll returns a GuidedEnrollment operation ready to be started.
        Operation<GuidedEnrollment>::Ptr guided_enroll(const Application& app, const User& user);

        // From biometry::TemplateStore.
        Operation<SizeQuery>::Ptr size(const Application& app, const User& user) override;
        Operation<Enrollment>::Ptr enroll(const Application& app, const User& user) override;
        Operation<Clearance>::Ptr clear(const Application& app, const User& user) override;

    private:
        /// @cond
        std::reference_wrapper<biometry::TemplateStore> impl;
        /// @endcond
    };

    /// @brief FingerprintReader initializes a new instance with the given device instance.
    explicit FingerprintReader(const std::shared_ptr<Device>& device);

    /// @brief template_store_with_guided_enrollment returns a TemplateStore providing support for guided enrollments.
    FingerprintReader::TemplateStore& template_store_with_guided_enrollment();

    /// @brief enroller returns a device-specific template_store implementation.
    biometry::TemplateStore& template_store() override;
    /// @brief identifier returns a device-specific Identifier implementation.
    biometry::Identifier& identifier() override;
    /// @brief verifier returns a device-specific Verifier implementation.
    biometry::Verifier& verifier() override;

private:
    /// @cond
    std::shared_ptr<Device> device_;
    FingerprintReader::TemplateStore template_store_;
    /// @endcond
};

/// @brief operator== returns true if lhs and rhs compare equal.
BIOMETRY_DLL_PUBLIC bool operator==(
        const FingerprintReader::GuidedEnrollment::Hints& lhs,
        const FingerprintReader::GuidedEnrollment::Hints& rhs);
}
}

#endif // BIOMETRYD_DEVICES_FINGERPRINT_READER_H_
