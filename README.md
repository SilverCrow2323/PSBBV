<div align="center">
<img src="docs/assets/spdw-factory-mark.svg" alt="SPDW Factory visual signature" width="220">

# PSBBV Core — PlayStation Broadband Vita
**A modern, native Vita navigator inspired by PSBBN.**

[![Vita Build](https://github.com/SilverCrow2323/PSBBV/actions/workflows/build-vita.yml/badge.svg?branch=dev/foundation)](https://github.com/SilverCrow2323/PSBBV/actions/workflows/build-vita.yml)
[![SPDW Field Guide](https://img.shields.io/badge/SPDW-Field%20Guide-d4ff43?labelColor=11131a)](docs/index.html)
[![Platform](https://img.shields.io/badge/Platform-PS%20Vita-ff5c50)](#)
[![Status](https://img.shields.io/badge/Status-Foundation%20build-8a76f4)](#status)

### Core node registry

PSBBV Core integrates six functional nodes: Games, Applications, Network, Media, Downloads and System. See the [node registry](docs/nodes.html) for responsibilities, boundaries and release-manifest rules.

## [OPEN THE ONLINE SPDW FIELD GUIDE ↗](https://silvercrow2323.github.io/PSBBV/)
[Local source preview](docs/index.html) · [GitHub repository](https://github.com/SilverCrow2323/PSBBV)
</div>

---

> **SPDW FACTORY / PROJECT FILE 001**  
> PSBBN's channel-first spirit. A Vita-native experience. A lot more thought than another icon grid.

## 🧭 What's PSBBV?
PSBBV is an independent PlayStation Vita homebrew navigator. It brings the broader, console-like navigation concept of PlayStation Broadband Navigator into a modern handheld frontend: channels, contextual workspaces, local content and optional connected experiences.

We're not doing a pixel-perfect PSBBN replica. We're taking the good idea, updating the experience and making it fit the hardware.

<sub>Independent community project. Not affiliated with, endorsed by, or sponsored by Sony Interactive Entertainment.</sub>

## 🏛️ Core is the official project hub

PSBBV Core owns the integrated product identity, navigation, node registration, lifecycle, permissions, compatibility contracts and the official release channel. The six node repositories (Games, Applications, Network, Media, Downloads and System) are independently developed components. Their builds and tags do not replace an integrated Core release.

**Release contract:** [Core release manifest](docs/CORE-RELEASE-MANIFEST.md) and [example manifest](docs/release-manifest.example.json).\n\n**Official integrated releases:** [PSBBV Core Releases](https://github.com/SilverCrow2323/PSBBV/releases). Each release must identify the Core version, compatible node revisions, included VPK/package artifacts, checksums, known limitations and validation evidence. The current foundation build is not presented as a stable release.

## 🚦 Status: foundation build

| Area | Current state |
|---|---|
| VitaSDK / CMake application target | **Implemented in source** |
| 960 × 544 Home renderer | **Implemented in source** |
| D-pad, Cross, Circle and Start navigation | **Implemented in source** |
| Front touchscreen | **Implemented in source** |
| About and application menu | **Implemented in source** |
| VPK packaging and CI artifact | **CI build passed** |
| Six channel modules | **Planned placeholders** |
| Real Vita installation and lifecycle test | **Pending** |

<details><summary><strong>🧯 What does “foundation build” actually mean?</strong></summary>
The shell can be cross-compiled and packaged into a VPK. Channel views are intentionally marked as planned placeholders. They do not yet scan or launch games, manage applications, play media, download content or provide network services.

A successful CI build is a useful checkpoint. It is not proof of real-device compatibility.
</details>

## 🎛️ First interaction map

| Input | Action |
|---|---|
| D-pad Up / Down | Move between channels or menu items |
| Cross | Open a channel or activate a menu item |
| Circle | Return or close the application menu |
| Start | Open or close the application menu |
| Front touchscreen | Focus and open channels or activate menu items |

The back touch panel is not used in this foundation build.

## 🧩 Six doors, one house

| Channel | Job | State |
|---|---|---|
| 🎮 Games | Local game collection and shortcuts | Planned |
| 🛠️ Applications | Homebrew and installed utilities | Planned |
| 🌐 Network | Connectivity and optional online features | Planned |
| 🎧 Media | Local music, video and images | Planned |
| 📦 Downloads | Transfer tasks and completed downloads | Planned |
| ⚙️ System | Device information and PSBBV preferences | Planned |

No fake progress bars. No pretend network lights. A feature gets the “implemented” label when it has a real handler and real data behind it.

## 🧱 Technology
- **C++17** — native application code.
- **VitaSDK** — platform toolchain and system APIs.
- **CMake** — reproducible project and VPK build.
- **libvita2d** — initial GPU-backed 2D rendering prototype.

### Build it
Requirements: VitaSDK, CMake 3.16+, and libvita2d installed through vdpm.

~~~sh
vdpm install libvita2d
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
~~~

The configured package target generates PSBBV.vpk inside the build directory. The automated **Vita Build** workflow uses the VitaSDK 2026.08 container image and uploads a PSBBV-VPK artifact when successful.

## 📚 The SPDW Field Guide
Documentation is part of the product. It should be useful, visual, direct and enjoyable to read—not a grey wall of text that looks like it was faxed from 2004.

| Guide | What you'll find |
|---|---|
| [✳ Product brief](docs/product.html) | Scope, goals, non-goals and acceptance criteria |
| [⌘ Architecture](docs/architecture.html) | Runtime layers, ownership and frame loop |
| [◈ Design language](docs/design.html) | Visual identity, palette and interaction rules |
| [↗ Roadmap](docs/roadmap.html) | Milestones and release gates |
| [✓ Hardware tests](docs/testing.html) | Vita installation, input, touch and lifecycle checklist |
| [🚀 Publishing guide](docs/publishing.html) | Enable GitHub Pages and publish the SPDW field guide |
| [🧩 Node API](docs/NODE_API.md) | Shared lifecycle, manifest, permissions, and compatibility contract |
| [🎬 Media blueprint](docs/MEDIA_NODE_BLUEPRINT.md) | Independent media-center product and Kodi compatibility plan |

**[OPEN THE FULL SPDW DOCUMENTATION PORTAL ↗](https://silvercrow2323.github.io/PSBBV/)**

> **Publishing note:** this is the expected GitHub Pages address. The project still needs the one-time Pages source selection described in [the publishing guide](docs/publishing.html); use the local [docs preview](docs/index.html) until a successful deployment is confirmed.

## 🗂️ Repository map

~~~text
.
├── .github/workflows/         # Vita build and documentation deployment
├── assets/branding/           # Editable PSBBV mark
├── docs/
│   ├── assets/                # SPDW visual system and diagrams
│   ├── *.html                 # Branded documentation portal
│   ├── *.md                   # Source documentation
│   └── _config.yml            # GitHub Pages settings
├── include/psbbv/             # Application and UI headers
├── sce_sys/                   # VPK icon and LiveArea resources
├── src/
│   ├── app/                   # Navigation and application views
│   └── main.cpp               # Vita entry point and frame loop
├── CMakeLists.txt
└── README.md
~~~

## 🏭 The SPDW rule
Build the thing. Explain the thing. Make both worth opening.

All project-facing documentation, source comments and user-facing UI strings are written in English. A repository licence has not yet been selected; until one is added, all rights are reserved by the copyright holder.

<div align="center">
**SPDW FACTORY**  
*Built with intent. Documented with attitude.*
</div>


## The ecosystem

See the [PSBBV Ecosystem Map](docs/ECOSYSTEM.md) for repository ownership, node boundaries, manifest policy and the Core integration model.
