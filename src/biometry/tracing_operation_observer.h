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

namespace biometry
{
template<typename T>
class TracingObserver : public biometry::Operation<T>::Observer
{
public:
    typedef typename biometry::Operation<T>::Observer Super;

    void on_started()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    void on_progress(const typename Super::Progress&)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    void on_canceled(const typename Super::Reason&)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    void on_failed(const typename Super::Error&)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    void on_succeeded(const typename Super::Result&)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
};
}

#endif // BIOMETRYD_TRACING_OPERATION_OBSERVER_H_
