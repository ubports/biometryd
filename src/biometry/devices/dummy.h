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

#ifndef BIOMETRYD_DEVICES_DUMMY_H_
#define BIOMETRYD_DEVICES_DUMMY_H_

#include <biometry/device.h>

#include <biometry/identifier.h>
#include <biometry/template_store.h>
#include <biometry/verifier.h>

namespace biometry
{
namespace devices
{
/// @brief Dummy is a biometry::Device.
class BIOMETRY_DLL_PUBLIC Dummy : public biometry::Device
{
public:
    static constexpr const char* id{"Dummy"};

    template<typename T>
    struct Operation : public biometry::Operation<T>
    {
        void start_with_observer(const typename biometry::Operation<T>::Observer::Ptr& observer) override
        {
            observer->on_started();
            typename biometry::Operation<T>::Result result{};
            observer->on_succeeded(result);
        }

        void cancel() override
        {

        }
    };

    class TemplateStore : public biometry::TemplateStore
    {
    public:
        // From biometry::TemplateStore.
        biometry::Operation<biometry::TemplateStore::SizeQuery>::Ptr size(const biometry::Application& app, const biometry::User& user) override;
        biometry::Operation<biometry::TemplateStore::List>::Ptr list(const biometry::Application& app, const biometry::User& user) override;
        biometry::Operation<biometry::TemplateStore::Enrollment>::Ptr enroll(const biometry::Application& app, const biometry::User& user) override;
        biometry::Operation<biometry::TemplateStore::Removal>::Ptr remove(const biometry::Application& app, const biometry::User& user, biometry::TemplateStore::TemplateId id) override;
        biometry::Operation<biometry::TemplateStore::Clearance>::Ptr clear(const biometry::Application& app, const biometry::User& user) override;
    };

    class Identifier : public biometry::Identifier
    {
    public:
        // From biometry::Identifier.
        biometry::Operation<biometry::Identification>::Ptr identify_user(const biometry::Application& app, const biometry::Reason& reason) override;
    };

    class Verifier : public biometry::Verifier
    {
    public:
        // From biometry::Identifier.
        Operation<Verification>::Ptr verify_user(const Application& app, const User& user, const Reason& reason) override;
    };

    /// @brief make_descriptor returns a descriptor instance describing a Dummy device;
    static Descriptor::Ptr make_descriptor();

    /// @brief Dummy initializes a new instance.
    Dummy();

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

#endif // BIOMETRYD_DEVICES_DUMMY_H_
