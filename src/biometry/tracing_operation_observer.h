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

#include <biometry/operation.h>

#include <iostream>
#include <string>

namespace biometry
{
template<typename T>
class TracingObserver : public biometry::Operation<T>::Observer
{
public:
    typedef typename biometry::Operation<T>::Observer Super;

    TracingObserver(std::size_t offset = 0, std::ostream& out = std::cout)
        : indent(offset, ' '),
          out{out}
    {
    }

    void on_started()
    {
        out << indent << __FUNCTION__ << std::endl;
    }

    void on_progress(const typename Super::Progress&)
    {
        out << indent << __FUNCTION__ << std::endl;
    }

    void on_canceled(const typename Super::Reason&)
    {
        out << indent << __FUNCTION__ << std::endl;
    }

    void on_failed(const typename Super::Error&)
    {
        out << indent << __FUNCTION__ << std::endl;
    }

    void on_succeeded(const typename Super::Result&)
    {
        out << indent << __FUNCTION__ << std::endl;
    }

private:
    std::string indent;
    std::ostream& out;
};
}

#endif // BIOMETRYD_TRACING_OPERATION_OBSERVER_H_
