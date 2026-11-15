# PSBBV

**PlayStation Broadband Vita**  
*The next generation of PlayStation navigation.*

PSBBV is an independent, native homebrew navigator for PlayStation Vita, inspired by the channel-based experience and visual identity of PlayStation Broadband Navigator (PSBBN).

The goal is not to recreate PSBBN pixel-for-pixel or to build another conventional game launcher. PSBBV brings that broader, console-like navigation concept to Vita with a refined interface, modern interaction design, and useful system-aware features.

> PSBBV is an independent homebrew project. It is not affiliated with, endorsed by, or sponsored by Sony Interactive Entertainment.

## Project status

PSBBV now has its first native application scaffold: a C++17 Vita application, a libvita2d-based rendering baseline, physical and front-touch navigation, an application menu, original branding assets, VPK packaging, and an automated VitaSDK build workflow.

This is an early foundation build, not a release. The six channel workspaces are explicitly marked as planned placeholders; they do not yet scan, launch, download, stream, or manage content.

The first interface targets the Vita's native **960 × 544** display. The product and acceptance criteria are described in [the Product Brief](docs/PRODUCT_BRIEF.md), and the runtime boundaries are described in [the Architecture](docs/ARCHITECTURE.md).

## Core principles

- **Native first:** a homebrew application built for PlayStation Vita, not a desktop or web UI port.
- **PSBBN heritage:** channel-oriented navigation, contextual content, and a cohesive digital environment.
- **Modern execution:** clear hierarchy, restrained motion, legible typography, and deliberate spacing.
- **Hardware-aware:** responsive controls, modest memory use, and predictable frame pacing.
- **Offline-capable:** core navigation must remain useful without a network connection.
- **Honest functionality:** unfinished features must be clearly identified and must never masquerade as working services.

## Technology

- C++17
- VitaSDK
- CMake
- libvita2d for the initial GPU-backed 2D rendering prototype
- Vita system APIs for application lifecycle, controller input, and front-touch input

The renderer dependency is intentionally limited to the first native UI prototype. Additional dependencies must be justified by a concrete feature and measured hardware cost.

## Build

### Requirements

- VitaSDK installed and available through the VITASDK environment variable.
- CMake 3.16 or newer.
- libvita2d installed through vdpm.

Install the renderer dependency:

~~~sh
vdpm install libvita2d
~~~

Configure and build from the repository root:

~~~sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
~~~

The build is configured to generate PSBBV.vpk in the build directory.

### Automated build

The **Vita Build** workflow runs on pushes to main and dev/**, pull requests, and manual dispatch. A successful run uploads the VPK as the PSBBV-VPK artifact.

The workflow uses the VitaSDK 2026.08 container image. See the [official VitaSDK guide](https://vitasdk.org/) for SDK installation and toolchain details.

## Current interaction

| Input | Action |
|---|---|
| D-pad Up / Down | Move between channels or menu items |
| Cross | Open a channel or activate a menu item |
| Circle | Return or close the application menu |
| Start | Open or close the application menu |
| Front touchscreen | Focus and open a channel or activate a menu item |

The application menu contains Resume Navigation, About PSBBV, and Exit Application. The back touch panel is not used in this foundation build.

## Initial channels

| Channel | Current state |
|---|---|
| Games | Planned placeholder |
| Applications | Planned placeholder |
| Network | Planned placeholder |
| Media | Planned placeholder |
| Downloads | Planned placeholder |
| System | Planned placeholder |

A channel must have a real activation handler and a documented data source before it can be described as implemented.

## Repository map

~~~text
.
├── .github/workflows/        # Automated Vita builds
├── assets/branding/          # Editable PSBBV source artwork
├── docs/
│   ├── ARCHITECTURE.md
│   ├── DESIGN_GUIDELINES.md
│   ├── PRODUCT_BRIEF.md
│   ├── ROADMAP.md
│   └── TESTING.md
├── include/psbbv/            # Application and UI headers
├── sce_sys/                  # VPK icon and LiveArea resources
├── src/
│   ├── app/                  # Navigation and application views
│   └── main.cpp              # Vita entry point and frame loop
├── CMakeLists.txt
└── README.md
~~~

## Project identity

- Product name: **PSBBV**
- Full name: **PlayStation Broadband Vita**
- Display resolution: **960 × 544**
- Initial VPK title ID: SCBV00001 (provisional; must be confirmed before a public release)
- Documentation and user-facing interface language: **English**

The visual identity follows the proposed PSBBN-to-PSBBV transition: a darkened legacy vertical stroke and a red Vita-facing V accent. The current SVG is an editable direction, not the final production mark.

## Contributing and licensing

Read [CONTRIBUTING.md](CONTRIBUTING.md) before proposing changes. Keep documentation, comments, and user-facing strings in English.

A repository license has deliberately not been selected yet. Until one is added, all rights are reserved by the copyright holder.

## Acknowledgements

PSBBV builds on the work of the Vita homebrew community and the maintainers of [VitaSDK](https://vitasdk.org/) and [libvita2d](https://github.com/xerpi/libvita2d).
