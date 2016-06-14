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

#ifndef BIOMETRYD_DEVICES_DISPATCHING_H_
#define BIOMETRYD_DEVICES_DISPATCHING_H_

#include <biometry/device.h>

#include <biometry/identifier.h>
#include <biometry/template_store.h>
#include <biometry/verifier.h>

#include <biometry/util/dispatcher.h>

#include <boost/asio.hpp>

#include <memory>

namespace biometry
{
namespace devices
{
/// @brief Dispatching is a biometry::Device that dispatches calls to a second biometry::Device implementation via an executor.
class BIOMETRY_DLL_PUBLIC Dispatching : public biometry::Device
{
public:
    // Safe us some typing.
    typedef std::shared_ptr<Dispatching> Ptr;

    class TemplateStore : public biometry::TemplateStore
    {
    public:
        TemplateStore(const std::shared_ptr<biometry::util::Dispatcher>& dispatcher, const std::shared_ptr<biometry::Device>& impl);

        // From biometry::TemplateStore.
        biometry::Operation<biometry::TemplateStore::SizeQuery>::Ptr size(const biometry::Application& app, const biometry::User& user) override;
        biometry::Operation<biometry::TemplateStore::List>::Ptr list(const biometry::Application& app, const biometry::User& user) override;
        biometry::Operation<biometry::TemplateStore::Enrollment>::Ptr enroll(const biometry::Application& app, const biometry::User& user) override;
        biometry::Operation<biometry::TemplateStore::Removal>::Ptr remove(const biometry::Application& app, const biometry::User& user, biometry::TemplateStore::TemplateId id) override;
        biometry::Operation<biometry::TemplateStore::Clearance>::Ptr clear(const biometry::Application& app, const biometry::User& user) override;

    private:
        std::shared_ptr<biometry::util::Dispatcher> dispatcher;
        std::shared_ptr<biometry::Device> impl;
    };

    class Identifier : public biometry::Identifier
    {
    public:
        Identifier(const std::shared_ptr<biometry::util::Dispatcher>& dispatcher, const std::shared_ptr<biometry::Device>& impl);

        // From biometry::Identifier.
        biometry::Operation<biometry::Identification>::Ptr identify_user(const biometry::Application& app, const biometry::Reason& reason) override;

    private:
        std::shared_ptr<biometry::util::Dispatcher> dispatcher;
        std::shared_ptr<biometry::Device> impl;
    };

    class Verifier : public biometry::Verifier
    {
    public:
        Verifier(const std::shared_ptr<biometry::util::Dispatcher>& dispatcher, const std::shared_ptr<biometry::Device>& impl);

        // From biometry::Identifier.
        Operation<Verification>::Ptr verify_user(const Application& app, const User& user, const Reason& reason) override;

    private:
        std::shared_ptr<biometry::util::Dispatcher> dispatcher;
        std::shared_ptr<biometry::Device> impl;
    };

    /// @brief Forwarding creates a new instance, forwarding calls to device.
    /// @throws std::runtime_error if device is null.
    Dispatching(const std::shared_ptr<biometry::util::Dispatcher>& dispatcher, const std::shared_ptr<Device>& device);

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

#endif // BIOMETRYD_DEVICES_DISPATCHING_H_
