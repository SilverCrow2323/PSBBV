#!/usr/bin/env python3
"""Validate the PSBBV 0.1.0-rc.1 manifest and its immutable CI provenance."""

import json
import re
import sys
from pathlib import Path
from typing import Any, Dict

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "docs/releases/v0.1.0-rc.1-manifest.json"
RUN_URL = re.compile(r"^https://github\\.com/SilverCrow2323/PSBBV/actions/runs/(\\d+)$")
ARTIFACT_URL = re.compile(
    r"^https://github\\.com/SilverCrow2323/PSBBV/actions/runs/(\\d+)/artifacts/(\\d+)$"
)
SHA1 = re.compile(r"^[0-9a-f]{40}$")
SHA256 = re.compile(r"^[0-9a-f]{64}$")


class ManifestError(ValueError):
    """The candidate record is internally inconsistent."""


def require(condition: bool, message: str) -> None:
    if not condition:
        raise ManifestError(message)


def validate_manifest(data: Dict[str, Any]) -> None:
    """Validate status gates and cross-field provenance without network access."""
    require(data.get("manifest_schema") == "psbbv.core-release/v1", "unexpected manifest_schema")
    require(data.get("core_version") == "0.1.0-rc.1", "candidate version must remain 0.1.0-rc.1")
    require(data.get("channel") == "release-candidate", "channel must be release-candidate")
    require(data.get("official_release") is False, "candidate must never claim official_release=true")
    require(data.get("release_tag") is None and data.get("release_date") is None,
            "unpublished candidate must not claim a release tag/date")

    source = data.get("source", {})
    require(source.get("repository") == "SilverCrow2323/PSBBV", "unexpected source repository")
    require(source.get("branch") == "dev/foundation", "candidate source must remain on dev/foundation")
    source_commit = source.get("commit", "")
    require(bool(SHA1.fullmatch(source_commit)), "source commit must be a full immutable SHA")

    identity = data.get("package_identity", {})
    require(identity.get("title_id_status") == "provisional",
            "Title ID must remain provisional until hardware conflict check")

    validation = data.get("validation", {})
    run_url = validation.get("workflow_run", "")
    run_match = RUN_URL.fullmatch(run_url)
    require(run_match is not None, "workflow_run must be the canonical PSBBV Actions run URL")
    run_id = int(run_match.group(1))
    require(validation.get("workflow_head_commit") == source_commit,
            "workflow head commit must equal the artifact source commit")
    require(validation.get("host_ctest") == "success; 10 of 10", "host CTest evidence is missing or unexpected")
    require(validation.get("vpk_verifier_unit_tests") == "success; 9 of 9",
            "VPK verifier test evidence is missing or unexpected")
    require(validation.get("vita_sdk_cross_build") == "success; VitaSDK 2026.08",
            "VitaSDK cross-build evidence is missing")
    require(str(validation.get("vpk_integrity", "")).startswith("success"),
            "VPK integrity evidence is missing")
    require(validation.get("hardware_acceptance") ==
            "pending; no physical Vita evidence for this exact artifact",
            "hardware acceptance must remain pending until a device report is recorded")

    artifacts = data.get("artifacts", [])
    require(isinstance(artifacts, list) and len(artifacts) == 1,
            "expected exactly one candidate CI artifact")
    artifact = artifacts[0]
    require(artifact.get("name") == "PSBBV-VPK", "unexpected Actions artifact name")
    require(isinstance(artifact.get("size_bytes"), int) and artifact["size_bytes"] > 0,
            "artifact ZIP size must be a positive integer")
    archive_sha = artifact.get("sha256", "")
    require(bool(SHA256.fullmatch(archive_sha)), "artifact archive SHA-256 must be a full digest")
    require(artifact.get("digest_scope") ==
            "downloaded GitHub Actions artifact ZIP; not the extracted VPK digest",
            "artifact checksum scope must identify the downloaded ZIP")

    artifact_run_id = artifact.get("workflow_run_id")
    require(artifact_run_id == run_id, "artifact workflow_run_id must equal validation workflow run")
    artifact_id = artifact.get("artifact_id")
    require(isinstance(artifact_id, int) and artifact_id > 0, "artifact_id must be a positive integer")
    url_match = ARTIFACT_URL.fullmatch(artifact.get("url", ""))
    require(url_match is not None, "artifact URL must be a canonical run/artifact URL")
    require(int(url_match.group(1)) == run_id, "artifact URL run ID does not match validation workflow")
    require(int(url_match.group(2)) == artifact_id, "artifact URL ID does not match artifact_id")

    extracted = artifact.get("extracted_vpk", {})
    require(extracted.get("name") == "PSBBV.vpk", "extracted package must be PSBBV.vpk")
    vpk_sha = extracted.get("sha256", "")
    require(bool(SHA256.fullmatch(vpk_sha)), "extracted VPK SHA-256 must be a full digest")
    require(extracted.get("digest_scope") == "SHA-256 of extracted VPK file bytes",
            "extracted VPK checksum scope is missing")

    checksum_rows = data.get("checksums", [])
    expected = {
        "downloaded GitHub Actions artifact ZIP; not the extracted VPK digest": archive_sha,
        "extracted PSBBV.vpk file bytes": vpk_sha,
    }
    actual = {
        row.get("scope"): row.get("value")
        for row in checksum_rows
        if row.get("algorithm") == "sha256"
    }
    require(actual == expected, "checksums must match both artifact ZIP and extracted VPK digests")

    nodes = data.get("nodes", [])
    required_nodes = {"games", "applications", "network", "media", "downloads", "system"}
    require(len(nodes) == 6 and {node.get("id") for node in nodes} == required_nodes,
            "candidate must record all six canonical node identities")
    require(all(node.get("included") is False for node in nodes),
            "planned nodes must remain excluded from this foundation candidate")


def main() -> None:
    try:
        data = json.loads(MANIFEST.read_text(encoding="utf-8"))
        validate_manifest(data)
    except (OSError, json.JSONDecodeError, ManifestError) as exc:
        print(f"Release candidate manifest: ERROR: {exc}", file=sys.stderr)
        raise SystemExit(1)

    print("Release candidate manifest: valid; source, run, artifact, digests and hardware gate are consistent.")


if __name__ == "__main__":
    main()
