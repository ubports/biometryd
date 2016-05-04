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

import QtQuick 2.0
import QtTest 1.0
import Biometryd 0.0

TestCase {
    name: "Biometryd"

    Observer {
        id: observer
        onStarted: {
            console.log("started")
        }
        onCanceled: {
            console.log("canceled")
        }
        onFailed: {
            console.log("failed")
        }
        onProgressed: {
            // biometryd API users can use details to receive
            // device/operation-specific information about the
            // operation. We illustrate the case of a FingerprintReader here.
            console.log("progressed: ", percent.toFixed(2));

            var isFingerPresent             = details[FingerprintReader.isFingerPresent]
            var hasMainClusterIdentified    = details[FingerprintReader.hasMainClusterIdentified]
            var suggestedNextDirection      = details[FingerprintReader.suggestedNextDirection]
            var estimatedFingerSize         = details[FingerprintReader.estimatedFingerSize]
            var masks                       = details[FingerprintReader.masks]

            console.log("isFingerPresent:",            isFingerPresent,
                        "hasMainClusterIdentified:",   hasMainClusterIdentified,
                        "suggestedNextDirection:",     suggestedNextDirection,
                        "masks:",                      masks);
        }
        onSucceeded: {
            console.log("succeeded:", result)
        }
    }

    User {
        id: user
        uid: 0
    }

    SignalSpy {
        id: spy
        target: observer
        signalName: "succeeded"
    }

    function test_defaultDeviceIsAvailable() {
        var ts = Biometryd.defaultDevice.templateStore;
        var id = Biometryd.defaultDevice.identifier;
    }

    function test_templateStoreOfDefaultDeviceIsAvailable() {
        var ts = Biometryd.defaultDevice.templateStore;

        // The API is structured around the concept of an Operation.
        // An operation is asynchronous. Its state can be observed with the help
        // of an Observer.
        {
            var op = ts.enroll(user); op.start(observer);
            spy.wait(5000);
        }

        {
            op = ts.size(user); op.start(observer);
            spy.wait(5000);
        }

        {
            op = ts.clear(user); op.start(observer);
            spy.wait(5000);
        }
    }

    function test_identifierOfDefaultDeviceIsAvailable() {
        var identifier = Biometryd.defaultDevice.identifier;
        var op = identifier.identifyUser(); op.start(observer);
        spy.wait(5000);
    }
}
