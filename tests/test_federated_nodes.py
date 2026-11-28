"""Validate that the PSBBV Core README keeps the federated node map complete."""
from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[1]
README = (ROOT / "README.md").read_text(encoding="utf-8")
EXPECTED = {
    "PSBBV": "https://github.com/SilverCrow2323/PSBBV",
    "PSBBV-Games": "https://github.com/SilverCrow2323/PSBBV-Games",
    "PSBBV-Applications": "https://github.com/SilverCrow2323/PSBBV-Applications",
    "PSBBV-Media": "https://github.com/SilverCrow2323/PSBBV-Media",
    "PSBBV-Network": "https://github.com/SilverCrow2323/PSBBV-Network",
    "PSBBV-Downloads": "https://github.com/SilverCrow2323/PSBBV-Downloads",
    "PSBBV-System": "https://github.com/SilverCrow2323/PSBBV-System",
}


class FederatedNodeMapTests(unittest.TestCase):
    def test_every_node_repository_is_linked(self):
        for name, url in EXPECTED.items():
            with self.subTest(repository=name):
                self.assertIn(url, README)

    def test_node_repository_names_are_not_duplicated_in_the_map(self):
        for name in EXPECTED:
            count = README.count(f"https://github.com/SilverCrow2323/{name})")
            self.assertEqual(count, 1, name)

    def test_core_disclaimer_is_present(self):
        self.assertRegex(README.lower(), re.compile(r"not affiliated.*sony|not.*endorsed.*sony", re.S))
        self.assertIn("does not mean its feature is already wired", README)


if __name__ == "__main__":
    unittest.main()
