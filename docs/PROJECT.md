# Project: PSBBV — PlayStation Broadband Vita

## Purpose
Build an independent native PlayStation Vita navigator inspired by PSBBN's channel-based experience. PSBBV is a console-style homebrew shell, not a pixel-perfect PSBBN recreation and not merely another flat game launcher.

## Users and use cases
- Vita owners navigating installed games, applications, media, downloads, network and system information.
- Maintainers integrating independently versioned capability nodes.
- Contributors validating native Vita rendering, input, memory use and offline operation.

## Scope and boundaries
Core owns the native shell, rendering, global navigation/focus, lifecycle, node registration, permission policy and compatibility handling. Games, Applications, Media, Network, Downloads and System remain separate repositories/nodes with explicit contracts. A node repository existing does not mean its UI or services are already wired into Core.

Out of scope: commercial game/media payload redistribution, Sony affiliation, a desktop/web UI port as the Vita app, arbitrary dynamic plugin loading, and unsupported privileged system changes.

## Target and architecture
- PlayStation Vita / PlayStation TV, native VitaSDK toolchain, 960×544 display.
- Core shell and renderer with controller/touch input.
- Versioned node manifest/API; built-in nodes first.
- Offline-capable navigation and local state; network-dependent capabilities remain optional.
- Host contract tests, VitaSDK build/VPK inspection and physical Vita tests are separate gates.

## Federated node ownership
- Games: installed-title discovery, catalogue and launch boundary.
- Applications: application catalogue and confirmed package operations.
- Media: local/remote media catalogue and playback.
- Network: connectivity status and secure request adapters.
- Downloads: transfer queues and integrity verification.
- System: preferences, diagnostics and safe cache operations.

## Dependencies and integration
VitaSDK and documented Vita system APIs. Each node consumes the versioned Core contract; pinned schema copies must be reconciled when Core changes. No Python runtime or third-party dynamic loader is assumed on Vita.

## Safety, accessibility and localization
Treat metadata as untrusted, validate paths, bound reads and allocations, keep operations cancellable and require confirmation for destructive actions. Keep core navigation useful offline, focus order predictable, text readable and UI controller-first. UI and technical docs are English.

## Verified baseline
The default branch currently provides the product definition and federated repository map. Native shell and node implementation work is present in separate open PRs and is not treated as merged until it lands. Physical Vita validation is not claimed.
