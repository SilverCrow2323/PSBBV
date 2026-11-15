# PSBBV Product Brief

## Product definition

PSBBV (PlayStation Broadband Vita) is an independent, native PlayStation Vita homebrew application. It adapts the channel-oriented navigation idea of PlayStation Broadband Navigator to the Vita's 960 × 544 screen, physical controls and touch input.

PSBBV is an application, not a replacement shell, firmware modification, or promise of recreating PSBBN exactly.

## Product goals

1. Make the Vita feel like a cohesive connected entertainment device.
2. Organize experiences into recognizable channels rather than a flat list of icons.
3. Keep navigation responsive and understandable with buttons or touch.
4. Make core navigation and locally available content useful offline.
5. Clearly distinguish working features, unavailable services and future concepts.
6. Keep memory, storage, startup time and frame pacing appropriate for Vita hardware.

## Non-goals for the first milestone

- Replacing the Vita LiveArea or system shell.
- Reimplementing Sony network services or depending on unavailable proprietary APIs.
- Shipping copyrighted games, media, firmware or service credentials.
- Presenting mock channel screens as functional services.
- Building a general-purpose web browser before the navigation foundation is stable.

## First milestone: native navigation shell

The first milestone is complete only when the application:

- Builds reproducibly into an installable VPK using the documented VitaSDK toolchain.
- Starts and exits cleanly on a real Vita.
- Renders a stable 960 × 544 home screen.
- Supports deterministic D-pad navigation and Cross/ Circle actions.
- Supports touch selection without breaking physical input.
- Exposes clear placeholder states for channels not yet implemented.
- Handles suspend/resume and application exit without leaving rendering resources active.
- Has a documented manual hardware test checklist.

## Initial channel model

The channel registry should be data-driven. Each entry has a stable identifier, display title, icon/art reference, availability state and activation route.

Suggested initial categories:

- Home: landing page, recent activity and quick access.
- Games: locally installed or user-provided game shortcuts, subject to platform APIs.
- Media: local media entry point; no unsupported codec or streaming promises.
- Network: connectivity status and future network experiences.
- Library: locally available content and favourites.
- Settings: application preferences, input and appearance.

Only channels with implemented handlers may be marked available. Other entries must use an explicit planned or unavailable state.

## Interaction contract

- D-pad: move focus through visible navigation items.
- Cross: activate the focused item.
- Circle: return to the previous view or close a transient panel.
- Start: open the application menu; it must not unexpectedly terminate the app.
- Select: reserved for contextual actions where useful.
- Touch: tap to focus/activate and provide visual feedback.
- A visible focus indicator must always identify the current selection.

The exact button policy is versioned in the input contract before channel-specific shortcuts are added.

## Visual direction

Use the Vita's native 960 × 544 canvas as a deliberate composition, not a scaled desktop layout. The visual language should reference PSBBN's channel identity while remaining its own product: restrained gradients, confident typography, clear tile hierarchy, purposeful motion and strong focus states.

The proposed PSBBN-to-PSBBV mark is a direction, not permission to reuse Sony artwork or imply affiliation. Final artwork must be original.

## Technical direction

- C++17 and VitaSDK.
- CMake-based reproducible builds.
- A small native rendering layer, with dependencies justified by measured needs.
- Explicit application, navigation, input, channel and persistence boundaries.
- No network requirement in the home-screen render path.
- Resource ownership and lifecycle behavior must be testable and documented.

## Release discipline

A successful cross-build is not a hardware release. Public releases require a reproducible VPK, recorded toolchain version, clean startup/exit, input verification, suspend/resume checks, and a tested install/update/uninstall path. Do not publish a release while the VPK title ID remains provisional.
