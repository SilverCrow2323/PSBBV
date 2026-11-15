<div align="center">

![PSBBV / SPDW Factory](../assets/branding/psbbv-docs-banner.svg)

# 🎬 PSBBV MEDIA

**A proper media center. Not a video button wearing a fancy jacket.**

<sub>SPDW FACTORY · PRODUCT BLUEPRINT / 001</sub>

</div>

## Product target

PSBBV Media is a separately developed media-center node for the PSBBV ecosystem. Its intended experience is comparable in breadth to a modern Kodi-style media center, while its interface, navigation, branding, and interaction design remain original SPDW Factory work.

Proposed repository: `SilverCrow2323/PSBBV-Media`.

This blueprint does not imply that the repository has been created or that playback, IPTV, Stream4me, or Kodi add-on compatibility is already implemented.

## Product areas

| Workspace | Intended scope |
|---|---|
| Movies | Local libraries, artwork, metadata, collections, watched state |
| TV Shows | Series, seasons, episodes, resume position |
| IPTV | User-provided playlists, channel groups, EPG, favourites |
| Streaming sources | Pluggable source adapters and playback hand-off |
| Stream4me | A separately verified provider integration, subject to its actual interface and terms |
| Music | Artists, albums, tracks, playlists |
| Add-ons | Discovery, installation, enable/disable, updates, permissions |
| Player | Playback controls, seeking, audio/subtitle tracks, resume |
| Settings | Library paths, providers, network, privacy, playback preferences |

## Architecture

Keep the user interface independent from the media engine:

- **SPDW Media UI** — original visual system, navigation, library browsing, detail pages, player overlay.
- **Media Core** — catalog model, metadata, playback session state, subtitle/audio selection, resume data.
- **Source adapters** — local files, user-provided IPTV playlists, remote providers.
- **Add-on compatibility layer** — API bridge and compatibility reporting.
- **Platform backends** — capability-specific playback and storage implementations.

The desktop backend should be developed first where it provides fast iteration and richer diagnostics. A PS Vita backend is a constrained target, not a promise that every desktop feature or codec will fit.

## Kodi add-on compatibility: a measured target

“Compatible with Kodi add-ons” must be demonstrated per add-on and per API surface.

Planned compatibility layers:

1. **Metadata and repository formats** — inspect supported add-on manifests and dependency declarations.
2. **Python API bridge** — implement a documented subset of Kodi's add-on APIs where licensing, runtime, and platform constraints permit.
3. **Runtime and dependency manager** — isolate add-on execution, resolve dependencies, and expose explicit permissions.
4. **Native add-on interfaces** — offer a PSBBV SDK for integrations that need platform-specific performance.

Compatibility classes:

| Label | Meaning |
|---|---|
| Verified | Automated tests and a real end-to-end run pass on the named platform |
| Partial | Some APIs work; missing APIs and limitations are listed |
| Metadata-only | Add-on can be listed but not executed |
| Unsupported | Runtime, dependency, binary, API, or policy requirement cannot be met |

Kodi binary add-ons are architecture- and platform-dependent. Python add-ons can still rely on Kodi modules, APIs, native dependencies, or services that do not exist on the Vita. A complete Kodi runtime must not be assumed portable.

Do not execute untrusted add-ons with unrestricted filesystem, network, or credential access. Add-on permissions, source provenance, dependency versions, and update behavior are part of the product—not optional polish.

## IPTV, remote sources, and legal boundaries

The application should support user-configured sources and lawful streams. It must not bundle copyrighted channel lists, paid-service credentials, DRM circumvention, or unauthorized content. Stream4me support remains a research item until its official integration surface, authentication model, platform availability, and terms are verified.

Network operations must be cancellable, time out, and report clear offline/authentication/format errors. Credentials belong in platform-protected storage where available and must never appear in logs or exported diagnostics.

## Platform reality check

Before selecting a playback stack, run a technical spike on each target:

- supported containers, codecs, profiles, and maximum practical resolutions;
- hardware decode availability and API access;
- memory footprint and buffering requirements;
- subtitle rendering and audio-track selection;
- network throughput, TLS support, and source protocol constraints;
- licensing and redistribution obligations of every bundled library.

PS Vita support will likely need a narrower feature matrix than desktop. The UI must expose unsupported formats honestly rather than silently failing.

## Milestones

- [ ] Create repository and SPDW project skeleton.
- [ ] Define shared PSBBV Node API integration and manifest.
- [ ] Build platform-neutral media catalog and metadata model.
- [ ] Add local library scanner with safe, cancellable indexing.
- [ ] Prototype desktop playback backend and measure supported formats.
- [ ] Implement Movies and TV Shows library views.
- [ ] Add user-configured IPTV playlist and EPG support.
- [ ] Verify Stream4me integration feasibility before implementation.
- [ ] Build add-on manager and compatibility test harness.
- [ ] Implement Kodi API bridge incrementally; publish per-add-on results.
- [ ] Build original SPDW Media UI and player experience.
- [ ] Evaluate and implement a Vita-specific feature subset on hardware.

## Definition of done

A feature is complete only when its source, tests, user-facing behavior, failure handling, and supported-platform matrix agree. A listed add-on is not a working add-on. A visible poster is not a scanned library. A playback button is not a decoder.

---

**SPDW FACTORY** · PSBBV Media  
A bigger media experience. Honest compatibility. No magic claims.
