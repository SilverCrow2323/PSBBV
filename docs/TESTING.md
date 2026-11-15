<div align="center">

![PSBBV / SPDW Factory](../assets/branding/psbbv-docs-banner.svg)

# 🧪 TAKE IT TO THE VITA

**CI checks the build. This checklist checks the actual handheld.**

<sub>SPDW FACTORY · HARDWARE LAB / 005</sub>

</div>

> **Important:** a successful cross-build does not prove that the app installs, launches, or behaves correctly on a Vita. This is where the hardware gets a vote.

## 📦 Build and package

- [ ] Confirm the Vita Build workflow completed successfully.
- [ ] Download the PSBBV-VPK artifact from that successful run.
- [ ] Verify the VPK installs without an error.
- [ ] Check the LiveArea title, icon, background, and startup artwork.
- [ ] Confirm SCBV00001 does not conflict with an installed title before release.

## 🚀 Startup and shutdown

- [ ] Launch PSBBV from LiveArea.
- [ ] Confirm the screen uses the expected 960 × 544 landscape canvas.
- [ ] Check the wordmark and all text at handheld distance.
- [ ] Open the application menu with Start.
- [ ] Navigate to Exit Application and confirm clean shutdown.
- [ ] Relaunch and repeat.

## 🎮 Physical controls

- [ ] Move down through all six channels.
- [ ] Move up through all six channels.
- [ ] Confirm selection wraps at both ends.
- [ ] Open a channel using Cross.
- [ ] Return to Home using Circle.
- [ ] Open and close the application menu.
- [ ] Open About PSBBV and return with Circle and Cross.

## 👆 Front touchscreen

- [ ] Tap each channel row and confirm the correct channel opens.
- [ ] Tap each Home shortcut card.
- [ ] Tap each application-menu option.
- [ ] Tap outside the menu and confirm it dismisses without activating an underlying item.
- [ ] Hold a finger on the panel and confirm there is no repeated activation.

## 🔋 Lifecycle and performance

- [ ] Suspend the Vita while PSBBV is running.
- [ ] Resume and confirm rendering and input continue.
- [ ] Repeat suspend/resume and check for visual corruption.
- [ ] Observe frame pacing while navigating.
- [ ] Watch for abnormal memory growth during extended use.
- [ ] Confirm exit works after touch sampling has been enabled.

## 📝 Test record

| Field | Record |
|---|---|
| Vita model | |
| Firmware / homebrew environment | |
| VPK SHA-256 | |
| Test date | |
| Result | |
| Notes / issues | |

Do not mark a test as passed from memory. Record what was tested, on which device, and with which build.

---

**SPDW FACTORY** · PSBBV Hardware Lab  
Looks good in CI? Great. Now make it survive the console.
