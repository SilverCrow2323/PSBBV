import struct
import sys
import tempfile
import unittest
import warnings
import zipfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from verify_vpk import verify_vpk


def make_sfo(title_id="SCBV00001", version="00.10", title="PSBBV"):
    entries = [("TITLE_ID", title_id), ("APP_VER", version), ("TITLE", title)]
    keys = bytearray()
    values = bytearray()
    index = bytearray()
    for key, value in entries:
        key_offset = len(keys)
        keys.extend(key.encode("utf-8") + b"\x00")
        raw_value = value.encode("utf-8") + b"\x00"
        value_offset = len(values)
        values.extend(raw_value)
        index.extend(struct.pack("<HHIII", key_offset, 0x0204, len(raw_value), len(raw_value), value_offset))
    key_table_start = 20 + len(index)
    data_table_start = key_table_start + len(keys)
    header = struct.pack("<4sIIII", b"\x00PSF", 0x00000101, key_table_start, data_table_start, len(entries))
    return header + index + keys + values


def write_vpk(path, *, title_id="SCBV00001", version="00.10", title="PSBBV",
              missing=(), extra=(), bad_sfo=False, bad_sfo_magic=False,
              bad_xml=False, duplicate=False, symlink=False):
    files = {
        "eboot.bin": b"PSBBV-SELF",
        "sce_sys/param.sfo": (
            b"not-a-real-sfo" if bad_sfo else
            b"X" * 20 if bad_sfo_magic else
            make_sfo(title_id, version, title)
        ),
        "sce_sys/icon0.png": b"\x89PNG\r\n\x1a\nicon",
        "sce_sys/livearea/contents/bg.png": b"\x89PNG\r\n\x1a\nbg",
        "sce_sys/livearea/contents/startup.png": b"\x89PNG\r\n\x1a\nstartup",
        "sce_sys/livearea/contents/template.xml": b"<livearea><gate /></livearea>" if not bad_xml else b"<livearea>",
    }
    for name in missing:
        files.pop(name, None)
    with zipfile.ZipFile(path, "w", compression=zipfile.ZIP_DEFLATED) as archive:
        for name, content in files.items():
            archive.writestr(name, content)
        for name, content in extra:
            archive.writestr(name, content)
        if duplicate:
            with warnings.catch_warnings():
                warnings.simplefilter("ignore", UserWarning)
                archive.writestr("eboot.bin", b"duplicate")
        if symlink:
            info = zipfile.ZipInfo("sce_sys/untrusted-link")
            info.create_system = 3
            info.external_attr = (0o120777 << 16)
            archive.writestr(info, "target")


class VpkVerificationTests(unittest.TestCase):
    def verify(self, path):
        return verify_vpk(
            path,
            expected_title_id="SCBV00001",
            expected_version="00.10",
            expected_name="PSBBV",
        )

    def test_accepts_complete_vpk_and_reads_identity(self):
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "PSBBV.vpk"
            write_vpk(path)
            result = self.verify(path)
            self.assertEqual(result["title_id"], "SCBV00001")
            self.assertEqual(result["version"], "00.10")
            self.assertEqual(result["title"], "PSBBV")
            self.assertGreater(result["entries"], 5)

    def test_rejects_title_id_mismatch(self):
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "wrong-title.vpk"
            write_vpk(path, title_id="OTHER0001")
            with self.assertRaisesRegex(ValueError, "TITLE_ID"):
                self.verify(path)

    def test_rejects_missing_livearea_asset(self):
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "missing-asset.vpk"
            write_vpk(path, missing=("sce_sys/livearea/contents/startup.png",))
            with self.assertRaisesRegex(ValueError, "missing required files"):
                self.verify(path)

    def test_rejects_truncated_param_sfo(self):
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "truncated-sfo.vpk"
            write_vpk(path, bad_sfo=True)
            with self.assertRaisesRegex(ValueError, "is truncated"):
                self.verify(path)

    def test_rejects_invalid_param_sfo_magic(self):
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "bad-magic.vpk"
            write_vpk(path, bad_sfo_magic=True)
            with self.assertRaisesRegex(ValueError, "invalid PSF magic"):
                self.verify(path)

    def test_rejects_invalid_livearea_xml(self):
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "bad-xml.vpk"
            write_vpk(path, bad_xml=True)
            with self.assertRaisesRegex(ValueError, "invalid XML"):
                self.verify(path)

    def test_rejects_duplicate_entries(self):
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "duplicate.vpk"
            write_vpk(path, duplicate=True)
            with self.assertRaisesRegex(ValueError, "duplicate ZIP entries"):
                self.verify(path)

    def test_rejects_path_traversal_and_bundled_game_data(self):
        for extra, expected in (
            (("../outside.txt", b"no"), "unsafe path"),
            (("game.iso", b"no"), "must not bundle game data"),
        ):
            with self.subTest(extra=extra):
                with tempfile.TemporaryDirectory() as temporary:
                    path = Path(temporary) / "unsafe.vpk"
                    write_vpk(path, extra=(extra,))
                    with self.assertRaisesRegex(ValueError, expected):
                        self.verify(path)

    def test_rejects_symbolic_links(self):
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "symlink.vpk"
            write_vpk(path, symlink=True)
            with self.assertRaisesRegex(ValueError, "symbolic link"):
                self.verify(path)


if __name__ == "__main__":
    unittest.main()
