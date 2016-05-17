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

#ifndef BIOMETRYD_TRACING_OPERATION_OBSERVER_H_
#define BIOMETRYD_TRACING_OPERATION_OBSERVER_H_

#include <biometry/dictionary.h>
#include <biometry/operation.h>
#include <biometry/progress.h>
#include <biometry/variant.h>
#include <biometry/void.h>

#include <iostream>
#include <string>

namespace biometry
{
namespace detail
{
// Printer helps in pretty printing a value with a given offset as in:
//   [  offset * ' '  ]value;
// We rely on this customization to make it easy to provide nicely formatted output
// without the need to provide a full abstraction on top of a std::ostream while still being
// able to only have one TracingObserver<T> without type specializations.
template<typename T>
struct Printer
{
    static std::ostream& print(std::ostream& out, const T& value, std::size_t offset)
    {
        return out << std::string(offset, ' ') << value;
    }
};

template<>
struct Printer<biometry::Dictionary>
{
    static std::ostream& print(std::ostream& out, const biometry::Dictionary& dict, std::size_t offset)
    {
        std::string indent(offset, ' ');
        for (auto it = dict.begin(); it != dict.end(); ++it)
            out << indent << it->first << " = " << it->second << (it != std::prev(dict.end()) ? "" : "");

        return out;
    }
};

template<>
struct Printer<biometry::Progress>
{
    static std::ostream& print(std::ostream& out, const biometry::Progress& progress, std::size_t offset)
    {
        std::string indent(offset, ' ');
        out << indent << progress.percent << " complete";

        if (progress.details.empty())
            return out;

        out << ":" << std::endl;
        Printer<biometry::Dictionary>::print(out, progress.details, offset + 2);
        return out;
    }
};

template<>
struct Printer<biometry::Void>
{
    static std::ostream& print(std::ostream& out, const biometry::Void&, std::size_t)
    {
        return out;
    }
};
}
template<typename T>
class TracingObserver : public biometry::Operation<T>::Observer
{
public:
    typedef typename biometry::Operation<T>::Observer Super;

    TracingObserver(std::size_t offset = 0, std::ostream& out = std::cout)
        : offset{offset},
          indent(offset, ' '),
          out{out}
    {
    }

    void on_started() override
    {
        out << indent << __FUNCTION__ << std::endl;
    }

    void on_progress(const typename Super::Progress& progress) override
    {
        out << indent << __FUNCTION__<< ": " << std::endl;
        detail::Printer<typename Super::Progress>::print(out, progress, offset + 2) << std::endl;
    }

    void on_canceled(const typename Super::Reason& reason) override
    {
        out << indent << __FUNCTION__ << ": " << std::endl;
        detail::Printer<typename Super::Reason>::print(out, reason, offset + 2) << std::endl;
    }

    void on_failed(const typename Super::Error& error) override
    {
        out << indent << __FUNCTION__ << ": " << std::endl;
        detail::Printer<typename Super::Error>::print(out, error, offset + 2) << std::endl;
    }

    void on_succeeded(const typename Super::Result& result) override
    {
        out << indent << __FUNCTION__ << std::endl;
        detail::Printer<typename Super::Result>::print(out, result, offset + 2) << std::endl;
    }

private:
    std::size_t offset;
    std::string indent;
    std::ostream& out;
};


}

#endif // BIOMETRYD_TRACING_OPERATION_OBSERVER_H_
