<div align="center">
<img src="docs/assets/spdw-factory-mark.svg" alt="SPDW Factory" width="170">

**SPDW FACTORY / CONTRIBUTOR NOTES**

# 🤝 Contributing to PSBBV

*Good ideas welcome. Mystery code, not so much.*
</div>

## Before changing code
- Read the [Product Brief](docs/product.html), [Architecture](docs/architecture.html) and [Design Guidelines](docs/design.html).
- Keep source comments, documentation and user-facing strings in English.
- Make focused changes with a clear reason and a test plan.
- Never present unfinished functionality as implemented.
- Do not add large dependencies without documenting their memory, performance and packaging cost.

## Code conventions
- C++17.
- Keep Vita-specific API calls behind the platform boundary where practical.
- Use descriptive names and small, focused functions.
- Avoid per-frame heap allocation.
- Keep UI colours in shared theme tokens.
- Preserve physical-button navigation for every essential flow.
- No blocking network or storage work in the render loop.

## Pull requests
A useful PR says:
1. What problem it solves.
2. How the implementation works.
3. What CI and hardware tests were performed.
4. Screenshots or a short capture for visual changes.
5. Known limitations and remaining work.

## Documentation standard
Documentation is part of the SPDW Factory signature. Use direct titles, clear language, visual variety and accurate implementation states. Add diagrams or artwork when they explain something better than another paragraph.

Sarcasm is welcome. Ambiguity is not.

## Licensing
A repository licence has not yet been selected. Until one is added, contributions must not assume that the project is available under an open-source licence.
