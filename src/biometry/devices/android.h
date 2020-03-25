/*
 * Copyright (C) 2020 Canonical, Ltd.
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
 * Authored by: Erfan Abdi <erfangplus@gmail.com>
 *
 */

#ifndef BIOMETRYD_DEVICES_ANDROID_H_
#define BIOMETRYD_DEVICES_ANDROID_H_

#include <biometry/device.h>

#include <biometry/identifier.h>
#include <biometry/template_store.h>
#include <biometry/verifier.h>

#include <biometry/hardware/biometry.h>

namespace biometry
{
namespace devices
{
/// @brief android is a biometry::Device.
class BIOMETRY_DLL_PUBLIC android : public biometry::Device
{
public:
    static constexpr const char* id{"android"};

    class TemplateStore : public biometry::TemplateStore
    {
    public:
        TemplateStore(UHardwareBiometry hybris_fp_instance);

        // From biometry::TemplateStore.
        biometry::Operation<biometry::TemplateStore::SizeQuery>::Ptr size(const biometry::Application& app, const biometry::User& user) override;
        biometry::Operation<biometry::TemplateStore::List>::Ptr list(const biometry::Application& app, const biometry::User& user) override;
        biometry::Operation<biometry::TemplateStore::Enrollment>::Ptr enroll(const biometry::Application& app, const biometry::User& user) override;
        biometry::Operation<biometry::TemplateStore::Removal>::Ptr remove(const biometry::Application& app, const biometry::User& user, biometry::TemplateStore::TemplateId id) override;
        biometry::Operation<biometry::TemplateStore::Clearance>::Ptr clear(const biometry::Application& app, const biometry::User& user) override;

    private:
        UHardwareBiometry hybris_fp_instance;
    };

    class Identifier : public biometry::Identifier
    {
    public:
        Identifier(UHardwareBiometry hybris_fp_instance);

        // From biometry::Identifier.
        biometry::Operation<biometry::Identification>::Ptr identify_user(const biometry::Application& app, const biometry::Reason& reason) override;

    private:
        UHardwareBiometry hybris_fp_instance;
    };

    class Verifier : public biometry::Verifier
    {
    public:
        Verifier(UHardwareBiometry hybris_fp_instance);

        // From biometry::Identifier.
        Operation<Verification>::Ptr verify_user(const Application& app, const User& user, const Reason& reason) override;

    private:
        UHardwareBiometry hybris_fp_instance;
    };

    /// @brief make_descriptor returns a descriptor instance describing a android device;
    static Descriptor::Ptr make_descriptor();

    /// @brief android initializes a new instance.
    android(UHardwareBiometry hybris_fp_instance);

    // From biometry::Device
    biometry::TemplateStore& template_store() override;
    biometry::Identifier& identifier() override;
    biometry::Verifier& verifier() override;

private:
    TemplateStore template_store_;
    Identifier identifier_;
    Verifier verifier_;
};
}
}

#endif // BIOMETRYD_DEVICES_ANDROID_H_
