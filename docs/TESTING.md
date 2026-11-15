<div align="center">
<img src="assets/spdw-factory-mark.svg" alt="SPDW Factory" width="160">

**SPDW FACTORY / FIELD NOTES**

### 🧪 PSBBV Hardware Test Bench

*Manual install, controls, touch and lifecycle checks.*

[OPEN THE VISUAL GUIDE ↗](testing.html) · [PROJECT HOME](index.html)
</div>

---


This checklist is intended for a homebrew-enabled PlayStation Vita. A successful cross-build does not replace device testing.

## Build and package

- [ ] Confirm the Vita Build workflow completed successfully.
- [ ] Download the PSBBV-VPK artifact from the successful workflow run.
- [ ] Verify the VPK installs without an error.
- [ ] Verify the LiveArea title, icon, background, and startup artwork.
- [ ] Confirm the installed application title ID does not conflict with another installed title before release.

## Startup and shutdown

- [ ] Launch PSBBV from LiveArea.
- [ ] Confirm the display fills the expected 960 × 544 landscape canvas.
- [ ] Confirm the wordmark and all text are legible.
- [ ] Open the application menu with Start.
- [ ] Navigate to Exit Application and confirm a clean exit.
- [ ] Relaunch the application and repeat the test.

## Physical controls

- [ ] Move down through all six channels using D-pad Down.
- [ ] Move up through all six channels using D-pad Up.
- [ ] Confirm selection wraps at both ends.
- [ ] Open a channel using Cross.
- [ ] Return to Home using Circle.
- [ ] Open and close the application menu.
- [ ] Open About PSBBV and return with Circle and Cross.

## Front touchscreen

- [ ] Tap each channel row and confirm the correct channel opens.
- [ ] Tap each Home shortcut card and confirm the matching channel opens.
- [ ] Tap each application-menu option.
- [ ] Confirm taps outside the menu dismiss it without triggering an unintended action.
- [ ] Confirm touch input does not repeat while a finger remains on the panel.

## Lifecycle and performance

- [ ] Suspend the Vita while PSBBV is running.
- [ ] Resume and confirm rendering and input continue normally.
- [ ] Check for visual corruption after repeated suspend/resume cycles.
- [ ] Observe frame pacing during idle navigation.
- [ ] Check for abnormal memory growth during extended use.
- [ ] Confirm the application exits cleanly after touch sampling has been enabled.

## Result record

Record the Vita model, firmware environment, homebrew setup, VPK SHA-256, test date, and pass/fail notes when a device test is completed.
