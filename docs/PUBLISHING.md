<div align="center">
<img src="assets/spdw-factory-mark.svg" alt="SPDW Factory" width="160">

**SPDW FACTORY / PUBLISHING NOTES**

# 🚀 Publish the field guide

*One switch in Settings. Then the docs ship with the repo.*
</div>

## One-time GitHub setup

1. Open the repository **Settings**.
2. Open **Pages** under Code and automation.
3. Under **Build and deployment**, set **Source** to **GitHub Actions**.
4. Save the setting.

## What happens next?

The SPDW Documentation workflow runs on pushes to main when the docs folder, workflow or README changes. It packages the docs directory and deploys it through GitHub Pages.

The portal entry point is index.html. The HTML pages use local CSS, JavaScript and SVG assets; there is no external frontend framework or build step.

## Preview locally

From the repository root:

~~~sh
python3 -m http.server 8000 --directory docs
~~~

Then open the local server address shown by Python and browse the index page.

## Before publishing

- Check that the links and SVG assets resolve from the docs directory.
- Confirm the README reflects the current project state.
- Never mark a planned channel as implemented just because its page looks finished.
- Keep all page content and visual assets in the SPDW Factory system.

> GitHub Pages hosting becomes available only after the one-time repository setting is enabled and a deployment run succeeds.
