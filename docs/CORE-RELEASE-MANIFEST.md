# PSBBV Core release manifest

The release manifest is the authoritative record of what an official integrated PSBBV Core release contains. It prevents a node build, a green CI run or a documentation deployment from being mistaken for a complete product release.

## One release authority

- **PSBBV Core** owns the product version, release tag, integrated VPK/package, release notes and checksums.
- Each node owns its source and tests. Core pins the exact node commit included in an integrated release.
- A node that is not listed as included in the manifest is not part of that release.
- Development/foundation builds must keep `official_release: false`.

## Required release record

Copy [release-manifest.example.json](release-manifest.example.json) into the release preparation workspace and fill in:

1. The Core version, release channel, date and exact Core commit.
2. For each of the six nodes, the canonical repository, exact commit, inclusion flag and integration state.
3. Every distributable artifact, with filename, platform, size and SHA-256.
4. CI, VPK integrity and real-device acceptance evidence.
5. Known limitations and any compatibility restrictions.

Do not use a branch name such as `main` as a revision. Pin immutable commit SHAs. Do not mark a node included merely because its repository exists.

## Release gate

An official release may be tagged only after the manifest is complete, the integrated package is built from the pinned revisions, the automated VPK verifier confirms ZIP/CRC integrity, safe paths, required LiveArea assets and the expected `sce_sys/param.sfo` identity, the required Vita hardware checks are recorded, and the release notes accurately describe the tested feature set.

A successful cross-build is necessary evidence, not hardware acceptance. A website deployment is documentation publishing, not a software release.

## Current foundation state

The example manifest intentionally contains no node repositories, commits or artifacts. Those fields remain unregistered until the canonical node repositories and integration revisions are confirmed. The current PSBBV foundation build is not labelled as an official stable release.
