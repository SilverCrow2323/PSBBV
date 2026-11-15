<div align="center">
<img src="assets/spdw-factory-mark.svg" alt="SPDW Factory" width="160">

**SPDW FACTORY / FIELD NOTES**

### 🧱 PSBBV Architecture

*Runtime layers, ownership and frame-loop rules.*

[OPEN THE VISUAL GUIDE ↗](architecture.html) · [PROJECT HOME](index.html)
</div>

---


## Status

This document defines the intended architecture for the foundation milestone. It is a design contract, not a claim that all listed modules already exist in source code.

## Runtime layers

```text
Vita OS / VitaSDK
       |
Application lifecycle
       |
Input snapshot ---- Touch input
       |
Navigation state machine
       |
Channel registry + channel views
       |
UI composition / focus / transitions
       |
Rendering backend
       |
Frame presentation
```

## Module responsibilities

### Application lifecycle

Owns initialization, main-loop pacing, suspend/resume handling, orderly shutdown and cleanup. Platform calls stay behind a narrow boundary so they can be reviewed independently from UI behavior.

### Input

Samples physical controls once per frame and converts platform-specific button bits into semantic actions. UI code consumes semantic actions, not raw Vita button masks. Touch input is normalized into the same focus/activation model.

### Navigation

Owns the current route, focused item, back stack and transition state. It must define behavior for empty channel lists, unavailable channels and repeated activation. Navigation state must not be hidden inside rendering code.

### Channel registry

Stores stable channel IDs and metadata. Availability is explicit: implemented, planned, unavailable or disabled. A channel marked implemented must have a registered activation handler.

### Views and rendering

Views describe what to draw and where interaction targets are. Rendering must not perform network requests, filesystem scans or blocking platform operations. The first milestone may use a deliberately small renderer.

### Persistence

Stores only user preferences needed by the first milestone, using versioned data and safe defaults. Corrupt or missing settings must fall back to defaults without preventing startup.

## Frame-loop contract

1. Sample input.
2. Update navigation and interaction state.
3. Update animations using elapsed time, not assumed frame count.
4. Render the current view.
5. Present the frame.
6. Process exit/suspend requests through the lifecycle owner.

No blocking network or storage operation belongs in this loop.

## Error and fallback behavior

- Missing optional artwork uses a built-in placeholder.
- Unknown channel IDs return to Home and produce a diagnostic.
- Invalid settings are ignored and defaults are restored safely.
- An unavailable service displays an honest explanation and never a fake success state.
- A failed optional subsystem must not prevent Home from opening.

## Quality gates

- Build: reproducible VitaSDK/CMake build and VPK generation.
- Static: warning-clean project code where practical; no unchecked narrowing or hidden global ownership.
- Navigation: deterministic focus movement, activation and back behavior.
- Lifecycle: clean start, exit and suspend/resume.
- Performance: no blocking I/O in the frame loop; measure frame pacing on device.
- Product truth: UI availability labels match actual handlers.

## Deferred decisions

The following must be decided from working prototypes rather than guessed now:

- Final font and asset pipeline.
- Persistent storage format and migration policy.
- Network stack and TLS dependency.
- Media playback libraries and supported formats.
- Background service model.
- Final VPK title ID and production signing/release process.

## Architecture change rule

A new subsystem must state its user-visible purpose, lifecycle owner, storage/network needs, failure behavior and hardware cost before it is added. Avoid introducing abstractions that have no second real use.
