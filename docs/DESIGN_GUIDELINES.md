<div align="center">

![PSBBV / SPDW Factory](../assets/branding/psbbv-docs-banner.svg)

# 🎨 THE LOOK. THE FEEL. THE RULES.

**A Vita screen is not a shrunk desktop. Design accordingly.**

<sub>SPDW FACTORY · DESIGN / 003</sub>

</div>

## 📐 Start with the actual screen

<grid columns="2" gap="12">
  <grid-item>

  **960 × 544**

  Native landscape canvas. Use the space with intent.

  </grid-item>
  <grid-item>

  **Buttons + touch**

  Every essential action must work without touching the screen.

  </grid-item>
</grid>

Keep key content inside a consistent safe area. Touch targets need room. Dense grids need a very good reason to exist.

## 🎛️ The PSBBV palette

| Token | Colour | Hex | Use |
|---|---|---|---|
| Background | <badge color="#080A0E">●</badge> | #080A0E | Main canvas |
| Sidebar | <badge color="#0C0F15">●</badge> | #0C0F15 | Navigation rail |
| Panel | <badge color="#11151D">●</badge> | #11151D | Main surfaces |
| Raised | <badge color="#171C25">●</badge> | #171C25 | Cards and elevated panels |
| Primary text | <badge color="#F4F6F8">●</badge> | #F4F6F8 | Main titles |
| Secondary text | <badge color="#9DA6B3">●</badge> | #9DA6B3 | Supporting copy |
| Muted text | <badge color="#697382">●</badge> | #697382 | Metadata |
| Vita red | <badge color="#E42F3D">●</badge> | #E42F3D | Focus and identity |
| Graphite | <badge color="#454B56">●</badge> | #454B56 | Legacy-mark detail |

<box border radius="lg" padding="12" gap="6">
  **Accent rule**

  Red is a deliberate signal: focus, action, or identity. It is not the default background for every button. We are making a navigator, not a fire alarm.
</box>

## 🔤 Type should work at handheld distance

- Make page titles, section titles, labels, and metadata clearly different.
- Keep home-screen copy short.
- Never rely on colour alone to show focus or status.
- Check legibility at 960 × 544, not on a 32-inch monitor.
- The foundation build uses the Vita system PGF font. A production typeface needs a separate licensing decision.

## 🕹️ Interaction rules

| Rule | Why |
|---|---|
| Every important route works with physical controls | Touch is optional |
| Focus is always visible | No “where did my selection go?” moments |
| Cross activates; Circle returns; Start opens the menu | Consistency beats surprise |
| Restore focus when returning | Keep the user's place |
| Avoid hidden gestures | Essential actions should not be secret knowledge |

## ✨ Motion with a job to do

Motion should explain focus, hierarchy, or a state change.

**Good:** short transitions, gentle emphasis, predictable focus movement.

**Not good:** constant parallax, blur that hides text, expensive effects that tank frame pacing, or animations that never let the user get on with it.

## 🧾 Content states must be designed too

| State | What the user should see |
|---|---|
| Loading | What is loading, without invented progress |
| Empty | Why there is no content and what to do next |
| Offline | What still works and what needs a connection |
| Error | The cause when known, plus a recovery path |
| Planned | Clearly marked as not implemented |
| Success | Only after the operation really succeeds |

## 🧪 Visual review checklist

- [ ] Looks right at 960 × 544.
- [ ] Readable at handheld distance.
- [ ] Selected state is obvious.
- [ ] Touch targets are easy to hit.
- [ ] Physical controls can complete the same task.
- [ ] Motion does not get in the way.
- [ ] Errors and empty states are designed, not improvised.

---

**SPDW FACTORY** · PSBBV Design  
Make it clear. Make it feel right. Then test it on the actual device.
