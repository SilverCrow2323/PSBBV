import copy
import json
import unittest
from pathlib import Path

from tools.validate_release_candidate import ManifestError, validate_manifest

ROOT = Path(__file__).resolve().parents[1]
FIXTURE = json.loads((ROOT / "docs/releases/v0.1.0-rc.1-manifest.json").read_text(encoding="utf-8"))


class ReleaseCandidateManifestTests(unittest.TestCase):
    def test_current_candidate_manifest_is_consistent(self):
        validate_manifest(copy.deepcopy(FIXTURE))

    def test_human_readable_candidate_documents_match_manifest_provenance(self):
        artifact = FIXTURE["artifacts"][0]
        source_commit = FIXTURE["source"]["commit"]
        run_id = str(artifact["workflow_run_id"])
        artifact_id = str(artifact["artifact_id"])
        zip_sha = artifact["sha256"]
        vpk_sha = artifact["extracted_vpk"]["sha256"]
        documents = [
            ROOT / "docs/releases/v0.1.0-rc.1.md",
            ROOT / "docs/TESTING.md",
            ROOT / "docs/CORE-RELEASE-MANIFEST.md",
        ]
        for path in documents:
            content = path.read_text(encoding="utf-8")
            with self.subTest(document=path.name):
                self.assertIn(source_commit, content)
                self.assertIn(run_id, content)
                self.assertIn(artifact_id, content)
                self.assertIn(zip_sha, content)
                self.assertIn(vpk_sha, content)

    def assert_rejected_after(self, mutate):
        payload = copy.deepcopy(FIXTURE)
        mutate(payload)
        with self.assertRaises(ManifestError):
            validate_manifest(payload)

    def test_rejects_workflow_head_mismatch(self):
        self.assert_rejected_after(lambda d: d["validation"].update(workflow_head_commit="0" * 40))

    def test_rejects_artifact_run_mismatch(self):
        self.assert_rejected_after(lambda d: d["artifacts"][0].update(workflow_run_id=1))

    def test_rejects_artifact_url_mismatch(self):
        self.assert_rejected_after(lambda d: d["artifacts"][0].update(url="https://github.com/SilverCrow2323/PSBBV/actions/runs/1/artifacts/2"))

    def test_rejects_artifact_id_mismatch(self):
        self.assert_rejected_after(lambda d: d["artifacts"][0].update(artifact_id=1))

    def test_rejects_zip_digest_mismatch(self):
        self.assert_rejected_after(lambda d: d["checksums"][0].update(value="0" * 64))

    def test_rejects_extracted_vpk_digest_mismatch(self):
        self.assert_rejected_after(lambda d: d["artifacts"][0]["extracted_vpk"].update(sha256="0" * 64))

    def test_rejects_candidate_source_commit_mismatch(self):
        self.assert_rejected_after(lambda d: d["source"].update(commit="0" * 40))


if __name__ == "__main__":
    unittest.main()
