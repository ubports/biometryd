# Manual Test Plan

This section lists manual test cases that should be executed prior to landing. The test cases exercise the main functionality and aim to guarantee a baseline level of functionality that should not regress across releases. 

Please note that individual landings might require specific testing steps in addition to the ones listed here.

We assume that testers use a freshly bootstrapped device.

## Fingerprint Reader With Guidance

The following section provides testing instructions for fingerprint
readers featuring guided enrollment processes.

### Enrolling a New Template

  - Boot the phone
  - Unlock the greeter/complete the wizard
  - Start "System Settings"
  - Switch to the "Security & Privacy" page
  - Select "Fingerprint ID"
  - Select "Add Fingerprint"
      + Enroll a new template according to the onscreen instructions.
      + Make sure that feedback given during enrollment is meaningful and reasonable.
      + After completion, check if the list of enrolled fingerprints has grown by 1.
  - Select the recently enrolled fingerprint and rename it:
      + Ensure that the name of the fingerprint is persistent across restarts of "System Settings"

### Identifying With A Fingerprint

  - In "System Settings", choose Fingerprint ID as lock security.
  - Lock the screen.
  - Wake up the phone by pressing the power button.
  - Try to identify with your previously enrolled fingerprint.
  - Lock the screen again.
  - Wake up with the home button.
  - Try to identify with your previously enrolled fingerprint.
  - Lock the screen again.
  - Wake up the screen and try to identify with a finger that hasn't been enrolled previously. The attempts should fail and the device should fall back to your passcode.

### Removing a Previously Enrolled Template

  - Start "System Settings"
  - Switch to "Security & Privacy" page
  - Remove at least one enrolled fingerprint
      + Make sure that the fingerprint is removed from the list
  - Lock the screen and try to identify with the fingerprint. The attemtps should fail.
