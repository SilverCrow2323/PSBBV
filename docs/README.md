<div align="center">

![PSBBV documentation — SPDW Factory](../assets/branding/psbbv-docs-banner.svg)

# PSBBV / THE DOCS

**The manual has a pulse.**

Native Vita navigation, explained by the people building it.

<sub>SPDW FACTORY · PROJECT DOCUMENTATION</sub>

</div>

---

> **🧭 New here?** Start with the [Product Brief](PRODUCT_BRIEF.md). It explains what PSBBV is, what it is not, and what the first milestone must actually do.

## 🚦 Project pulse

| Area | Status | What that means |
|---|---|---|
| Product definition | 🟢 Written | Scope and non-goals are documented |
| Architecture | 🟡 Foundation | Runtime boundaries are defined; more code is still coming |
| Native shell | 🟡 Foundation | Initial Vita UI and navigation scaffold exists |
| Channel modules | 🔵 Planned | Visible placeholders are not working services |
| Hardware validation | 🟠 Needs testing | CI cannot hold a Vita and press buttons for us |

## 🗺️ Pick your route

<div align="center">

| 📘 Start here | 🧰 Build and understand | 🎨 Make it look right |
|---|---|---|
| [Product Brief](PRODUCT_BRIEF.md) | [Architecture](ARCHITECTURE.md) | [Design Guidelines](DESIGN_GUIDELINES.md) |
| [Roadmap](ROADMAP.md) | [Hardware Test Checklist](TESTING.md) | [SPDW Factory Style Guide](STYLE_GUIDE.md) |

</div>

### 📘 Product Brief
The big idea, first milestone, boundaries, channel model, and input rules. Start here before inventing a feature that belongs in a different app.

### 🧰 Architecture
How the app is split into lifecycle, input, navigation, views, rendering, and future modules. The goal: no mystery spaghetti hiding behind the home screen.

### 🎨 Design Guidelines
Canvas, palette, typography, focus, touch targets, and motion. Pretty is good. Readable and usable is better.

### 🛣️ Roadmap
What is done, what is underway, and what is still just an idea wearing a nice jacket.

### 🧪 Hardware Test Checklist
The checks that need a real Vita. A green cross-build is useful; it is not a tiny handheld console.

### 🏭 SPDW Factory Style Guide
The house rules for every PSBBV document: voice, layouts, artwork, status labels, and visual consistency.

## 🧩 How the pieces fit

~~~mermaid
flowchart TD
    A["Vita OS / VitaSDK"] --> B["Application lifecycle"]
    B --> C["Input snapshot"]
    C --> D["Navigation state"]
    D --> E["Channel registry"]
    E --> F["Channel views"]
    F --> G["UI composition"]
    G --> H["Rendering backend"]
    H --> I["Frame presentation"]
~~~

## 🏷️ Status key

- 🟢 **READY** — implemented and verified for the named environment.
- 🟡 **IN PROGRESS** — implementation has started; gaps remain.
- 🔵 **PLANNED** — designed or discussed, not implemented.
- 🟠 **NEEDS TESTING** — built, but not yet checked on target hardware.
- 🔴 **BLOCKED** — a named issue prevents progress.

A label is not a wish. We only promote a feature when its code and tests earn it.

## ✍️ How we write

Short sentences. Useful headings. Real screenshots. Clear diagrams. Commands that work. Limitations shown before they surprise you.

No jargon parade, no fake feature claims, no grey walls.

---

<div align="center">

**SPDW FACTORY**  
Built with intent. Written for humans. Still allergic to grey walls.

</div>
