<div align="center">

![PSBBV / SPDW Factory](../assets/branding/psbbv-docs-banner.svg)

# 🧩 THE NODE CONTRACT

**One navigator. Independent workspaces. A shared set of rules.**

<sub>SPDW FACTORY · ENGINEERING / 006</sub>

</div>

> This document defines the proposed integration boundary for PSBBV nodes. It is an architecture contract, not a claim that a dynamic node loader, process sandbox, or third-party SDK already exists.

## Why nodes are separate

PSBBV owns the Vita application lifecycle, Home navigation, input routing, shared visual language, and node discovery. A node owns one product domain: its screens, data model, services, and domain-specific error handling.

A node must not take over the Vita main loop or draw directly from a background worker. The host remains responsible for frame pacing, rendering, input sampling, suspend/resume, and shutdown.

## Stable identity and manifest

Every node has a permanent reverse-domain-style ID. Renaming a display title must not change the ID.

Example IDs:

| Node | ID |
|---|---|
| Games | `factory.spdw.psbbv.games` |
| Applications | `factory.spdw.psbbv.applications` |
| Network | `factory.spdw.psbbv.network` |
| Media | `factory.spdw.psbbv.media` |
| Downloads | `factory.spdw.psbbv.downloads` |
| System | `factory.spdw.psbbv.system` |

Each package declares a `psbbv-node.json` descriptor validated against `schemas/psbbv-node-manifest.schema.json`.

The descriptor carries identity, semantic version, Node API compatibility, supported platforms, requested capabilities, permissions, and declared availability. It does not itself grant permissions or make an implementation executable.

## Node lifecycle

The host owns the lifecycle and invokes node handlers through a narrow adapter:

1. **Discover** — read and validate the descriptor.
2. **Check** — verify API compatibility, platform support, availability, and permission policy.
3. **Prepare** — allocate node-owned state without blocking the frame loop.
4. **Activate** — create the node workspace and register its navigation route.
5. **Suspend / Resume** — release or restore transient resources safely.
6. **Deactivate** — stop jobs, release resources, and return control to the host.
7. **Report** — expose a structured state and diagnostic when a step fails.

A node may be unavailable without making Home unavailable. Failed preparation must leave navigation usable.

## API surface: version 1 proposal

The first API version is intentionally small:

| Contract | Responsibility |
|---|---|
| `NodeDescriptor` | Validated identity and compatibility metadata |
| `NodeContext` | Read-only host services, storage namespace, logger, capability grants |
| `NodeLifecycle` | prepare, activate, suspend, resume, deactivate |
| `NodeView` | Node-owned view model rendered by host-approved UI primitives |
| `NodeAction` | Typed user intent; never a raw platform button bitmask |
| `NodeJob` | Cancellable asynchronous work with progress and terminal result |
| `NodeResult` | Success or typed error with a safe user-facing message |
| `NodeEvent` | Versioned event for host/node communication |

All potentially slow work is asynchronous. Jobs must support cancellation and must not mutate UI state from worker threads. The host marshals results back to the UI thread.

The Vita build currently uses C++17 with exceptions and RTTI disabled. The first implementation should therefore use explicit result/status values, bounded data structures, and a host-defined adapter. Do not expose C++ standard-library ABI types across independently compiled binary boundaries.

## Availability states

| State | Meaning |
|---|---|
| `planned` | Designed, no usable implementation |
| `ready` | A real implementation passed its declared checks |
| `unavailable` | Known but unsupported in this environment |
| `disabled` | Intentionally disabled by the user or host |
| `error` | Implementation exists but failed to initialize or operate |

The UI must show the reason for `unavailable` and `error`. A manifest cannot self-certify a node as ready; the host derives effective availability from validation and runtime results.

## Permissions and trust

Capabilities are declarations, not automatic grants. The host grants only the minimum required permissions and records the effective grant.

Initial permission vocabulary:

- `media.read` — read user-selected media locations.
- `media.write` — write metadata or artwork only where explicitly approved.
- `network.client` — make outbound network requests.
- `storage.private` — use the node's private data namespace.
- `storage.shared.read` — read explicitly exposed shared catalog data.
- `input.global` — receive host-routed semantic input while active.

