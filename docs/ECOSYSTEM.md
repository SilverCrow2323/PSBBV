<div align="center">

# 🧩 THE PSBBV ECOSYSTEM

**One Vita host. Six focused nodes. No six competing shells.**

<sub>SPDW FACTORY · ENGINEERING / ECOSYSTEM MAP</sub>

</div>

## Repository ownership

| Repository | Product responsibility | What it must not own |
|---|---|---|
| [PSBBV Core](https://github.com/SilverCrow2323/PSBBV) | Vita app lifecycle, Home navigation, focus/input, rendering, Node API, permissions and node registry | Domain-specific media, game, package or transfer engines |
| [PSBBV-Media](https://github.com/SilverCrow2323/PSBBV-Media) | Movies, TV, IPTV, metadata, playback orchestration, subtitles/audio and reviewed add-on compatibility | Global navigation or unrestricted provider execution |
| [PSBBV-Games](https://github.com/SilverCrow2323/PSBBV-Games) | Installed-title discovery, catalogue, favourites and guarded launching | Downloading or redistributing commercial game content |
| [PSBBV-Applications](https://github.com/SilverCrow2323/PSBBV-Applications) | Homebrew/application inventory, package inspection and confirmed management operations | Silent deletion, saves removal or unverified packages |
| [PSBBV-Network](https://github.com/SilverCrow2323/PSBBV-Network) | Connectivity status, diagnostics and secure shared network adapters | A competing authentication authority or insecure TLS bypass |
| [PSBBV-Downloads](https://github.com/SilverCrow2323/PSBBV-Downloads) | Cancellable transfer queue, safe destinations and integrity verification | Blocking UI work or unconfirmed overwrite of user data |
| [PSBBV-System](https://github.com/SilverCrow2323/PSBBV-System) | Preferences, supported diagnostics, safe cache management and redacted reports | Arbitrary shell execution or unsupported privileged settings |

## Product and integration model

- Core owns the application process and the global frame/input lifecycle.
- Each node owns its domain models, use cases, provider adapters and workspace screens.
- Nodes communicate through the versioned Node API and typed host services; they do not reach into another node's private implementation.
- Network, storage and input access are declared as capabilities/permissions. A declaration is not itself a grant.
- The six domain repositories are development workspaces, not independently installable nodes or separately released products. Core remains the only Vita application and the only PSBBV release authority.
- The Core build must not fetch, build or link a development workspace as an external runtime dependency. Integration means reviewed source is brought into Core and tested as part of the single product.
- Desktop tools and platform adapters may be added independently, but every platform must declare and test its own capabilities.

## Current baseline

- Core: Node API 1.0 proposal, JSON Schema, built-in metadata registry and channel-to-node binding.
- The six workspace manifests describe development contracts and status only; their versions are not PSBBV product versions and they do not produce official releases.
- All node repositories carry a pinned copy of the Core manifest schema and a CI validation workflow.
- Manifest validation passing proves descriptor consistency only. It does not prove a node implementation, permission grant, playback engine, launcher, network integration or hardware compatibility.

## Change discipline

1. Propose Node API changes in Core first.
2. Review compatibility impact before updating pinned schema copies.
3. Update affected node manifests and tests in the same coordinated change window.
4. Keep all node availability at `planned` until implementation and platform evidence justify a promotion.
5. Keep all six workspace repositories release-free as PSBBV products. Only the Core repository may publish an official PSBBV release.
6. Do not merge the Core foundation PR or publish a stable release without explicit approval and completed hardware gates.

---

**SPDW FACTORY** · One contract. Independent domains. A coherent experience.
