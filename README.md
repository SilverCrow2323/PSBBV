# PSBBV

**PlayStation Broadband Vita**  
*The next generation of PlayStation navigation.*

PSBBV is an independent, native homebrew navigator for PlayStation Vita, inspired by the channel-based experience and visual identity of PlayStation Broadband Navigator (PSBBN).

The goal is not to recreate PSBBN pixel-for-pixel or to build another conventional game launcher. PSBBV aims to bring that broader, console-like navigation concept to Vita with a refined interface, modern interaction design, and useful system-aware features.

> PSBBV is an independent homebrew project. It is not affiliated with, endorsed by, or sponsored by Sony Interactive Entertainment.

## Project status

PSBBV is in its foundation stage. The repository now includes a portable C++17 navigation state machine, a first native libvita2d menu shell, host regression tests and a VitaSDK VPK workflow. The six channel destinations are navigation foundations; their domain features are not yet wired.

The first interface milestone targets the Vita's native **960 × 544** display and provides a navigable set of channel entry points. Channel functionality will be implemented incrementally; entries that are not implemented yet are presentation scaffolding, not claims of working services.

## Core principles

- **Native first:** a homebrew application built for PlayStation Vita, not a desktop or web UI port.
- **PSBBN heritage:** channel-oriented navigation, contextual content, and a cohesive digital environment.
- **Modern execution:** clear hierarchy, restrained motion, legible typography, and deliberate spacing.
- **Hardware-aware:** responsive controls, modest memory use, and predictable frame pacing.
- **Offline-capable:** core navigation must remain useful without a network connection.
- **Honest functionality:** unfinished features must be clearly identified and must never masquerade as working services.

## Federated module repositories

PSBBV is split into a small native Core and independently versioned capability nodes. The repositories below own their respective domains; a node being present does not mean its feature is already wired into the Vita UI.

| Node | Repository | Responsibility |
|---|---|---|
| Core | [PSBBV](https://github.com/SilverCrow2323/PSBBV) | Native shell, navigation, rendering and node orchestration |
| Games | [PSBBV-Games](https://github.com/SilverCrow2323/PSBBV-Games) | Installed-title discovery, catalogue metadata, favourites and launch boundary |
| Applications | [PSBBV-Applications](https://github.com/SilverCrow2323/PSBBV-Applications) | Homebrew/application catalogue and discovery |
| Media | [PSBBV-Media](https://github.com/SilverCrow2323/PSBBV-Media) | Media metadata and media-provider integration |
| Network | [PSBBV-Network](https://github.com/SilverCrow2323/PSBBV-Network) | Network capability contracts and provider adapters |
| Downloads | [PSBBV-Downloads](https://github.com/SilverCrow2323/PSBBV-Downloads) | Download queue, staging and integrity boundaries |
| System | [PSBBV-System](https://github.com/SilverCrow2323/PSBBV-System) | Device/system information and safe system actions |

### Integration rules

- Core owns navigation, lifecycle and presentation; nodes own domain logic and expose versioned manifests/contracts.
- Node discovery and manifest validation must fail closed: an invalid or incompatible node is unavailable, not partially loaded.
- Network and download nodes must expose explicit state and errors; the UI must never simulate completion.
- Destructive or privileged system actions require a visible preview and explicit confirmation. Read-only probes remain separate from mutation APIs.
- Offline navigation and locally available catalogues must remain usable without a network connection.
- CI for a node proves its own contract/build checks. Core integration and physical Vita behaviour require separate validation.

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
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="$VITASDK/share/vita.toolchain.cmake"
cmake --build build --parallel
~~~

The VitaSDK build produces `build/PSBBV.vpk`. A separate host-only CMake build (without VitaSDK) runs the portable navigation tests via `ctest --test-dir build --output-on-failure`. Install it on a compatible, homebrew-enabled PlayStation Vita for hardware testing.

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
