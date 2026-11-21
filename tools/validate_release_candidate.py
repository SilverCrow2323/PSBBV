#!/usr/bin/env python3
"""Guard the PSBBV release-candidate record against accidental stable claims."""

import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "docs/releases/v0.1.0-rc.1-manifest.json"


def fail(message: str) -> None:
    print(f"Release candidate manifest: ERROR: {message}", file=sys.stderr)
    raise SystemExit(1)


def main() -> None:
    try:
        data = json.loads(MANIFEST.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        fail(f"cannot read valid JSON ({exc})")

    if data.get("manifest_schema") != "psbbv.core-release/v1":
        fail("unexpected manifest_schema")
    if data.get("core_version") != "0.1.0-rc.1":
        fail("candidate version must remain 0.1.0-rc.1")
    if data.get("channel") != "release-candidate":
        fail("channel must be release-candidate")
    if data.get("official_release") is not False:
        fail("candidate must never claim official_release=true")
    if data.get("release_tag") is not None or data.get("release_date") is not None:
        fail("unpublished candidate must not claim a release tag/date")

    source = data.get("source", {})
    if not re.fullmatch(r"[0-9a-f]{40}", source.get("commit", "")):
        fail("source commit must be a full immutable SHA")

    identity = data.get("package_identity", {})
    if identity.get("title_id_status") != "provisional":
        fail("Title ID must remain provisional until hardware conflict check")

    validation = data.get("validation", {})
    if validation.get("host_ctest") != "success; 10 of 10":
        fail("host CTest evidence is missing or unexpected")
    if validation.get("vpk_verifier_unit_tests") != "success; 9 of 9":
        fail("VPK verifier test evidence is missing or unexpected")
    if validation.get("vita_sdk_cross_build") != "success; VitaSDK 2026.08":
        fail("VitaSDK cross-build evidence is missing")
    if validation.get("vpk_integrity", "").startswith("success") is False:
        fail("VPK integrity evidence is missing")
    if validation.get("hardware_acceptance") != "pending; no physical Vita evidence for this exact artifact":
        fail("hardware acceptance must remain pending until a device report is recorded")

    artifacts = data.get("artifacts", [])
    if len(artifacts) != 1:
        fail("expected exactly one candidate CI artifact")
    artifact = artifacts[0]
    if not re.fullmatch(r"[0-9a-f]{64}", artifact.get("sha256", "")):
        fail("artifact archive SHA-256 must be a full digest")
    if "not the extracted VPK digest" not in artifact.get("digest_scope", ""):
        fail("artifact checksum scope must distinguish ZIP from VPK")

    nodes = data.get("nodes", [])
    if len(nodes) != 6 or any(node.get("included") is not False for node in nodes):
        fail("all six planned nodes must remain excluded from this foundation candidate")

    print("Release candidate manifest: valid; non-stable status and hardware gate preserved.")


if __name__ == "__main__":
    main()
