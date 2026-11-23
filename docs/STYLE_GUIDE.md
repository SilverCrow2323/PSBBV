# SPDW Factory / Documentation Style Guide

<div align="center">

# MAKE IT CLEAR. MAKE IT GOOD. MAKE IT OURS.

**SPDW Factory** · The people behind PSBBV

*Technical documentation with a pulse. No grey walls. No corporate fog.*

</div>

---

## 🎛️ The rule

A PSBBV document is part of the product. It is not an afterthought, a dump of notes, or a punishment for clicking the wrong menu.

Every page should be:

- **Useful first.** The reader came to solve something. Get them there.
- **Good to look at.** Break up long passages with diagrams, cards, callouts, examples, tables, and artwork.
- **Easy to understand.** Say what the thing does. Then say how. Skip the ceremonial vocabulary.
- **A little cheeky.** Personality is welcome. Confusion isn't.
- **Honest.** Never turn a plan into a feature by writing it in the present tense.

## 🧭 Voice and tone

| Do | Don't |
|---|---|
| “The app checks the file and shows an error if it cannot read it.” | “The application undertakes a comprehensive validation procedure…” |
| “This part is not ready yet.” | “This functionality is expected to be made available in a future iteration.” |
| “Press Circle to go back.” | “Utilize the circular input modality to navigate to the preceding view.” |
| “The build failed. Here's why.” | “An unforeseen operational anomaly was encountered.” |

Use short sentences and concrete verbs. A little sarcasm can make a point memorable, but never mock the reader. Be playful about the problem, not the person trying to solve it.

## 🧱 Page anatomy

Not every page needs every component. Variety is the point; decoration for decoration's sake is not.

| Component | Best use |
|---|---|
| **Hero banner** | Introduce a major guide, release, or feature |
| **Quick facts** | Put version, platform, status, and requirements up front |
| **Callout** | Highlight a warning, useful shortcut, or important limitation |
| **Cards** | Present related choices or feature groups |
| **Tables** | Compare options, inputs, compatibility, or parameters |
| **Diagrams** | Explain architecture, flows, and dependencies |
| **Code blocks** | Show commands that can be copied and run |
| **Screenshots / artwork** | Show the actual product or explain a visual concept |
| **FAQ** | Answer recurring questions without repeating the whole guide |

Avoid using the same layout for every page. A troubleshooting page should feel different from a product overview.

## 🏷️ Status labels

Use these labels consistently. A green-looking badge is not proof that code exists.

| Label | Meaning |
|---|---|
| 🟢 **READY** | Implemented and verified for the stated environment |
| 🟡 **IN PROGRESS** | Work has started; known gaps remain |
| 🔵 **PLANNED** | Designed or discussed, not implemented |
| 🟠 **NEEDS TESTING** | Built, but not yet verified on target hardware |
| 🔴 **BLOCKED** | Cannot proceed until a named issue is resolved |
| ⚪ **REFERENCE** | Background information, not a feature status |

Always include the environment and test scope when saying something is verified.

## 🎨 Visual system

PSBBV and SPDW Factory share one identity, but the product and its documentation have different jobs. Keep their visual systems related—not interchangeable.

### PSBBV product UI · PlayStation Vita

- **Canvas:** deep near-black.
- **Surfaces:** graphite and cool charcoal.
- **Primary text:** clean white.
- **Supporting text:** cool grey with readable contrast.
- **Accent:** PSBBV red, used for focus and important states.
- **Mark:** the PSBBV wordmark and red V; concepts stay labelled as concepts.

### SPDW Factory documentation · Field Guide

- **Canvas:** warm paper (#F4F2EC) with ink (#11131A).
- **Editorial accents:** acid lime (#D4FF43), coral (#FF5C50), violet (#8A76F4) and cyan (#60D9ED).
- **Typography:** strong headings, compact metadata, readable body copy and clear captions.
- **Layout:** varied cards, tables, diagrams, callouts, artwork and navigable page groups.
- **Brand:** SPDW Factory signature on the landing page and major guides.

The documentation site deliberately uses a lighter editorial canvas so long technical pages remain comfortable to scan. Do not copy its palette into the Vita UI. Use colour with purpose; we're building a navigator, not a fire alarm.

## 🧩 Markdown toolkit

GitHub Markdown and Mermaid are the baseline. Use them with intent:

- Emoji in titles and navigation when they improve scanning.
- Tables for comparisons, not for ordinary paragraphs.
- Mermaid for architecture, sequence, and decision diagrams.
- Blockquotes for callouts; label the purpose clearly.
- Fenced code blocks with the correct language.
- Relative links to local artwork and screenshots.
- Short captions to explain what an image or diagram teaches.

Do not fake components that GitHub cannot render. A clean, well-structured Markdown page beats a broken imitation of a web dashboard.

## 🖼️ Artwork rules

- Use original SPDW Factory / PSBBV artwork.
- Prefer SVG for editable diagrams, wordmarks, and vector banners.
- Keep raster art optimized for the Vita and GitHub page loads.
- Give every image useful alternative text.
- Never use a screenshot of a mockup as evidence of a working feature.
- Mark concepts as **CONCEPT** until the implementation matches them.

## 🧪 Technical truth checklist

Before publishing a page, ask:

1. Does every command match the current repository?
2. Is the described feature actually implemented?
3. Are prerequisites and tested versions stated?
4. Are known limitations visible before the reader hits them?
5. Can a reader follow the steps without guessing?
6. Does the page link to the next useful page?
7. Does it look and sound like SPDW Factory?

## 🧷 The signature

Major guides should end with a short, consistent signature:

> **SPDW Factory** · PSBBV Documentation  
> Built with intent. Written for humans. Still allergic to grey walls.

The signature is part of the identity. It is not a substitute for useful content.
