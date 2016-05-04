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


#ifndef TESTING_DID_FINISH_SUCCESSFULLY_H_
#define TESTING_DID_FINISH_SUCCESSFULLY_H_

#include <core/posix/wait.h>

#include <gtest/gtest.h>

namespace core
{
namespace posix
{
void PrintTo(Signal signal, std::ostream* os)
{
    switch (signal)
    {
    case Signal::unknown:
        *os << "unknown";
        break;
    case Signal::sig_hup:
        *os << "sig_hup";
        break;
    case Signal::sig_int:
        *os << "sig_int";
        break;
    case Signal::sig_quit:
        *os << "sig_quit";
        break;
    case Signal::sig_ill:
        *os << "sig_ill";
        break;
    case Signal::sig_abrt:
        *os << "sig_abrt";
        break;
    case Signal::sig_fpe:
        *os << "sig_fpe";
        break;
    case Signal::sig_kill:
        *os << "sig_kill";
        break;
    case Signal::sig_segv:
        *os << "sig_segv";
        break;
    case Signal::sig_pipe:
        *os << "sig_pipe";
        break;
    case Signal::sig_alrm:
        *os << "sig_alrm";
        break;
    case Signal::sig_term:
        *os << "sig_term";
        break;
    case Signal::sig_usr1:
        *os << "sig_usr1";
        break;
    case Signal::sig_usr2:
        *os << "sig_usr2";
        break;
    case Signal::sig_chld:
        *os << "sig_chld";
        break;
    case Signal::sig_cont:
        *os << "sig_cont";
        break;
    case Signal::sig_stop:
        *os << "sig_stop";
        break;
    case Signal::sig_tstp:
        *os << "sig_tstp";
        break;
    case Signal::sig_ttin:
        *os << "sig_ttin";
        break;
    case Signal::sig_ttou:
        *os << "sig_ttou";
        break;
    }
}

namespace wait
{

void PrintTo(Result::Status status, std::ostream* os)
{
    switch (status)
    {
    case core::posix::wait::Result::Status::undefined: *os << "undefined"; break;
    case core::posix::wait::Result::Status::no_state_change: *os << "no_statue_change"; break;
    case core::posix::wait::Result::Status::exited: *os << "exited"; break;
    case core::posix::wait::Result::Status::signaled: *os << "signaled"; break;
    case core::posix::wait::Result::Status::stopped: *os << "stopped"; break;
    case core::posix::wait::Result::Status::continued: *os << "continued"; break;
    }
}

void PrintTo(const Result& result, std::ostream* os)
{
    *os << testing::PrintToString(result.status);
    if (result.status == Result::Status::signaled)
        *os << " with " << testing::PrintToString(result.detail.if_signaled.signal);
}
}
}
}

namespace testing
{
::testing::AssertionResult did_exit(const core::posix::wait::Result& result)
{
    if (result.status != core::posix::wait::Result::Status::exited)
        ::testing::AssertionFailure() << "Process did not exit: " << testing::PrintToString(result);

    return ::testing::AssertionSuccess();
}

::testing::AssertionResult did_finish_successfully(const core::posix::wait::Result& result)
{
    if (result.status != core::posix::wait::Result::Status::exited)
    {
        return ::testing::AssertionFailure() << "Process did not exit, but: " << testing::PrintToString(result);
    }
    if (result.detail.if_exited.status != core::posix::exit::Status::success)
        return ::testing::AssertionFailure() << "Process did exit with failure.";

    return ::testing::AssertionSuccess();
}
}

#endif // TESTING_DID_FINISH_SUCCESSFULLY_H_