No node receives unrestricted filesystem or network access by default. Credentials and tokens must never be written to logs or manifests.

## Compatibility and versioning

- Node API versions use `major.minor`.
- A host accepts a node only when the declared major version matches and the host supports the requested minor contract.
- Adding optional fields or capabilities is a minor change.
- Removing or changing the meaning of a field/action is a major change.
- Unknown optional manifest fields may be ignored; unknown required capabilities must cause a clear unavailable state.
- Every event and persisted node-owned data format must be versioned.

## Current implementation slice

The PSBBV Core now contains a compile-time built-in registry for the six stable node IDs. The six Home channels carry matching stable Node IDs, and the channel detail view derives its status from the registry's effective API/platform availability result. It records the Node API version, supported platform mask, category, and declared availability. Host-side tests cover stable IDs, uniqueness, lookup, planned-state preservation, API-major/minor rejection, platform rejection, and disabled/unavailable/error states.

The Core now also has a bounded C++ manifest parser for the metadata subset used by the Vita built-in host. It rejects oversized input, malformed structure, duplicate top-level fields, unknown fields, missing required fields, unsupported API values and unsupported runtime/platform declarations. A separate identity gate accepts only manifests matching a node already compiled into the host, including its stable ID, category, API version and platform. This is not a dynamic loader: it cannot register arbitrary third-party code, grant permissions, or execute jobs. All six registered nodes remain `planned`.

A portable node-job state policy now models queued/running/succeeded/failed/cancelled jobs, enforces monotonic 0–99 progress, reserves 100% for success, and provides bounded typed terminal results. Cancellation is deterministic for queued and running jobs. This is a state policy only: it does not create threads, schedule work, interrupt an in-flight I/O operation, or marshal callbacks.\n\nA pure lifecycle transition policy now models `discovered → validated → prepared → active ↔ suspended`, with explicit failure and terminal deactivation states. Invalid skips and attempts to restart failed/deactivated nodes are rejected in host tests. This is transition policy only; it does not invoke node callbacks, allocate resources, or schedule asynchronous work.

## Runtime strategy — do not skip the hardware reality check

The first Vita milestone should register built-in nodes compiled into the PSBBV application. This is the simplest auditable route for a constrained homebrew target.

External native modules, dynamic loading, scripting runtimes, and process isolation are future options only. They require separate VitaSDK, memory, security, and lifecycle feasibility tests. The descriptor format is designed to leave room for them; it does not claim they work today.

## Acceptance gates

- [x] Built-in registry validates stable IDs and compatibility states in host tests.\n- [x] Manifest schema validates known-good and rejects malformed descriptors.
- [x] Duplicate manifest fields are rejected deterministically; duplicate compiled-in node IDs are covered by registry tests.
- [x] Manifest identity must match an existing compiled-in node before it passes the built-in gate.
- [ ] Unsupported API major versions produce an explicit unavailable state.
- [ ] Missing capabilities do not crash or block Home.
- [x] Pure lifecycle transition policy rejects invalid activation, resume and terminal-state transitions.
- [x] Portable node-job state policy supports cancellation, monotonic progress, and typed success/error/cancelled terminal results (host tests).
- [ ] Suspend/resume and deactivation are tested on the target device.
- [ ] No node can block the frame loop with I/O.

---

**SPDW FACTORY** · PSBBV Engineering  
Small contracts. Independent nodes. One reliable host.

## First external node build integration

The Vita build now pins the independently maintained `PSBBV-Games` repository at reviewed commit `a2f331dd58e288fd1d9e47b4502d39e54bfbae34` using CMake FetchContent. The production catalogue, bounded PARAM.SFO parser, transactional discovery coordinator and user-mode Vita filesystem provider are compiled into the Core build and linked through `psbbv_games_vita_adapter`.

For local development, pass `-DPSBBV_GAMES_SOURCE_DIR=/path/to/PSBBV-Games` to use a working checkout instead of the pinned remote revision. The Core build disables the node's host test executables while the standalone Games repository continues to run them in its own CI.

This is build integration, not a claim that the Games channel is ready. The Core UI does not yet drive an asynchronous discovery job or present the discovered catalogue; the node remains `planned` until that lifecycle and physical Vita validation are complete.
