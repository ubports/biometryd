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

#ifndef BIOMETRYD_DBUS_INTERFACE_H_
#define BIOMETRYD_DBUS_INTERFACE_H_

#include <core/dbus/macros.h>
#include <core/dbus/types/object_path.h>

#include <chrono>
#include <string>

namespace biometry
{
namespace dbus
{
namespace interface
{
struct Service
{
    static inline std::string name()
    {
        return "com.ubuntu.biometryd.Service";
    }

    static inline core::dbus::types::ObjectPath path()
    {
        return core::dbus::types::ObjectPath{"/"};
    }

    struct Methods
    {
        Methods() = delete;

        struct DefaultDevice
        {
            static inline std::string name()
            {
                return "DefaultDevice";
            }

            typedef biometry::dbus::interface::Service Interface;
            typedef core::dbus::types::ObjectPath ResultType;

            inline static const std::chrono::milliseconds default_timeout()
            {
                return std::chrono::seconds{1};
            }
        };
    };
};

struct Device
{
    static inline const std::string& name()
    {
        static const std::string s{"com.ubuntu.biometryd.Device"};
        return s;
    }

    struct Methods
    {
        Methods() = delete;

        struct TemplateStore
        {
            static inline const std::string& name()
            {
                static const std::string s{"TemplateStore"};
                return s;
            }

            typedef biometry::dbus::interface::Device Interface;
            typedef core::dbus::types::ObjectPath ResultType;

            inline static const std::chrono::milliseconds default_timeout()
            {
                return std::chrono::seconds{1};
            }
        };

        struct Identifier
        {
            static inline const std::string& name()
            {
                static const std::string s{"Identifier"};
                return s;
            }


            typedef biometry::dbus::interface::Device Interface;
            typedef core::dbus::types::ObjectPath ResultType;

            inline static const std::chrono::milliseconds default_timeout()
            {
                return std::chrono::seconds{1};
            }
        };

        struct Verifier
        {
            static inline const std::string& name()
            {
                static const std::string s{"Verifier"};
                return s;
            }


            typedef biometry::dbus::interface::Device Interface;
            typedef core::dbus::types::ObjectPath ResultType;

            inline static const std::chrono::milliseconds default_timeout()
            {
                return std::chrono::seconds{1};
            }
        };
    };
};

struct Identifier
{
    static inline const std::string& name()
    {
        static const std::string s{"com.ubuntu.biometryd.Identifier"};
        return s;
    }

    struct Methods
    {
        Methods() = delete;

        struct IdentifyUser
        {
            static inline const std::string& name()
            {
                static const std::string s{"IdentifyUser"};
                return s;
            }

            typedef biometry::dbus::interface::Identifier Interface;
            typedef core::dbus::types::ObjectPath ResultType;

            inline static const std::chrono::milliseconds default_timeout()
            {
                return std::chrono::seconds{1};
            }
        };
    };
};

struct TemplateStore
{
    static inline const std::string& name()
    {
        static const std::string s{"com.ubuntu.biometryd.TemplateStore"};
        return s;
    }

    struct Methods
    {
        Methods() = delete;

        struct Size
        {
            static inline const std::string& name()
            {
                static const std::string s{"Size"};
                return s;
            }

            typedef biometry::dbus::interface::TemplateStore Interface;
            typedef core::dbus::types::ObjectPath ResultType;

            inline static const std::chrono::milliseconds default_timeout()
            {
                return std::chrono::seconds{1};
            }
        };

        struct List
        {
            static inline const std::string& name()
            {
                static const std::string s{"List"};
                return s;
            }

            typedef biometry::dbus::interface::TemplateStore Interface;
            typedef core::dbus::types::ObjectPath ResultType;

            inline static const std::chrono::milliseconds default_timeout()
            {
                return std::chrono::seconds{1};
            }
        };

        struct Enroll
        {
            static inline const std::string& name()
            {
                static const std::string s{"Enroll"};
                return s;
            }


            typedef biometry::dbus::interface::TemplateStore Interface;
            typedef core::dbus::types::ObjectPath ResultType;

            inline static const std::chrono::milliseconds default_timeout()
            {
                return std::chrono::seconds{1};
            }
        };

        struct Remove
        {
            static inline const std::string& name()
            {
                static const std::string s{"Remove"};
                return s;
            }

            typedef biometry::dbus::interface::TemplateStore Interface;
            typedef core::dbus::types::ObjectPath ResultType;

            inline static const std::chrono::milliseconds default_timeout()
            {
                return std::chrono::seconds{1};
            }
        };

        struct Clear
        {
            static inline const std::string& name()
            {
                static const std::string s{"Clear"};
                return s;
            }


            typedef biometry::dbus::interface::TemplateStore Interface;
            typedef core::dbus::types::ObjectPath ResultType;

            inline static const std::chrono::milliseconds default_timeout()
            {
                return std::chrono::seconds{1};
            }
        };
    };
};

struct Operation
{
    struct Observer
    {
        static inline const std::string& name()
        {
            static const std::string s{"com.ubuntu.biometryd.Operation.Observer"};
            return s;
        }

        struct Methods
        {
            Methods() = delete;

            DBUS_CPP_METHOD_DEF(OnStarted,      Observer)
            DBUS_CPP_METHOD_DEF(OnProgress,     Observer)
            DBUS_CPP_METHOD_DEF(OnCancelled,    Observer)
            DBUS_CPP_METHOD_DEF(OnFailed,       Observer)
            DBUS_CPP_METHOD_DEF(OnSucceeded,    Observer)
        };
    };

    static inline const std::string& name()
    {
        static const std::string s{"com.ubuntu.biometryd.Operation"};
        return s;
    }

    struct Methods
    {
        Methods() = delete;

        struct StartWithObserver
        {
            static inline const std::string& name()
            {
                static const std::string s{"StartWithOberserver"};
                return s;
            }

            typedef biometry::dbus::interface::Operation Interface;
            typedef void ResultType;

            inline static const std::chrono::milliseconds default_timeout()
            {
                return std::chrono::seconds{1};
            }
        };

        struct Cancel
        {
            static inline const std::string& name()
            {
                static const std::string s{"Cancel"};
                return s;
            }

            typedef biometry::dbus::interface::Operation Interface;
            typedef void ResultType;

            inline static const std::chrono::milliseconds default_timeout()
            {
                return std::chrono::seconds{1};
            }
        };
    };
};
}
}
}

namespace core
{
namespace dbus
{
namespace traits
{
template<> struct Service<biometry::dbus::interface::Service>
{
    static inline const std::string& interface_name()
    {
        static const std::string s{"com.ubuntu.biometryd.Service"};
        return s;
    }
};
}
}
}

#endif // BIOMETRYD_DBUS_INTERFACE_H_

