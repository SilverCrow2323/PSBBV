# Roadmap — PSBBV Core

## M0 — Product and contract foundation
- [x] Define Core ownership and separate the six capability-node repositories.
- [x] Establish native Vita, offline-first and honest-feature principles.
- [x] Add an automated check for the federated node map in the README.
- [ ] Stabilize the versioned Node API and shared manifest schema.

## M1 — Native shell
- [ ] Land the native application entry point, renderer and input loop after review.
- [ ] Implement focus/navigation lifecycle and safe node registration/removal.
- [ ] Add host tests for registry failure, node removal and navigation state.
- [ ] Build and inspect a Vita VPK with reproducible VitaSDK dependencies.

## M2 — Built-in nodes
- [ ] Integrate Games first, then Applications, Media, Network, Downloads and System through explicit contracts.
- [ ] Verify node capability/permission declarations against Core policy.
- [ ] Keep unimplemented nodes visibly unavailable rather than showing simulated success.

## M3 — Device qualification
- [ ] Test 960×544 layout, controller/touch input, memory and frame pacing on physical Vita.
- [ ] Test offline startup, missing-node recovery, suspend/resume and repeated launch.
- [ ] Record Vita firmware, model, VitaSDK revision and VPK hash for each qualified build.

## Acceptance
Core remains navigable offline; every node has one owner and a versioned contract; failures do not strand global navigation; CI/VitaSDK evidence is distinct from physical hardware results; no proprietary payload or unsupported privileged operation is included.

## Release target
Keep the default branch at foundation status until native shell PRs are reviewed, merged and followed by a VitaSDK build plus physical Vita qualification.
