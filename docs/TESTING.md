<div align="center">

![PSBBV / SPDW Factory](../assets/branding/psbbv-docs-banner.svg)

# 🧪 TAKE IT TO THE VITA

**CI checks the build. This checklist checks the actual handheld.**

<sub>SPDW FACTORY · HARDWARE LAB / 005</sub>

</div>

> **Release boundary:** a successful VitaSDK cross-build proves that the toolchain produced a structurally valid VPK. It does not prove installation, launch, controls, rendering, suspend/resume or compatibility on a physical Vita.

## 📦 Exact candidate under test

| Field | Current CI candidate |
|---|---|
| Core source commit | `a0482f126b7e6d00e273a88502bde9263b0c9337` |
| Workflow run | [Vita Build #45008252183](https://github.com/SilverCrow2323/PSBBV/actions/runs/45008252183) |
| Download artifact | [PSBBV-VPK #14747934624](https://github.com/SilverCrow2323/PSBBV/actions/runs/45008252183/artifacts/14747934624) |
| Artifact ZIP size | 151,521 bytes |
| Artifact ZIP SHA-256 | `d43c1506145ddf672dbdcd59189a3becd986d48cd96edb569a3205504c9cc5f0` |
| Extracted VPK SHA-256 | `04fd6227f52cbb38022055cc1aad053ce9a82b5fb4e3f8ea31109f32fcc7a005` |
| SFO identity checked by CI | `SCBV00001` / `00.10` |

The ZIP and VPK digests identify different files. The machine-readable [candidate manifest](releases/v0.1.0-rc.1-manifest.json) is the canonical record and its validator checks that the source commit, workflow, artifact URL, artifact ID and both digests agree.

## 🤖 Automated evidence — exact candidate

- [x] Ten CTest executables passed (10/10).
- [x] Nine VPK verifier unit tests passed.
- [x] Node manifest schema: valid fixture accepted and nine invalid fixtures rejected.
- [x] VitaSDK 2026.08 cross-build completed.
- [x] VPK verifier accepted ZIP/CRC structure, safe paths, required LiveArea assets and SFO identity.
- [x] GitHub Actions uploaded the VPK artifact.
- [x] Release candidate manifest provenance tests reject mixed run/artifact/commit/checksum records.

These are automated CI results. They are not physical-device results.

## 🧭 Shell and navigation — physical Vita

- [ ] Install this exact VPK on the target Vita.
- [ ] Confirm the LiveArea title, icon, background and startup artwork.
- [ ] Confirm the installed package reports Title ID `SCBV00001` and version `00.10`.
- [ ] Confirm the provisional Title ID does not conflict with another installed title.
- [ ] Launch from LiveArea and confirm the expected 960 × 544 landscape canvas.
- [ ] Inspect wordmark, typography, contrast and clipping at handheld distance.
- [ ] Open and close the application menu with Start.
- [ ] Navigate the application menu with D-pad Up/Down and verify wraparound and repeat feel.
- [ ] Activate menu options with Cross and return/close with Circle.
- [ ] Open About PSBBV, return, then open the menu over About and use Resume; verify focus returns to the underlying view.
- [ ] Verify menu touch hitboxes: row interiors activate; the 8 px gaps and outside edges do not.
- [ ] Tap outside the menu and verify it dismisses without activating an underlying item.
- [ ] Hold a finger on a menu row and verify activation is edge-triggered, not repeated.

## 🚪 Channel and node boundary — do not test imaginary features

All six registry entries (Games, Applications, Network, Media, Downloads and System) are currently `PLANNED`. They are metadata, not openable destinations.

- [ ] Confirm planned channel entries are visibly non-interactive and do not open placeholder screens.
- [ ] Confirm no planned channel displays a false READY/connected/working state.
- [ ] Do not mark channel navigation as passed by tapping a planned entry.
- [ ] Do not claim Games is device-validated: its discovery/browser code is compiled, but the channel remains gated and game launching is not implemented.

## 🎮 Games discovery — future hardware gate

Only run this section when the Games channel is deliberately enabled by a later reviewed change. The current candidate must not be reported as passing it.

- [ ] Open Games and scan installed titles under the fixed `ux0:/app` root.
- [ ] Confirm bounded PARAM.SFO parsing and title sorting.
- [ ] Browse beyond the five-row viewport; verify wraparound and touch selection.
- [ ] Verify retry after discovery failure and worker join/reset ordering.
- [ ] Confirm no title launches until launch/return lifecycle is implemented and tested.

## 🔋 Lifecycle, stability and performance — physical Vita

- [ ] Suspend the Vita while PSBBV is running; resume and verify rendering/input recovery.
- [ ] Repeat suspend/resume and check for visual corruption or stale input.
- [ ] Verify clean shutdown from the application menu.
- [ ] Observe frame pacing while navigating.
- [ ] Watch for abnormal memory growth during an extended session.
- [ ] Repeat launch, menu, About, suspend/resume and shutdown after a cold restart.

## 📝 Test record

| Field | Record |
|---|---|
| Vita model | |
| Firmware / homebrew environment | |
| Candidate commit | `a0482f126b7e6d00e273a88502bde9263b0c9337` |
| Artifact ID | `14747934624` |
| Extracted VPK SHA-256 | `04fd6227f52cbb38022055cc1aad053ce9a82b5fb4e3f8ea31109f32fcc7a005` |
| Test date | |
| Result | |
| Notes / issue links | |

Do not mark a test as passed from memory. Record the exact device, environment, package and observed result.

---

**SPDW FACTORY** · PSBBV Hardware Lab  
Looks good in CI? Great. Now make it survive the console.
