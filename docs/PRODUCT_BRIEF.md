<div align="center">

![PSBBV / SPDW Factory](../assets/branding/psbbv-docs-banner.svg)

# 📘 THE PRODUCT BRIEF

**What we're building. What we're not. What must work before we celebrate.**

<sub>SPDW FACTORY · PRODUCT / 001</sub>

</div>

> **The pitch:** PSBBV brings the channel-based spirit of PlayStation Broadband Navigator to the Vita — with a cleaner interface, modern interaction, and a healthy allergy to fake features.

## 🎯 The product, in one minute

PSBBV (PlayStation Broadband Vita) is an independent, native PlayStation Vita homebrew application. It adapts the channel-oriented navigation idea of PSBBN to the Vita's **960 × 544** screen, physical controls, and touchscreen.

It is an application — **not** a replacement shell, firmware mod, or promise to recreate PSBBN line for line.

| Promise | What it means |
|---|---|
| Native | Built for Vita, not a desktop UI squeezed into a handheld |
| Channel-first | Games, apps, media, network, downloads, and system live in clear workspaces |
| Usable offline | Core navigation does not need a server to wake up |
| Hardware-aware | Memory, startup time, input response, and frame pacing matter |
| Honest | Planned, unavailable, and working features look different |

## 🚦 First milestone: the Navigator Shell

<badge>FOUNDATION MILESTONE</badge> The first milestone is deliberately narrow. Get the shell right before adding six half-working destinations and calling it an ecosystem.

<list connector="solid" marker="number" gap={8}>
  <list-item>Build a reproducible VitaSDK application and installable VPK.</list-item>
  <list-item>Start and exit cleanly on a real Vita.</list-item>
  <list-item>Render a stable 960 × 544 Home screen.</list-item>
  <list-item>Support D-pad navigation plus Cross and Circle actions.</list-item>
  <list-item>Support front-touch selection without breaking physical controls.</list-item>
  <list-item>Show honest placeholder states for channels without working handlers.</list-item>
  <list-item>Handle suspend/resume and clean up rendering resources.</list-item>
  <list-item>Pass the manual hardware checklist in the testing guide.</list-item>
</list>

> **Release rule:** a successful cross-build is not a hardware release. The VPK must install, launch, navigate, suspend, resume, and exit on an actual Vita.

## 🚫 Not in the first milestone

- Replacing the Vita LiveArea or system shell.
- Reimplementing Sony network services or relying on unavailable proprietary APIs.
- Shipping copyrighted games, media, firmware, or service credentials.
- Making mock channel screens look like working services.
- Building a general-purpose browser before the navigation foundation is stable.

## 🧭 The channel lineup

Each channel gets a stable ID, title, visual identity, availability state, and activation route.

| Channel | Job | First-shell state |
|---|---|---|
| 🏠 Home | Landing page, recent activity, quick access | Foundation view |
| 🎮 Games | Local game discovery and shortcuts | Planned |
| 🧰 Applications | Installed homebrew and utilities | Planned |
| 🌐 Network | Connection status and future online experiences | Planned |
| 🎵 Media | Local music, video, and image entry point | Planned |
| ⬇️ Downloads | Transfer tasks and completed downloads | Planned |
| ⚙️ Settings / System | Preferences, input, appearance, and device info | Planned |

Only channels with real handlers may be marked **READY**. A nice tile is not a feature. Sorry, tiles.

## 🎮 The input contract

| Input | Action |
|---|---|
| D-pad | Move focus through visible items |
| Cross | Activate the focused item |
| Circle | Go back or close a transient panel |
| Start | Open the application menu |
| Select | Reserved for useful contextual actions |
| Touch | Focus or activate an item with visible feedback |

Button mapping must be configurable later. Confirm/cancel conventions differ between systems and users; the UI should not start a civil war over one button.

## 🎨 Visual direction

PSBBN provides inspiration, not a tracing template. PSBBV should feel like its own product:

- confident typography and a clear hierarchy;
- calm, dark surfaces with a deliberate Vita-red accent;
- tile layouts with room to breathe;
- purposeful focus states and motion;
- touch targets that are easy to hit;
- English as the initial UI and documentation language.

The PSBBN-to-PSBBV mark is a concept: a graphite legacy stroke and a red V. Final artwork must be original and must not imply Sony affiliation.

## 🧩 How the first shell fits together

~~~mermaid
flowchart TD
    A["Vita OS"] --> B["PSBBV App"]
    B --> C["Input: buttons + touch"]
    C --> D["Navigation state"]
    D --> E["Home / Channel views"]
    E --> F["Native renderer"]
    F --> G["Vita display"]
~~~

## ✅ Definition of done

A milestone is done when its acceptance criteria are met and recorded — not when the checkbox looks lonely.

- [x] Reproducible VPK build (Vita Build #83; VPK package inspected)
- [ ] Clean launch and exit on hardware
- [ ] Stable Home at native resolution
- [ ] D-pad, Cross, Circle, Start, and front touch tested
- [ ] Placeholder states clearly labelled
- [ ] Suspend/resume verified
- [ ] Hardware test notes recorded

---

<div align="center">

**SPDW FACTORY** · PSBBV Product Documentation  
Built with intent. Written for humans. Still allergic to grey walls.

</div>
