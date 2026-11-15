#!/usr/bin/env python3
"""Validate the PSBBV node manifest schema and representative positive/negative fixtures."""

import copy
import json
from pathlib import Path

from jsonschema import Draft202012Validator

ROOT = Path(__file__).resolve().parents[1]
SCHEMA_PATH = ROOT / "schemas" / "psbbv-node-manifest.schema.json"

VALID_MANIFEST = {
    "manifestVersion": 1,
    "id": "factory.spdw.psbbv.example",
    "name": "Example Node",
    "version": "1.0.0",
    "apiVersion": "1.0",
    "description": "A schema validation fixture.",
    "category": "applications",
    "platforms": ["psvita"],
    "capabilities": ["storage.private"],
    "permissions": ["storage.private"],
    "availability": "planned",
    "runtime": {"kind": "builtin"},
}


def expect_valid(validator, manifest, label):
    errors = list(validator.iter_errors(manifest))
    if errors:
        raise AssertionError(f"{label} should be valid: " + "; ".join(e.message for e in errors))


def expect_invalid(validator, manifest, label):
    errors = list(validator.iter_errors(manifest))
    if not errors:
        raise AssertionError(f"{label} should have been rejected")


def main():
    schema = json.loads(SCHEMA_PATH.read_text(encoding="utf-8"))
    Draft202012Validator.check_schema(schema)
    validator = Draft202012Validator(schema)

    expect_valid(validator, VALID_MANIFEST, "minimal built-in manifest")

    cases = []
    missing_required = copy.deepcopy(VALID_MANIFEST)
    del missing_required["apiVersion"]
    cases.append(("missing required field", missing_required))

    invalid_id = copy.deepcopy(VALID_MANIFEST)
    invalid_id["id"] = "Not A Stable ID"
    cases.append(("invalid stable ID", invalid_id))

    duplicate_platform = copy.deepcopy(VALID_MANIFEST)
    duplicate_platform["platforms"] = ["psvita", "psvita"]
    cases.append(("duplicate platform", duplicate_platform))

    unknown_permission = copy.deepcopy(VALID_MANIFEST)
    unknown_permission["permissions"] = ["filesystem.unrestricted"]
    cases.append(("unknown permission", unknown_permission))

    unknown_property = copy.deepcopy(VALID_MANIFEST)
    unknown_property["unreviewedFlag"] = True
    cases.append(("unknown top-level property", unknown_property))

    malformed_runtime = copy.deepcopy(VALID_MANIFEST)
    malformed_runtime["runtime"] = {"kind": "builtin", "unexpected": True}
    cases.append(("unknown runtime property", malformed_runtime))

    for label, manifest in cases:
        expect_invalid(validator, manifest, label)

    print(f"Node manifest schema: valid fixture accepted; {len(cases)} invalid fixtures rejected.")


if __name__ == "__main__":
    main()
