# PSBBV

**PlayStation Broadband Vita**  
*The next generation of PlayStation navigation.*

PSBBV is an independent, native homebrew navigator for PlayStation Vita, inspired by the channel-based experience and visual identity of PlayStation Broadband Navigator (PSBBN).

The goal is not to recreate PSBBN pixel-for-pixel or to build another conventional game launcher. PSBBV aims to bring that broader, console-like navigation concept to Vita with a refined interface, modern interaction design, and useful system-aware features.

> PSBBV is an independent homebrew project. It is not affiliated with, endorsed by, or sponsored by Sony Interactive Entertainment.

## Project status

PSBBV is in its foundation stage. The repository currently establishes the native application architecture, rendering baseline, input loop, design direction, and Vita build pipeline.

The first interface milestone targets the Vita's native **960 × 544** display and provides a navigable set of channel entry points. Channel functionality will be implemented incrementally; entries that are not implemented yet are presentation scaffolding, not claims of working services.

## Core principles

- **Native first:** a homebrew application built for PlayStation Vita, not a desktop or web UI port.
- **PSBBN heritage:** channel-oriented navigation, contextual content, and a cohesive digital environment.
- **Modern execution:** clear hierarchy, restrained motion, legible typography, and deliberate spacing.
- **Hardware-aware:** responsive controls, modest memory use, and predictable frame pacing.
- **Offline-capable:** core navigation must remain useful without a network connection.
- **Honest functionality:** unfinished features must be clearly identified and must never masquerade as working services.

## Technology

- C++17
- VitaSDK (supported release channel: 2026.08)
- CMake
- libvita2d for GPU-accelerated 2D rendering
- Vita system APIs for lifecycle and controller input

The renderer is intentionally small. The project will add dependencies only when they solve a concrete product requirement.

## Build

### 1. Install VitaSDK

Follow the official [VitaSDK getting started guide](https://vitasdk.org/). On Linux, the release-channel installation flow is:

~~~sh
git clone https://github.com/vitasdk/vdpm
cd vdpm
VITASDK_CHANNEL=2026.08 ./bootstrap-vitasdk.sh
~~~

Set the SDK environment variables in your shell:

~~~sh
export VITASDK=/usr/local/vitasdk
export PATH="$VITASDK/bin:$PATH"
~~~

### 2. Install the renderer dependency

~~~sh
vdpm install libvita2d
~~~

### 3. Configure and build

From the repository root:

~~~sh
cmake -S . -B build
cmake --build build --parallel
~~~

The build produces PSBBV.vpk in the build directory. Install it on a compatible, homebrew-enabled PlayStation Vita for hardware testing.

A Docker-based build is also supported by the GitHub Actions workflow. See [VitaSDK documentation](https://vitasdk.org/) for SDK installation, package management, and toolchain details.

## Initial controls

| Input | Action |
|---|---|
| D-pad Up / Down | Move between channels |
| Cross | Open the selected channel |
| Start | Exit the application |

The initial channel screens are visual foundations. Full channel implementations and configurable controls are planned work.

## Repository map

~~~text
.
├── .github/workflows/   # Automated Vita builds
├── assets/branding/     # Source brand artwork
├── docs/                # Product, design, and architecture documentation
├── include/              # Public application headers
├── src/
│   ├── app/              # Application state and navigation
│   └── main.cpp          # Vita entry point and lifecycle
├── CMakeLists.txt
└── README.md
~~~

## Project identity

- Product name: **PSBBV**
- Full name: **PlayStation Broadband Vita**
- Display resolution: **960 × 544**
- Initial VPK title ID: SCBV00001 (provisional; confirm before a public release)
- Documentation and user-facing interface language: **English**

The visual identity follows the proposed PSBBN-to-PSBBV transition: a darkened legacy vertical stroke and a red Vita-facing V accent. The final production logo and LiveArea artwork will be refined as part of the design work.

## Contributing and licensing

The architecture and contribution workflow are still being established. A repository license has deliberately not been selected yet; until one is added, all rights are reserved by the copyright holder. Please open an issue before proposing substantial changes.

## Acknowledgements

PSBBV builds on the work of the Vita homebrew community and the maintainers of [VitaSDK](https://github.com/vitasdk) and [libvita2d](https://github.com/xerpi/libvita2d).
