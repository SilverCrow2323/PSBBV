<div align="center">
<img src="assets/spdw-factory-mark.svg" alt="SPDW Factory" width="160">

**SPDW FACTORY / FIELD NOTES**

### 🎨 PSBBV Design Guidelines

*Palette, typography and interaction principles.*

[OPEN THE VISUAL GUIDE ↗](design.html) · [PROJECT HOME](index.html)
</div>

---


## Canvas and layout

- Target the Vita's native display: **960 × 544**.
- Design for landscape orientation.
- Keep important controls and labels inside a consistent safe area.
- Use touch targets large enough for reliable front-panel interaction.
- Keep every essential task navigable without touch.
- Prefer a small number of clear content groups over dense grids.

## Visual identity

PSBBV should preserve the spirit of PSBBN while presenting a distinct modern identity.

| Token | Hex | Purpose |
|---|---|---|
| Background | #080A0E | Main canvas |
| Sidebar | #0C0F15 | Navigation rail |
| Panel | #11151D | Primary surfaces |
| Raised panel | #171C25 | Cards and elevated surfaces |
| Primary text | #F4F6F8 | Main titles |
| Secondary text | #9DA6B3 | Supporting information |
| Muted text | #697382 | Metadata |
| Vita red | #E42F3D | Selection and identity accent |
| Graphite | #454B56 | Legacy mark detail |

Use red as a deliberate accent, not as a fill color for every component.

## Typography

- Prioritize legibility at handheld viewing distance.
- Maintain a clear hierarchy between page, section, label, and metadata text.
- Avoid long paragraphs on the home screen.
- Do not rely on color alone to communicate selection or status.
- Keep essential information legible at the real 960 × 544 target size.

The foundation build uses the Vita system PGF font to prove native text rendering. A production typeface and bundled font licensing decision will be made separately.

## Navigation and interaction

- Every essential route must be reachable with physical controls.
- Selection must remain visually obvious.
- Cross activates; Circle returns; Start opens the application menu.
- Touch is an additional input method, never the only input method.
- Avoid hidden gestures for essential actions.
- Restore focus when returning from a channel or transient view.

## Motion

- Motion should communicate hierarchy, focus, or a state change.
- Prefer short, smooth transitions.
- Avoid constant parallax, excessive blur, and full-screen effects that reduce readability.
- Every animation must have an immediate-transition path if later settings support it.

## Content integrity

- Loading, empty, offline, error, planned, and success states must be explicit.
- Never display sample data as real device data.
- Label planned features honestly.
- Network indicators must reflect actual connection state, not merely Wi-Fi availability.
