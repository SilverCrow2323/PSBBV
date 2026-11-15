<div align="center">

![PSBBV / SPDW Factory](assets/branding/psbbv-docs-banner.svg)

# 🤝 BUILD WITH US

**Good ideas welcome. Mystery changes, not so much.**

<sub>SPDW FACTORY · CONTRIBUTING / 006</sub>

</div>

## Before touching code

- Read the [Product Brief](docs/PRODUCT_BRIEF.md) and [Architecture](docs/ARCHITECTURE.md).
- Follow the [Design Guidelines](docs/DESIGN_GUIDELINES.md).
- Read the [SPDW Factory Documentation Style Guide](docs/STYLE_GUIDE.md).
- Keep source comments, documentation, and user-facing strings in English.
- Keep changes focused and explain why they belong in PSBBV.
- Never label unfinished functionality as working.
- Do not add a large dependency without explaining its memory, performance, and packaging cost.

## 🧑‍💻 Code rules

- Use C++17.
- Keep Vita-specific APIs behind clear platform boundaries where practical.
- Choose descriptive names and small, focused functions.
- Avoid heap allocation in the frame loop.
- Keep shared visual tokens in the theme layer.
- Preserve physical-button navigation for every essential flow.
- Keep blocking storage and network work out of rendering.

## 🎨 Documentation is part of the product

Every document must follow the SPDW Factory editorial direction:

- Direct, readable writing. No ceremonial vocabulary.
- A little personality and sarcasm, never at the reader's expense.
- Visual variety: useful headings, callouts, tables, diagrams, screenshots, and original artwork.
- No endless walls of text.
- Clear status labels: implemented, planned, unavailable, blocked, and needs testing.
- English language across project documentation and UI.

See [docs/STYLE_GUIDE.md](docs/STYLE_GUIDE.md). The documentation is part of our signature, not an afterthought.

## 📬 Pull request checklist

A useful PR includes:

1. The problem it solves.
2. The implementation approach.
3. Build and hardware tests actually performed.
4. Screenshots or a capture for visible UI changes, when available.
5. Known limitations and follow-up work.

## ⚖️ Licensing

A repository license has not been selected yet. Until one is added, do not assume the project is available under an open-source license.

---

**SPDW FACTORY** · PSBBV Contribution Guide  
Bring the idea. Show the work. Tell us what still needs fixing.
