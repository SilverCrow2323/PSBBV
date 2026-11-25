#!/usr/bin/env python3
"""Guard the single-product PSBBV release and standalone Core build boundary."""

from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class CoreReleaseArchitectureTests(unittest.TestCase):
    def test_core_does_not_fetch_or_link_development_workspaces(self):
        cmake = (ROOT / "CMakeLists.txt").read_text(encoding="utf-8")
        forbidden = (
            "FetchContent",
            "PSBBV_GAMES_SOURCE_DIR",
            "PSBBV_GAMES_GIT_TAG",
            "psbbv_games_vita_adapter",
            "FetchContent_MakeAvailable",
        )
        for token in forbidden:
            with self.subTest(token=token):
                self.assertNotIn(token, cmake)

    def test_application_shell_has_no_games_workspace_runtime_coupling(self):
        header = (ROOT / "include/psbbv/app/App.hpp").read_text(encoding="utf-8")
        source = (ROOT / "src/app/App.cpp").read_text(encoding="utf-8")
        for token in ("psbbv/games/", "GamesDiscovery", "games::", "psbbv_games"):
            with self.subTest(file="App.hpp", token=token):
                self.assertNotIn(token, header)
            with self.subTest(file="App.cpp", token=token):
                self.assertNotIn(token, source)

    def test_docs_declare_one_official_release_authority(self):
        readme = (ROOT / "README.md").read_text(encoding="utf-8")
        ecosystem = (ROOT / "docs/ECOSYSTEM.md").read_text(encoding="utf-8")
        release = (ROOT / "docs/CORE-RELEASE-MANIFEST.md").read_text(encoding="utf-8")
        self.assertIn("one official release stream", readme)
        self.assertIn("development workspaces", readme)
        self.assertIn("only PSBBV Core publishes an official release", readme)
        self.assertIn("only Vita application and the only PSBBV release authority", ecosystem)
        self.assertIn("do not publish independent PSBBV releases", release)


if __name__ == "__main__":
    unittest.main()
