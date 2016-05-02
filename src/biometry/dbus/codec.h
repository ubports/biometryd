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

#ifndef BIOMETRYD_DBUS_CODEC_H_
#define BIOMETRYD_DBUS_CODEC_H_

#include <biometry/application.h>
#include <biometry/geometry.h>
#include <biometry/progress.h>
#include <biometry/reason.h>
#include <biometry/user.h>
#include <biometry/variant.h>
#include <biometry/void.h>

#include <core/dbus/types/variant.h>
#include <core/dbus/types/stl/map.h>
#include <core/dbus/types/stl/string.h>
#include <core/dbus/types/stl/vector.h>

#include <iostream>

namespace core
{
namespace dbus
{
namespace helper
{
template<>
struct TypeMapper<biometry::Variant::None>
{
    constexpr static inline ArgumentType type_value()
    {
        return ArgumentType::structure;
    }

    constexpr static bool is_basic_type()
    {
        return false;
    }

    constexpr static bool requires_signature()
    {
        return true;
    }

    static std::string signature()
    {
        return DBUS_STRUCT_BEGIN_CHAR_AS_STRING DBUS_TYPE_VARIANT_AS_STRING DBUS_STRUCT_END_CHAR_AS_STRING;
    }
};

template<>
struct TypeMapper<biometry::Point>
{
    constexpr static inline ArgumentType type_value()
    {
        return ArgumentType::structure;
    }
    constexpr static bool is_basic_type()
    {
        return false;
    }
    constexpr static bool requires_signature()
    {
        return true;
    }

    static std::string signature()
    {
        return DBUS_STRUCT_BEGIN_CHAR_AS_STRING
                + TypeMapper<std::uint32_t>::signature()
                + TypeMapper<std::uint32_t>::signature()
                + DBUS_STRUCT_END_CHAR_AS_STRING;
    }
};

template<>
struct TypeMapper<biometry::Rectangle>
{
    constexpr static inline ArgumentType type_value()
    {
        return ArgumentType::structure;
    }
    constexpr static bool is_basic_type()
    {
        return false;
    }
    constexpr static bool requires_signature()
    {
        return true;
    }

    static std::string signature()
    {
        return DBUS_STRUCT_BEGIN_CHAR_AS_STRING
                + TypeMapper<biometry::Point>::signature()
                + TypeMapper<biometry::Point>::signature()
                + DBUS_STRUCT_END_CHAR_AS_STRING;
    }
};

template<>
struct TypeMapper<biometry::Variant>
{
    constexpr static inline ArgumentType type_value()
    {
        return ArgumentType::variant;
    }

    constexpr static bool is_basic_type()
    {
        return false;
    }

    constexpr static bool requires_signature()
    {
        return true;
    }

    static std::string signature()
    {
        return DBUS_STRUCT_BEGIN_CHAR_AS_STRING DBUS_TYPE_UINT16_AS_STRING DBUS_TYPE_VARIANT_AS_STRING DBUS_STRUCT_END_CHAR_AS_STRING;
    }
};

template<>
struct TypeMapper<biometry::Void>
{
    constexpr static inline ArgumentType type_value()
    {
        return ArgumentType::structure;
    }

    constexpr static bool is_basic_type()
    {
        return false;
    }

    constexpr static bool requires_signature()
    {
        return true;
    }

