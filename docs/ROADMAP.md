<div align="center">
<img src="assets/spdw-factory-mark.svg" alt="SPDW Factory" width="160">

**SPDW FACTORY / FIELD NOTES**

### 🛠️ PSBBV Roadmap

*Build milestones and release gates.*

[OPEN THE VISUAL GUIDE ↗](roadmap.html) · [PROJECT HOME](index.html)
</div>

---


This roadmap describes intended work. A milestone is complete only when its acceptance criteria are met and recorded against the repository.

## Phase 0 — Native foundation

- [x] Establish the product brief and architecture contract.
- [x] Add the VitaSDK/CMake application target.
- [x] Add a libvita2d rendering loop and system font baseline.
- [x] Add deterministic physical-button navigation.
- [x] Add front touchscreen navigation.
- [x] Add an application menu with About and Exit actions.
- [x] Add a branded home screen and clearly marked planned channel views.
- [x] Add initial SVG branding and Vita LiveArea package resources.
- [x] Add a GitHub Actions VPK build workflow.
- [ ] Confirm a successful CI package build.
- [ ] Install and test the VPK on physical Vita hardware.
- [ ] Verify suspend/resume behavior and resource recovery on hardware.

## Phase 1 — UI foundation

- [ ] Formalize reusable panels, cards, labels, focus states, and layout primitives.
- [ ] Add consistent transitions and focus restoration.
- [ ] Add configurable confirm/back button mapping.
- [ ] Add rear-touch behavior only where it has a clear interaction purpose.
- [ ] Establish a lightweight asset and texture cache.
- [ ] Validate frame pacing and memory usage on hardware.

## Phase 2 — Local content model

- [ ] Define typed metadata models for games and applications.
- [ ] Implement safe local content discovery.
- [ ] Build empty, loading, unsupported, and error states.
- [ ] Add sorting, filtering, and recently used views.
- [ ] Keep discovery read-only until safe launch and management flows are designed.

## Phase 3 — Channel modules

- [ ] Games
- [ ] Applications
- [ ] Network
- [ ] Media
- [ ] Downloads
- [ ] System

Each channel must have a documented data source, error model, and offline behavior before it is considered implemented.

## Phase 4 — Product polish and release

- [ ] Finalize the production logo and complete LiveArea artwork.
- [ ] Add settings and persistent preferences.
- [ ] Add localization infrastructure while keeping English as the initial language.
- [ ] Run hardware compatibility and performance tests.
- [ ] Confirm the final VPK title ID.
- [ ] Publish release notes, checksums, and a reproducible VPK build.

## Release discipline

A cross-build is not a hardware release. No version should be called stable until the VPK installs and launches on real hardware, the primary navigation paths are tested, and CI is green.
