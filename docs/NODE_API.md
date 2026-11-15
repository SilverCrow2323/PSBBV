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

The PSBBV Core now contains a compile-time built-in registry for the six stable node IDs. It records the Node API version, supported platform mask, category, and declared availability. Host-side tests cover stable IDs, uniqueness, lookup, planned-state preservation, API-major/minor rejection, platform rejection, and disabled/unavailable/error states.

This is a registry and compatibility gate only. It does not parse external manifests, load node code, grant permissions, or execute jobs. All six registered nodes remain `planned`.

## Runtime strategy — do not skip the hardware reality check

The first Vita milestone should register built-in nodes compiled into the PSBBV application. This is the simplest auditable route for a constrained homebrew target.

External native modules, dynamic loading, scripting runtimes, and process isolation are future options only. They require separate VitaSDK, memory, security, and lifecycle feasibility tests. The descriptor format is designed to leave room for them; it does not claim they work today.

## Acceptance gates

- [x] Built-in registry validates stable IDs and compatibility states in host tests.\n- [ ] Manifest schema validates known-good and rejects malformed descriptors.
- [ ] Duplicate node IDs are rejected deterministically.
- [ ] Unsupported API major versions produce an explicit unavailable state.
- [ ] Missing capabilities do not crash or block Home.
- [ ] Node jobs can be cancelled and report typed terminal results.
- [ ] Suspend/resume and deactivation are tested on the target device.
- [ ] No node can block the frame loop with I/O.

---

**SPDW FACTORY** · PSBBV Engineering  
Small contracts. Independent nodes. One reliable host.
