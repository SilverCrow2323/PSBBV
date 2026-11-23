<div align="center">

![PSBBV / SPDW Factory](../assets/branding/psbbv-docs-banner.svg)

# 🛣️ THE ROADMAP

**What is real. What is next. What is still a nice idea.**

<sub>SPDW FACTORY · PROJECT TRACKER / 004</sub>

</div>

## 🚦 Status legend

| Marker | Meaning |
|---|---|
| ✅ | Implemented in the repository |
| 🟡 | Work started; verification or polish remains |
| 🔵 | Planned; not implemented |
| 🟠 | Needs target-hardware testing |
| ⛔ | Blocked by a named issue |

## Phase 0 — Native foundation

- [x] Product brief and architecture contract.
- [x] VitaSDK/CMake application target.
- [x] libvita2d render loop and system font baseline.
- [x] D-pad, Cross, Circle, and Start navigation.
- [x] Front touchscreen application-menu interaction.
- [x] Vertical left-stick navigation with a 48-unit deadzone and the shared D-pad repeat path (host-tested; physical calibration pending).
- [x] Application menu with About and Exit actions.
- [x] Branded Home view and a non-interactive list of planned channel identities.
- [x] Initial SVG identity and Vita LiveArea package resources.
- [x] GitHub Actions VPK build workflow.
- [x] Stable typed IDs and availability states for all six channels.
- [x] Centralized UI state for Home, Channel, About, and Menu views.
- [x] Edge-triggered physical-button input; held buttons do not repeatedly activate.
- [x] Restore the underlying view when closing the application menu.
- [x] Touch activation is edge-triggered to avoid repeated actions while a finger remains down.
- [x] Host tests for focus movement, menu navigation, return paths, and clean-exit requests.
- [x] Frame-based D-pad repeat with deterministic host coverage.
- [x] Automated VPK structural, path-safety, LiveArea and SFO identity verification.
- [x] Exact-head CI VPK build: [run #44895313787](https://github.com/SilverCrow2323/PSBBV/actions/runs/44895313787).
- [ ] 🟠 Install and test the exact candidate VPK on a physical Vita.
- [ ] 🟠 Verify suspend/resume, resource recovery, frame pacing and clean shutdown.

## Phase 0.5 — Define the node ecosystem

- [x] Draft the versioned Node API and lifecycle contract.
- [x] Add a machine-readable node manifest schema.
- [x] Document the independent PSBBV Media product and Kodi compatibility strategy.
- [x] Validate manifest schema JSON syntax in CI.
- [x] Add standards-compliant JSON Schema validation and positive/negative manifest fixtures.
- [x] Implement bounded host-side manifest parsing and built-in identity gating.
- [x] Implement and test the pure node lifecycle transition policy.
- [x] Reject duplicate/unknown top-level manifest fields and malformed or unsupported descriptors.
- [x] Register six built-in node identities with API/platform compatibility checks and explicit effective availability.
- [x] Establish separate Media, Games, Applications, Network, Downloads and System repositories with provisional manifests and pinned schema-validation CI.
- [x] Add a central ecosystem ownership and integration map.
- [x] Bind Home channels to stable Node IDs and derive channel status from effective registry availability.
- [x] Implement and host-test allocation-free node-job states, monotonic progress, cancellation, and typed terminal results.
- [ ] Connect job policy to a real host scheduler and UI-thread result delivery.
- [ ] Verify lifecycle, cancellation, and suspend/resume behavior on Vita.

## Phase 1 — Make the UI feel finished

- [ ] Reusable panels, cards, labels, focus states, and layout primitives.
- [ ] Directional focus movement across Home cards and channel content.
- [ ] Consistent transitions and focus restoration.
- [ ] Configurable confirm/back button mapping.
- [ ] Rear-touch behavior only where it has a clear purpose.
- [ ] Lightweight asset and texture cache.
- [ ] Frame pacing and memory measurements on hardware.

## Phase 2 — Give local content a proper home

- [ ] Typed metadata models for games and applications.
- [x] Read-only installed Games discovery from the fixed `ux0:/app` root, with bounded PARAM.SFO parsing on a worker thread.
- [x] Games workspace loading, empty, and failure states; no channel is promoted to READY by discovery alone.
- [x] Explicit Cross-to-retry flow after a failed Games scan, with worker join/reset ordering and host-tested lifecycle transitions.
- [x] Five-row Games catalogue browser with title sorting, D-pad wraparound, viewport scrolling, and touch selection.
- [x] Keep discovery read-only until launch and return-to-shell lifecycle are implemented.

The Games discovery code is a source-level foundation only. Games remains PLANNED and non-openable until device-side discovery, catalog behavior, and launch/return lifecycle are verified.

- [ ] Applications discovery and metadata model.
- [ ] Games filtering, favourites, recently used views, and title launch/return lifecycle.

## Phase 3 — Build the channels

- [ ] 🎮 Games
- [ ] 🧰 Applications
- [ ] 🌐 Network
- [ ] 🎵 Media — see [PSBBV Media Blueprint](MEDIA_NODE_BLUEPRINT.md)
- [ ] ⬇️ Downloads
- [ ] ⚙️ System

Every channel needs a real data source, a failure model, and useful offline behavior before it gets a READY label. A PLANNED registry entry is metadata only: it must not open a generic placeholder view or display a fake READY state.

## Phase 4 — Ship it properly

- [ ] Final production logo and complete LiveArea artwork.
- [ ] Settings and persistent preferences.
- [ ] Localization infrastructure; English remains the initial language.
- [ ] Compatibility and performance testing on hardware.
- [ ] Confirm the final VPK Title ID; `SCBV00001` is still provisional.
- [ ] Release notes, checksums, and reproducible package build.
- [ ] Complete the release manifest with exact integrated node revisions.
- [ ] Record physical Vita acceptance for the exact release candidate.

## Phase 4.1 — 0.1.0 release candidate

- [x] Establish Core version `0.1.0` in CMake.
- [x] Build candidate VPK from `dev/foundation` at `6e9cb2c42d113ad6c4db1013cb5102e1edf21cbd`.
- [x] Pass 10/10 host CTest executables and 9/9 VPK verifier unit tests.
- [x] Pass manifest schema validation (one positive fixture, nine negative fixtures).
- [x] Pass VitaSDK 2026.08 cross-build and automated VPK verification.
- [x] Upload a candidate VPK; the current immutable run, artifact ID and archive digest are pinned in the [candidate manifest](releases/v0.1.0-rc.1-manifest.json).
- [x] Prepare candidate notes and a machine-readable, explicitly non-stable release record.
- [ ] 🟠 Install and validate this exact artifact on a physical Vita.
- [ ] Confirm Title ID ownership/non-conflict and final packaging identity.
- [ ] Review and sign off the release manifest and limitations.
- [ ] Only then decide whether to publish a stable `v0.1.0`.

## 🧨 The release rule

A cross-build is not a hardware release. No version is called stable until the VPK installs and launches on real hardware, core shell navigation is tested, suspend/resume and shutdown are checked, the package identity is final, the release manifest is complete, and CI is green.

A release candidate may be shared as a CI artifact for testing. It must not be presented as an official stable release.

---

**SPDW FACTORY** · PSBBV Roadmap  
Progress you can verify. Not just boxes you can tick.
