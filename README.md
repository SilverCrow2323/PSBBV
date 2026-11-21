<div align="center">
<img src="docs/assets/spdw-factory-mark.svg" alt="SPDW Factory visual signature" width="220">

# PSBBV Core — PlayStation Broadband Vita
**A modern, native Vita navigator inspired by PSBBN.**

[![Vita Build](https://github.com/SilverCrow2323/PSBBV/actions/workflows/build-vita.yml/badge.svg?branch=dev/foundation)](https://github.com/SilverCrow2323/PSBBV/actions/workflows/build-vita.yml)
[![SPDW Field Guide](https://img.shields.io/badge/SPDW-Field%20Guide-d4ff43?labelColor=11131a)](docs/index.html)
[![Platform](https://img.shields.io/badge/Platform-PS%20Vita-ff5c50)](#)
[![Status](https://img.shields.io/badge/Status-0.1.0%20RC%20prep-8a76f4)](docs/releases/v0.1.0-rc.1.md)

### Core node registry

PSBBV Core registers six planned node identities: Games, Applications, Network, Media, Downloads and System. They are roadmap metadata, not openable destinations. See the [node registry](docs/nodes.html) for responsibilities, boundaries and release-manifest rules.

## [OPEN THE ONLINE SPDW FIELD GUIDE ↗](https://silvercrow2323.github.io/PSBBV/)
[Local source preview](docs/index.html) · [GitHub repository](https://github.com/SilverCrow2323/PSBBV)
</div>

---

> **SPDW FACTORY / PROJECT FILE 001**  
> PSBBN's channel-first spirit. A Vita-native experience. A lot more thought than another icon grid.

## 🧩 Federated module repositories

PSBBV is split into a small native Core and independently versioned capability nodes. A repository existing does not mean its feature is already wired into the Vita UI.

| Node | Repository | Responsibility |
|---|---|---|
| Core | [PSBBV](https://github.com/SilverCrow2323/PSBBV) | Native shell, navigation, rendering and node orchestration |
| Games | [PSBBV-Games](https://github.com/SilverCrow2323/PSBBV-Games) | Installed-title discovery, catalogue metadata and launch boundary |
| Applications | [PSBBV-Applications](https://github.com/SilverCrow2323/PSBBV-Applications) | Homebrew/application catalogue and discovery |
| Media | [PSBBV-Media](https://github.com/SilverCrow2323/PSBBV-Media) | Media metadata and provider integration |
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

## 🧭 What's PSBBV?

PSBBV is an independent PlayStation Vita homebrew navigator. It brings the broader, console-like navigation concept of PlayStation Broadband Navigator into a modern handheld frontend: channels, contextual workspaces, local content and optional connected experiences.

We're not doing a pixel-perfect PSBBN replica. We're taking the good idea, updating the experience and making it fit the hardware.

<sub>Independent community project. Not affiliated with, endorsed by, or sponsored by Sony Interactive Entertainment.</sub>

## 🏛️ Core is the official project hub

PSBBV Core owns the integrated product identity, navigation, node registration, lifecycle, permissions, compatibility contracts and the official release channel. The six node repositories (Games, Applications, Network, Media, Downloads and System) are independently developed components. Their builds and tags do not replace an integrated Core release.

**Release contract:** [Core release manifest](docs/CORE-RELEASE-MANIFEST.md) and [example manifest](docs/release-manifest.example.json).

**Current candidate:** [PSBBV Core 0.1.0-rc.1 notes](docs/releases/v0.1.0-rc.1.md) · [machine-readable candidate manifest](docs/releases/v0.1.0-rc.1-manifest.json) · [Vita Build #44895313787](https://github.com/SilverCrow2323/PSBBV/actions/runs/44895313787).

**Official integrated releases:** [PSBBV Core Releases](https://github.com/SilverCrow2323/PSBBV/releases). Each stable release must identify the Core version, compatible node revisions, included VPK/package artifacts, checksums, known limitations and validation evidence. The current candidate is not a published release.

## 🚦 Status: 0.1.0 release-candidate preparation

| Area | Current state |
|---|---|
| VitaSDK / CMake application target | **Implemented in source** |
| 960 × 544 Home renderer | **Implemented in source** |
| D-pad, Cross, Circle and Start navigation | **Implemented in source** |
| Front touchscreen application menu | **Implemented in source; host-tested hitboxes** |
| About and application menu | **Implemented in source** |
| Host test suite | **10/10 CTest + 9/9 VPK verifier tests passed** |
| Node manifest schema and parser | **Automated validation passed** |
| VitaSDK cross-build and VPK verifier | **Passed on candidate HEAD** |
| Six channel modules | **Planned metadata only; not openable** |
| Real Vita installation and lifecycle test | **Pending for exact candidate artifact** |
| Final Title ID | **SCBV00001 provisional; conflict check pending** |

<details><summary><strong>🧯 What does “release candidate” actually mean?</strong></summary>
The shell can be cross-compiled and packaged into a VPK. The six channel entries are registry metadata only: they are labelled PLANNED and cannot be opened. The Games discovery/browser code is compiled as a foundation but is not exposed as a user-facing Games channel, and game launching is not implemented.

The exact candidate passed automated tests and VPK verification. It has not yet been installed or validated on a physical Vita. It is not a stable release.
</details>

## 🎛️ First interaction map

| Input | Action |
|---|---|
| D-pad Up / Down | Navigate application-menu items while the menu is open |
| Cross | Activate the focused menu item |
| Circle | Return or close the application menu |
| Start | Open or close the application menu |
| Front touchscreen | Activate application-menu items; planned channels are not touch targets |

The back touch panel is not used in this candidate.

## 🧩 Six doors, one house

These are roadmap identities, not open destinations in the current candidate.

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
| [✓ Hardware tests](docs/testing.html) | Exact candidate, CI evidence and Vita checklist |
| [🚀 Publishing guide](docs/publishing.html) | Enable GitHub Pages and publish the SPDW field guide |
| [🧩 Node API](docs/NODE_API.md) | Shared lifecycle, manifest, permissions, and compatibility contract |
| [🎬 Media blueprint](docs/MEDIA_NODE_BLUEPRINT.md) | Independent media-center product and Kodi compatibility plan |
| [🚀 0.1.0-rc.1 notes](docs/releases/v0.1.0-rc.1.md) | Candidate scope, artifact and known limitations |
| [📋 Release manifest](docs/CORE-RELEASE-MANIFEST.md) | Stable release authority and acceptance gates |

**[OPEN THE FULL SPDW DOCUMENTATION PORTAL ↗](https://silvercrow2323.github.io/PSBBV/)**

> **Publishing note:** this is the expected GitHub Pages address. The one-time Pages source selection is still required; use the local [docs preview](docs/index.html) until a successful deployment is confirmed.

## 🗂️ Repository map

~~~text
.
├── .github/workflows/         # Vita build and documentation deployment
├── assets/branding/           # Editable PSBBV mark
├── docs/
│   ├── assets/                # SPDW visual system and diagrams
│   ├── releases/              # Candidate notes and immutable evidence
│   ├── *.html                 # Branded documentation portal
│   ├── *.md                   # Source documentation
│   └── _config.yml            # GitHub Pages settings
├── include/psbbv/             # Application and UI headers
├── sce_sys/                   # VPK icon and LiveArea resources
├── src/
│   ├── app/                   # Navigation and application views
│   └── main.cpp               # Vita entry point and frame loop
├── tools/                     # Package and release-candidate validators
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
