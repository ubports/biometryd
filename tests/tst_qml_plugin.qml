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
        onStarted: { console.log("started") }
        onCanceled: { console.log("canceled") }
        onFailed: { console.log("failed") }
        onProgressed: {
            console.log("progressed: ", percent.toFixed(2));
            if (hints["fingerprintReaderHints"]) {
                console.log(hints["fingerprintReaderHints"].hasMainClusterIdentified)
                console.log(hints["fingerprintReaderHints"])
            }
        }
        onSucceeded: {console.log("succeeded") }
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
        ts.enroll(user).start(observer);
        spy.wait(5000);
        ts.size(user).start(observer);
        spy.wait(5000);
        ts.clear(user).start(observer);
        spy.wait(5000);
    }

    function test_identifierOfDefaultDeviceIsAvailable() {
        var identifier = Biometryd.defaultDevice.identifier;
        identifier.identifyUser().start(observer);
        spy.wait(5000);
    }
}
