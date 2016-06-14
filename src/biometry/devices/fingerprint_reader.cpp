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

#include <biometry/devices/fingerprint_reader.h>

#include <biometry/dictionary.h>
#include <biometry/progress.h>

namespace
{

class GuidedEnrollmentOperation : public biometry::Operation<biometry::devices::FingerprintReader::GuidedEnrollment>
{
public:
    class EnrollmentObserver : public biometry::Operation<biometry::TemplateStore::Enrollment>::Observer
    {
    public:
        using Super = biometry::Operation<biometry::TemplateStore::Enrollment>::Observer;

        explicit EnrollmentObserver(const GuidedEnrollmentOperation::Observer::Ptr& impl) : impl{impl}
        {
        }

        void on_started() override
        {
            impl->on_started();
        }

        void on_progress(const Super::Progress& in) override
        {
            biometry::devices::FingerprintReader::GuidedEnrollment::Progress out;

            out.percent = in.percent;
            out.guidance.from_dictionary(in.details);

            impl->on_progress(out);
        }

        void on_canceled(const Super::Reason& reason) override
        {
            impl->on_canceled(reason);
        }

        void on_failed(const Super::Error& error) override
        {
            impl->on_failed(error);
        }

        void on_succeeded(const Super::Result& result) override
        {
            impl->on_succeeded(result);
        }

    private:
        GuidedEnrollmentOperation::Observer::Ptr impl;
    };

    explicit GuidedEnrollmentOperation(const biometry::Operation<biometry::TemplateStore::Enrollment>::Ptr& impl) : impl{impl}
    {
    }

    void start_with_observer(const typename Observer::Ptr& observer) override
    {
        impl->start_with_observer(std::make_shared<GuidedEnrollmentOperation::EnrollmentObserver>(observer));
    }

    void cancel() override
    {
        impl->cancel();
    }

private:
    biometry::Operation<biometry::TemplateStore::Enrollment>::Ptr impl;
};
}

void biometry::devices::FingerprintReader::GuidedEnrollment::Hints::from_dictionary(const biometry::Dictionary& dict)
{
    is_main_cluster_identified.reset();
    if (dict.count("is_main_cluster_identified") > 0)
        is_main_cluster_identified = dict.at("is_main_cluster_identified").boolean();

    suggested_next_direction.reset();
    if (dict.count("suggested_next_direction") > 0)
        suggested_next_direction = static_cast<biometry::devices::FingerprintReader::Direction>(dict.at("suggested_next_direction").integer());

    if (dict.count("masks") > 0)
    {
        masks = std::vector<biometry::Rectangle>{};

        auto v = dict.at("masks").vector();

        for (const auto& m : v)
            masks->push_back(m.rectangle());
    } else
    {
       masks.reset();
    }
}

biometry::Dictionary biometry::devices::FingerprintReader::GuidedEnrollment::Hints::to_dictionary() const
{
    biometry::Dictionary dict;
    if (is_main_cluster_identified)
        dict["is_main_cluster_identified"] = biometry::Variant::b(*is_main_cluster_identified);

    if (suggested_next_direction)
       dict["suggested_next_direction"] = biometry::Variant::i(static_cast<std::uint64_t>(*suggested_next_direction));

   if (masks)
   {
       std::vector<biometry::Variant> v;
       for (const auto& r : *masks)
           v.push_back(biometry::Variant::r(r));

       dict["masks"] = biometry::Variant::v(v);
   }

   return dict;
}

biometry::devices::FingerprintReader::TemplateStore::TemplateStore(const std::reference_wrapper<biometry::TemplateStore>& impl)
    : impl{impl}
{
}

biometry::Operation<biometry::devices::FingerprintReader::GuidedEnrollment>::Ptr biometry::devices::FingerprintReader::TemplateStore::guided_enroll(const Application& app, const User& user)
{
    return std::make_shared<GuidedEnrollmentOperation>(enroll(app, user));
}

biometry::Operation<biometry::TemplateStore::SizeQuery>::Ptr biometry::devices::FingerprintReader::TemplateStore::size(const Application& app, const User& user)
{
    return impl.get().size(app, user);
}

biometry::Operation<biometry::TemplateStore::List>::Ptr biometry::devices::FingerprintReader::TemplateStore::list(const Application& app, const User& user)
{
    return impl.get().list(app, user);
}

biometry::Operation<biometry::TemplateStore::Enrollment>::Ptr biometry::devices::FingerprintReader::TemplateStore::enroll(const Application& app, const User& user)
{
    return impl.get().enroll(app, user);
}

biometry::Operation<biometry::TemplateStore::Removal>::Ptr biometry::devices::FingerprintReader::TemplateStore::remove(const Application& app, const User& user, TemplateId id)
{
    return impl.get().remove(app, user, id);
}

biometry::Operation<biometry::TemplateStore::Clearance>::Ptr biometry::devices::FingerprintReader::TemplateStore::clear(const Application& app, const User& user)
{
    return impl.get().clear(app, user);
}

biometry::devices::FingerprintReader::FingerprintReader(const std::shared_ptr<Device>& device)
    : device_{device},
      template_store_{std::ref(device_->template_store())}
{
}

biometry::devices::FingerprintReader::TemplateStore& biometry::devices::FingerprintReader::template_store_with_guided_enrollment()
{
    return template_store_;
}

biometry::TemplateStore& biometry::devices::FingerprintReader::template_store()
{
    return template_store_;
}

biometry::Identifier& biometry::devices::FingerprintReader::identifier()
{
    return device_->identifier();
}

biometry::Verifier& biometry::devices::FingerprintReader::verifier()
{
    return device_->verifier();
}

bool biometry::devices::operator==(
        const FingerprintReader::GuidedEnrollment::Hints& lhs,
        const FingerprintReader::GuidedEnrollment::Hints& rhs)
{
    return std::tie(lhs.is_main_cluster_identified, lhs.suggested_next_direction, lhs.masks) ==
           std::tie(rhs.is_main_cluster_identified, rhs.suggested_next_direction, rhs.masks);
}
