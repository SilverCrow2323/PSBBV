<div align="center">

![PSBBV / SPDW Factory](../assets/branding/psbbv-docs-banner.svg)

# 🎨 THE LOOK. THE FEEL. THE RULES.

**A Vita screen is not a shrunk desktop. Design accordingly.**

<sub>SPDW FACTORY · DESIGN / 003</sub>

</div>

## 📐 Start with the actual screen

| **960 × 544** | **Buttons + touch** |
|---|---|
| Native landscape canvas. Use the space with intent. | Every essential action must work without touching the screen. |

Keep key content inside a consistent safe area. Touch targets need room. Dense grids need a very good reason to exist.

## 🎛️ The PSBBV palette

| Token | Colour | Hex | Use |
|---|---|---|---|
| Token | Hex | Use |
|---|---|---|
| Background | `#080A0E` | Main canvas |
| Sidebar | `#0C0F15` | Navigation rail |
| Panel | `#11151D` | Main surfaces |
| Raised | `#171C25` | Cards and elevated panels |
| Primary text | `#F4F6F8` | Main titles |
| Secondary text | `#9DA6B3` | Supporting copy |
| Muted text | `#697382` | Metadata |
| Vita red | `#E42F3D` | Focus and identity |
| Graphite | `#454B56` | Legacy-mark detail |

> **Accent rule**  
> Red is a deliberate signal: focus, action, or identity. It is not the default background for every button. We are making a navigator, not a fire alarm.

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
