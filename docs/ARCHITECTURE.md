<div align="center">

![PSBBV / SPDW Factory](../assets/branding/psbbv-docs-banner.svg)

# 🧠 HOW THE APP FITS TOGETHER

**A small shell, clear boundaries, no mysterious spaghetti.**

<sub>SPDW FACTORY · ENGINEERING / 002</sub>

</div>

> This is the architecture contract for the foundation milestone. It describes the boundaries the code should follow; it is not a claim that every future module already exists.

## 🗺️ The big picture

~~~mermaid
flowchart TD
    OS["Vita OS / VitaSDK"] --> LIFE["Application lifecycle"]
    LIFE --> INPUT["Input snapshot"]
    INPUT --> NAV["Navigation state machine"]
    NAV --> REG["Channel registry"]
    REG --> VIEW["Channel views"]
    VIEW --> UI["UI composition / focus / transitions"]
    UI --> RENDER["Rendering backend"]
    RENDER --> DISPLAY["Frame presentation"]
    TOUCH["Front touch"] --> INPUT
    PAD["Physical controls"] --> INPUT
~~~

## 🧩 The node ecosystem

PSBBV is the host; domain workspaces can evolve as independent nodes. The shared contract, manifest format, lifecycle, compatibility rules, and permission model are defined in [The Node Contract](NODE_API.md). The first Vita implementation should use built-in nodes compiled into the host. Dynamic modules and scripting runtimes remain feasibility work, not current capabilities.

The proposed media product is specified separately in [PSBBV Media Blueprint](MEDIA_NODE_BLUEPRINT.md). It is a design target only; the Media channel remains a planned placeholder until a real implementation is integrated.

## 🧱 The moving parts

### 1. Application lifecycle

Owns startup, the main loop, suspend/resume handling, shutdown, and cleanup. Platform calls stay behind a narrow boundary so UI behavior can be reviewed without digging through system glue.

### 2. Input

Samples physical controls once per frame and converts raw Vita button bits into semantic actions. UI code should consume actions such as MoveUp, Activate, Back, and OpenMenu — not platform-specific bitmasks.

Front-touch input joins the same focus and activation model. One action model, two ways to use it.

### 3. Navigation

Owns the current route, focused item, back stack, and transition state.

It must define behavior for:

- empty channel lists;
- unavailable channels;
- repeated activation;
- unknown route IDs;
- returning from a child view without losing useful focus.

Navigation state does not belong inside drawing code. That's how a three-line button action becomes a week-long archaeology dig.

### 4. Channel registry

Stores stable channel IDs and metadata. Availability is explicit:

| State | Meaning |
|---|---|
| 🟢 Implemented | A real activation handler is registered |
| 🔵 Planned | Designed, not built |
| ⚪ Unavailable | Known, but cannot run in this environment |
| ⛔ Disabled | Intentionally switched off |

A channel marked Implemented must have a real handler. No handler, no green label.

### 5. Views and rendering

Views describe what to draw and where interaction targets live. Rendering must not perform network requests, filesystem scans, or blocking platform operations.

The first renderer can stay deliberately small. We add heavier machinery only when a real screen needs it.

### 6. Persistence

Stores only preferences the first milestone actually needs. Data must be versioned and have safe defaults. Missing or corrupt settings must not stop Home from opening.

## 🔁 The frame loop

~~~mermaid
sequenceDiagram
    participant Vita as Vita OS
    participant Input as Input
    participant App as App state
    participant View as View
    participant GPU as Renderer
    loop Every frame
      Vita->>Input: Sample controls and touch
      Input->>App: Semantic actions
      App->>App: Update navigation and state
      App->>View: Current view state
      View->>GPU: Draw commands
      GPU->>Vita: Present frame
    end
~~~

1. Sample input.
2. Update navigation and interaction state.
3. Update animation using elapsed time, not assumed frame count.
4. Render the current view.
5. Present the frame.
6. Process exit or suspend requests through the lifecycle owner.

**No blocking network or storage work belongs in this loop.** The Vita has enough to do already.

## 🧯 Failure behavior

| Problem | Expected response |
|---|---|
| Missing optional artwork | Show a built-in fallback |
| Unknown channel ID | Return to Home and log a diagnostic |
| Invalid settings | Ignore them and restore safe defaults |
| Offline service | Explain the offline state; never fake success |
| Optional subsystem fails | Keep Home usable |

## 🧪 Quality gates

- **Build:** repeatable VitaSDK/CMake build and VPK generation.
- **Static checks:** project code should be warning-clean where practical.
- **Navigation:** deterministic focus, activation, and back behavior.
- **Lifecycle:** clean start, exit, and suspend/resume.
- **Performance:** no blocking I/O in the frame loop; measure on device.
- **Product truth:** availability labels must match actual handlers.

## 🧩 Node integration boundary

- The host owns lifecycle, input sampling, rendering, navigation, and shutdown.
- Nodes own domain data and asynchronous jobs; they never block the frame loop.
- Node identity is stable and independent of display names.
- Manifest declarations are validated, but never treated as proof of implementation or permission.
- Effective availability is determined by the host after compatibility and initialization checks.

See [NODE_API.md](NODE_API.md) for the versioned contract and [MEDIA_NODE_BLUEPRINT.md](MEDIA_NODE_BLUEPRINT.md) for the Media product plan.

## 🧊 Decisions we are not guessing yet

These need a working prototype, not a confident-sounding paragraph:

- Final font and asset pipeline.
- Persistent storage format and migrations.
- Network stack and TLS dependency.
- Media playback libraries and supported formats.
- Background service model.
- Final VPK title ID and release process.

## 🛠️ Architecture change rule

A new subsystem must explain its user-visible purpose, lifecycle owner, storage/network needs, failure behavior, and hardware cost. No abstraction gets added just because its name sounds expensive.

---

**SPDW FACTORY** · PSBBV Engineering  
Clear boundaries. Fewer mysteries. Better builds.
