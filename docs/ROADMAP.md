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
- [x] Front touchscreen navigation.
- [x] Application menu with About and Exit actions.
- [x] Branded Home view and clearly labelled channel placeholders.
- [x] Initial SVG identity and Vita LiveArea package resources.
- [x] GitHub Actions VPK build workflow.
- [x] Stable typed IDs and availability states for all six channels.
- [x] Centralized UI state for Home, Channel, About, and Menu views.
- [x] Edge-triggered physical-button input; held buttons do not repeatedly activate.
- [x] Restore the underlying view when closing the application menu.
- [x] Touch activation is edge-triggered to avoid repeated actions while a finger remains down.
- [x] Host tests for focus movement, menu navigation, return paths, and clean-exit requests.
- [x] ✅ Confirm a green CI VPK build (Vita Build #83, VitaSDK 2026.08, VPK artifact inspected).
- [ ] 🟠 Install and test the VPK on a physical Vita.
- [ ] 🟠 Verify suspend/resume and resource recovery.

## Phase 0.5 — Define the node ecosystem

- [x] Draft the versioned Node API and lifecycle contract.
- [x] Add a machine-readable node manifest schema.
- [x] Document the independent PSBBV Media product and Kodi compatibility strategy.
- [x] Validate manifest schema JSON syntax in CI.
- [x] Add standards-compliant JSON Schema validation and positive/negative manifest fixtures.
- [ ] Implement host-side manifest parsing and duplicate-ID rejection.
- [x] Register six built-in node identities with API/platform compatibility checks and explicit effective availability.
- [x] Establish separate Media, Games, Applications, Network, Downloads and System repositories with provisional manifests and pinned schema-validation CI.
- [x] Add a central ecosystem ownership and integration map.
- [x] Bind Home channels to stable Node IDs and derive channel status from effective registry availability.
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
- [ ] Safe local content discovery.
- [ ] Empty, loading, unsupported, and error states.
- [ ] Sorting, filtering, and recently used views.
- [ ] Read-only discovery before adding launch or management flows.

## Phase 3 — Build the channels

- [ ] 🎮 Games
- [ ] 🧰 Applications
- [ ] 🌐 Network
- [ ] 🎵 Media — see [PSBBV Media Blueprint](MEDIA_NODE_BLUEPRINT.md)
- [ ] ⬇️ Downloads
- [ ] ⚙️ System

Every channel needs a real data source, a failure model, and useful offline behavior before it gets a READY label.

## Phase 4 — Ship it properly

- [ ] Final production logo and complete LiveArea artwork.
- [ ] Settings and persistent preferences.
- [ ] Localization infrastructure; English remains the initial language.
- [ ] Compatibility and performance testing on hardware.
- [ ] Confirm the final VPK title ID.
- [ ] Release notes, checksums, and reproducible package build.

## 🧨 The release rule

A cross-build is not a hardware release. No version is called stable until the VPK installs and launches on real hardware, core navigation is tested, suspend/resume is checked, and CI is green.

---

**SPDW FACTORY** · PSBBV Roadmap  
Progress you can verify. Not just boxes you can tick.
