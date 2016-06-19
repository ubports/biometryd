# Introduction

Biometryd multiplexes and mediates access to devices for biometric
identification and verification. A fingerprint reader is an example of
such a device, but the overall system is designed with arbitrary
devices and mechanisms in mind.

Security and privacy is one the most important design goal. For that,
design, API and implementation do make sure that actual template
boundary is never exposed to client applications. More to this,
Biometryd and its API are designed such that actual template data is
not needed for operation (unless really needed by a device). Instead,
the API focuses on controlling and monitoring devices and operations
instead of dealing with handling actual template data.

## Coordinates

 - Project: <https://launchpad.net/biometryd>
   - Code: <https://launchpad.net/biometryd>
   - Docs: <http://biometryd.rtfd.io/>
   - Bugs: <https://bugs.launchpad.net/biometryd>
