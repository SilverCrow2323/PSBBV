# PSBBV

**PlayStation Broadband Vita**  
*The next generation of PlayStation navigation.*

PSBBV is an independent, native homebrew navigator for PlayStation Vita, inspired by the channel-based experience and visual identity of PlayStation Broadband Navigator (PSBBN).

The goal is not to recreate PSBBN pixel-for-pixel or to build another conventional game launcher. PSBBV aims to bring that broader, console-like navigation concept to Vita with a refined interface, modern interaction design, and useful system-aware features.

> PSBBV is an independent homebrew project. It is not affiliated with, endorsed by, or sponsored by Sony Interactive Entertainment.

## Project status

PSBBV is at the **product and architecture foundation stage**. The repository currently contains project documentation only; the native application, rendering loop, input handling, VPK packaging and automated build pipeline have not yet been implemented.

The first implementation milestone is a standalone Vita homebrew application for the native **960 × 544** display, with physical controls and touch navigation. Channel entries that are not implemented must be presented honestly as planned or unavailable.

See [the Product Brief](docs/PRODUCT_BRIEF.md) for scope and acceptance criteria, and [the Architecture](docs/ARCHITECTURE.md) for the intended runtime boundaries.

## Core principles

- **Native first:** a homebrew application built for PlayStation Vita, not a desktop or web UI port.
- **PSBBN heritage:** channel-oriented navigation, contextual content, and a cohesive digital environment.
- **Modern execution:** clear hierarchy, restrained motion, legible typography, and deliberate spacing.
- **Hardware-aware:** responsive controls, modest memory use, and predictable frame pacing.
- **Offline-capable:** core navigation must remain useful without a network connection.
- **Honest functionality:** unfinished features must be clearly identified and must never masquerade as working services.

## Technology direction

- C++17
- VitaSDK
- CMake
- A small native rendering backend
- Vita system APIs for lifecycle and controller input

The dependency set is not yet locked. Dependencies will be added only when a concrete prototype and hardware requirement justify them.

## Build status

There is no buildable application or VPK yet. Build instructions will be added alongside the first verified VitaSDK project scaffold. Do not treat the current repository as installable software.

## Initial interaction contract

| Input | Intended action |
|---|---|
| D-pad | Move focus between navigation items |
| Cross | Activate the focused item |
| Circle | Go back / close a transient panel |
| Start | Open the application menu |
| Touch | Focus or activate the tapped item |

This is the intended first-milestone contract, not a claim of implemented controls.

## Repository map

```text
.
├── docs/
│   ├── ARCHITECTURE.md
│   └── PRODUCT_BRIEF.md
└── README.md
```

The source tree, build system, assets and CI will be introduced as independently reviewable implementation increments.

## Project identity

- Product name: **PSBBV**
- Full name: **PlayStation Broadband Vita**
- Target display: **960 × 544**
- Initial VPK title ID: SCBV00001 (provisional; must be confirmed before a public release)
- Documentation and user-facing interface language: **English**

The visual identity follows the proposed PSBBN-to-PSBBV transition: a darkened legacy vertical stroke and a red Vita-facing V accent. The final production logo and LiveArea artwork will be original assets and will be refined during implementation.

## Contributing and licensing

The architecture and contribution workflow are still being established. A repository license has deliberately not been selected yet; until one is added, all rights are reserved by the copyright holder. Please open an issue before proposing substantial changes.

## Acknowledgements

PSBBV builds on the work of the Vita homebrew community and the maintainers of [VitaSDK](https://vitasdk.org/) and [libvita2d](https://github.com/xerpi/libvita2d).