    static std::string signature()
    {
        return DBUS_STRUCT_BEGIN_CHAR_AS_STRING DBUS_TYPE_VARIANT_AS_STRING DBUS_STRUCT_END_CHAR_AS_STRING;
    }
};
}
template<> struct Codec<biometry::Application>
{
    static void encode_argument(Message::Writer& out, const biometry::Application& in)
    {
        out.push_stringn(in.as_string().c_str(), in.as_string().size());
    }

    static void decode_argument(Message::Reader& in, biometry::Application& out)
    {
        out = biometry::Application{in.pop_string()};
    }
};

template<> struct Codec<biometry::Progress>
{
    static void encode_argument(Message::Writer& out, const biometry::Progress& in)
    {
        auto sw = out.open_structure();
        {
            sw.push_floating_point(*in.percent);
            Codec<biometry::Dictionary>::encode_argument(sw, in.details);
        }
        out.close_structure(std::move(sw));
    }

    static void decode_argument(Message::Reader& in, biometry::Progress& out)
    {
        auto sr = in.pop_structure();
        {
            out.percent = biometry::Percent::from_raw_value(sr.pop_floating_point());
            Codec<biometry::Dictionary>::decode_argument(sr, out.details);
        }
    }
};

template<> struct Codec<biometry::Point>
{
    static void encode_argument(Message::Writer& out, const biometry::Point& in)
    {
        auto sw = out.open_structure();
        {
            sw.push_uint32(in.x);
            sw.push_uint32(in.y);
        }
        out.close_structure(std::move(sw));
    }

    static void decode_argument(Message::Reader& in, biometry::Point& out)
    {
        auto sr = in.pop_structure();
        out.x = sr.pop_uint32();
        out.y = sr.pop_uint32();
    }
};

template<> struct Codec<biometry::Rectangle>
{
    static void encode_argument(Message::Writer& out, const biometry::Rectangle& in)
    {
        auto sw = out.open_structure();
        {
            Codec<biometry::Point>::encode_argument(sw, in.top_left);
            Codec<biometry::Point>::encode_argument(sw, in.bottom_right);
        }
        out.close_structure(std::move(sw));
    }

    static void decode_argument(Message::Reader& in, biometry::Rectangle& out)
    {
        auto sr = in.pop_structure();
        Codec<biometry::Point>::decode_argument(sr, out.top_left);
        Codec<biometry::Point>::decode_argument(sr, out.bottom_right);
    }
};

template<> struct Codec<biometry::Reason>
{
    static void encode_argument(Message::Writer& out, const biometry::Reason& in)
    {
        out.push_stringn(in.as_string().c_str(), in.as_string().size());
    }

    static void decode_argument(Message::Reader& in, biometry::Reason& out)
    {
        out = biometry::Reason{in.pop_string()};
    }
};

template<> struct Codec<biometry::User>
{
    static void encode_argument(Message::Writer& out, const biometry::User& in)
    {
        out.push_int32(in.id);
    }

    static void decode_argument(Message::Reader& in, biometry::User& out)
    {
        out.id = in.pop_int32();
    }
};

template<> struct Codec<biometry::Variant::None>
{
    static void encode_argument(Message::Writer& out, const biometry::Variant::None&)
    {
        auto sw = out.open_structure();
        {
            static const std::string s{"42"};
            sw.push_stringn(s.c_str(), s.size());
        }
        out.close_structure(std::move(sw));
    }

    static void decode_argument(Message::Reader& in, biometry::Variant::None&)
    {
        in.pop_structure();
    }
};

template<> struct Codec<biometry::Variant>
{
    static void encode_argument(Message::Writer& out, const biometry::Variant& in)
    {
        auto sw = out.open_structure();
        {
            sw.push_uint16(static_cast<std::uint16_t>(in.type()));

            switch (in.type())
            {
            case biometry::Variant::Type::none:
                static const biometry::Variant::None none;
                Codec<biometry::Variant::None>::encode_argument(sw, none);
                break;
            case biometry::Variant::Type::boolean:
            {
                auto vw = sw.open_variant(types::Signature{helper::TypeMapper<bool>::signature()});
                {
                    vw.push_boolean(in.boolean());
                }
                sw.close_variant(std::move(vw));
                break;
            }
            case biometry::Variant::Type::integer:
            {
                auto vw = sw.open_variant(types::Signature{helper::TypeMapper<std::int64_t>::signature()});
                {
                    vw.push_int64(in.integer());
                }
                sw.close_variant(std::move(vw));
                break;
            }
            case biometry::Variant::Type::floating_point:
            {
                auto vw = sw.open_variant(types::Signature{helper::TypeMapper<double>::signature()});
                {
                    vw.push_floating_point(in.floating_point());
                }
                sw.close_variant(std::move(vw));
                break;
            }
            case biometry::Variant::Type::rectangle:
            {
                auto vw = sw.open_variant(types::Signature{helper::TypeMapper<biometry::Rectangle>::signature()});
                {
                    Codec<biometry::Rectangle>::encode_argument(vw, in.rectangle());
                }
                sw.close_variant(std::move(vw));
                break;
            }
            case biometry::Variant::Type::string:
            {
                auto vw = sw.open_variant(types::Signature{helper::TypeMapper<std::string>::signature()});
                {
                    vw.push_stringn(in.string().c_str(), in.string().size());
                }
                sw.close_variant(std::move(vw));
                break;
            }
            case biometry::Variant::Type::blob:
            {
                break;
            }
            case biometry::Variant::Type::vector:
            {
                auto vw = sw.open_variant(core::dbus::types::Signature{"a(qv)"});
                {
                    auto aw = vw.open_array(core::dbus::types::Signature{"(qv)"});
                    {
                        for (const auto& v : in.vector())
                            Codec<biometry::Variant>::encode_argument(aw, v);
                    }
                    vw.close_array(std::move(aw));
                }
                sw.close_variant(std::move(vw));
                break;
            }
            }
        }
        out.close_structure(std::move(sw));
    }

    static void decode_argument(Message::Reader& in, biometry::Variant& out)
    {
        auto sr = in.pop_structure();
        {
            auto type = static_cast<biometry::Variant::Type>(sr.pop_uint16());
            auto vr = sr.pop_variant();

            switch (type)
            {
            case biometry::Variant::Type::none:
                static biometry::Variant::None none; Codec<biometry::Variant::None>::decode_argument(vr, none);
                break;
            case biometry::Variant::Type::boolean:
            {
                out.boolean(vr.pop_boolean());
                break;
            }
            case biometry::Variant::Type::integer:
            {
                out.integer(vr.pop_int64());
                break;
            }
            case biometry::Variant::Type::floating_point:
            {
                out.floating_point(vr.pop_floating_point());
                break;
            }
            case biometry::Variant::Type::rectangle:
            {
                biometry::Rectangle r; Codec<biometry::Rectangle>::decode_argument(vr, r);
                out.rectangle(r);
                break;
            }
            case biometry::Variant::Type::string:
            {
                out.string(vr.pop_string());
                break;
            }
            case biometry::Variant::Type::blob:
            {
                break;
            }
            case biometry::Variant::Type::vector:
            {
                std::vector<biometry::Variant> v; Codec<std::vector<biometry::Variant>>::decode_argument(vr, v);
                out.vector(v);
                break;
            }
            }
        }
    }
};

template<> struct Codec<biometry::Void>
{
    static void encode_argument(Message::Writer& out, const biometry::Void&)
    {
        auto sw = out.open_structure();
        {
            static const std::string s{"42"};
            sw.push_stringn(s.c_str(), s.size());
        }
        out.close_structure(std::move(sw));
    }

    static void decode_argument(Message::Reader& in, biometry::Void&)
    {
        in.pop_structure();
    }
};

}
}

#endif // BIOMETRYD_DBUS_CODEC_H_
