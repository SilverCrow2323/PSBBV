# PSBBV Core release manifest

The release manifest is the authoritative record of what an official integrated PSBBV Core release contains. It prevents a node build, a green CI run or a documentation deployment from being mistaken for a complete product release.

## One release authority

- **PSBBV Core** owns the product version, release tag, integrated VPK/package, release notes and checksums.
- Each node owns its source and tests. Core pins the exact node commit included in an integrated release.
- A node that is not listed as included in the manifest is not part of that release.
- Development/foundation builds and release candidates must keep `official_release: false`.

## Required release record

Copy [release-manifest.example.json](release-manifest.example.json) into the release preparation workspace and fill in:

1. The Core version, release channel, date and exact Core commit.
2. For each of the six nodes, the canonical repository, exact commit, inclusion flag and integration state.
3. Every distributable artifact, with filename, platform, size and SHA-256.
4. CI, VPK integrity and real-device acceptance evidence.
5. Known limitations and any compatibility restrictions.

Do not use a branch name such as `main` as a revision. Pin immutable commit SHAs. Do not mark a node included merely because its repository exists. Distinguish an Actions artifact ZIP digest from the digest of the extracted VPK.

## Release gate

An official stable release may be tagged only after the manifest is complete, the integrated package is built from the pinned revisions, the automated VPK verifier confirms ZIP/CRC integrity, safe paths, required LiveArea assets and the expected `sce_sys/param.sfo` identity, the required Vita hardware checks are recorded, and the release notes accurately describe the tested feature set.

A successful cross-build is necessary evidence, not hardware acceptance. A website deployment is documentation publishing, not a software release. A release candidate may be circulated as a CI artifact for device testing, but it must not be represented as a stable release.

## Current 0.1.0 candidate

The current [PSBBV Core 0.1.0-rc.1](releases/v0.1.0-rc.1.md) candidate is built from immutable Core commit `413df7d8be300d60b953169cf93c3a17940bd24f`.

- Vita Build: [run #44941640583](https://github.com/SilverCrow2323/PSBBV/actions/runs/44941640583).
- Actions artifact: [#14721901529](https://github.com/SilverCrow2323/PSBBV/actions/runs/44941640583/artifacts/14721901529), 151,520 bytes.
- Artifact ZIP SHA-256: `a044bbae8669503e45f40eda8b0cc85f87fe3f8f78a49618a51972e41ceacb00`.
- Extracted `PSBBV.vpk` SHA-256: `cf162a529f4461cb5f72449a417433e6b8567c77b3c3a6a43da1360e7f1caa63`.

The machine-readable [candidate manifest](releases/v0.1.0-rc.1-manifest.json) binds the Core source SHA, workflow run, artifact ID, canonical artifact URL and both file digests. Its validator rejects cross-run or checksum mismatches. It intentionally keeps `official_release: false`, leaves the release tag/date empty, excludes all six node repositories from the package, marks `SCBV00001` provisional and leaves hardware acceptance pending.

## Current foundation state

The generic example manifest intentionally contains no node repositories, commits or artifacts. The current foundation candidate is not an official stable release. Do not publish a stable tag until the exact candidate has passed physical Vita validation and its package identity is final.
