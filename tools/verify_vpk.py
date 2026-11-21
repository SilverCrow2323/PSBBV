#!/usr/bin/env python3
"""Read-only structural and identity checks for the PSBBV Vita VPK."""

from __future__ import annotations

import argparse
import stat
import struct
import sys
import zipfile
from pathlib import Path, PurePosixPath
from xml.etree import ElementTree

SFO_MAGIC = b"\x00PSF"
REQUIRED_FILES = {
    "eboot.bin",
    "sce_sys/param.sfo",
    "sce_sys/icon0.png",
    "sce_sys/livearea/contents/bg.png",
    "sce_sys/livearea/contents/startup.png",
    "sce_sys/livearea/contents/template.xml",
}
PNG_SIGNATURE = b"\x89PNG\r\n\x1a\n"
FORBIDDEN_GAME_SUFFIXES = {
    ".iso", ".cso", ".pbp", ".chd", ".cue", ".gcm", ".rvz",
    ".gcz", ".wbfs", ".wia", ".rom",
}


def parse_param_sfo(data: bytes) -> dict[str, object]:
    if len(data) < 20:
        raise ValueError("sce_sys/param.sfo is truncated")
    magic, _version, key_table_start, data_table_start, entry_count = struct.unpack_from(
        "<4sIIII", data, 0
    )
    if magic != SFO_MAGIC:
        raise ValueError("sce_sys/param.sfo has an invalid PSF magic")
    index_start = 20
    index_end = index_start + entry_count * 16
    if entry_count > 4096 or index_end > len(data):
        raise ValueError("sce_sys/param.sfo has an invalid index table")
    if not (index_end <= key_table_start <= data_table_start <= len(data)):
        raise ValueError("sce_sys/param.sfo has invalid table offsets")

    values: dict[str, object] = {}
    for index in range(entry_count):
        key_offset, value_format, length, max_length, value_offset = struct.unpack_from(
            "<HHIII", data, index_start + index * 16
        )
        key_start = key_table_start + key_offset
        if key_start < key_table_start or key_start >= data_table_start:
            raise ValueError("sce_sys/param.sfo contains an out-of-range key offset")
        key_end = data.find(b"\x00", key_start, data_table_start)
        if key_end < 0:
            raise ValueError("sce_sys/param.sfo contains an unterminated key")
        try:
            key = data[key_start:key_end].decode("utf-8")
        except UnicodeDecodeError as error:
            raise ValueError("sce_sys/param.sfo contains a non-UTF-8 key") from error
        value_start = data_table_start + value_offset
        if length > max_length or value_start + length > len(data):
            raise ValueError(f"sce_sys/param.sfo value {key!r} exceeds the data table")
        if key in values:
            raise ValueError(f"sce_sys/param.sfo contains duplicate key {key!r}")
        raw = data[value_start:value_start + length]
        if value_format == 0x0204:
            try:
                values[key] = raw.rstrip(b"\x00").decode("utf-8")
            except UnicodeDecodeError as error:
                raise ValueError(f"sce_sys/param.sfo value {key!r} is not UTF-8") from error
        elif value_format == 0x0404 and length == 4:
            values[key] = struct.unpack("<I", raw)[0]
        else:
            values[key] = raw
    return values


def verify_vpk(
    vpk_path: Path,
    *,
    expected_title_id: str,
    expected_version: str | None = None,
    expected_name: str | None = None,
) -> dict[str, object]:
    try:
        archive = zipfile.ZipFile(vpk_path, "r")
    except (OSError, zipfile.BadZipFile) as error:
        raise ValueError(f"VPK is not a readable ZIP archive: {error}") from error

    with archive:
        infos = archive.infolist()
        names = [info.filename for info in infos]
        if len(names) != len(set(names)):
            raise ValueError("VPK contains duplicate ZIP entries")
        if not infos:
            raise ValueError("VPK is empty")

        for info in infos:
            name = info.filename
            path = PurePosixPath(name)
            parts = name.rstrip("/").split("/")
            if (
                not name
                or "\\" in name
                or path.is_absolute()
                or any(part in ("", ".", "..") for part in parts)
            ):
                raise ValueError(f"VPK contains an unsafe path: {name!r}")
            mode = (info.external_attr >> 16) & 0o170000
            if mode == stat.S_IFLNK:
                raise ValueError(f"VPK contains a symbolic link: {name!r}")
            if not info.is_dir() and path.suffix.lower() in FORBIDDEN_GAME_SUFFIXES:
                raise ValueError(f"VPK must not bundle game data: {name!r}")
            if path.name.lower() == "disc-image.txt":
                raise ValueError("VPK must not bundle a disc-image selection file")

        corrupt_entry = archive.testzip()
        if corrupt_entry is not None:
            raise ValueError(f"VPK CRC validation failed for {corrupt_entry!r}")

        missing = sorted(REQUIRED_FILES.difference(names))
        if missing:
            raise ValueError("VPK is missing required files: " + ", ".join(missing))

        for name in REQUIRED_FILES:
            if archive.getinfo(name).file_size <= 0:
                raise ValueError(f"VPK required file is empty: {name}")

        for name in (
            "sce_sys/icon0.png",
            "sce_sys/livearea/contents/bg.png",
            "sce_sys/livearea/contents/startup.png",
        ):
            if not archive.read(name).startswith(PNG_SIGNATURE):
                raise ValueError(f"VPK asset is not a PNG: {name}")

        try:
            ElementTree.fromstring(archive.read("sce_sys/livearea/contents/template.xml"))
        except ElementTree.ParseError as error:
            raise ValueError("VPK LiveArea template.xml is invalid XML") from error

        metadata = parse_param_sfo(archive.read("sce_sys/param.sfo"))
        title_id = metadata.get("TITLE_ID")
        if title_id != expected_title_id:
            raise ValueError(
                f"VPK TITLE_ID is {title_id!r}; expected {expected_title_id!r}"
            )
        version = metadata.get("APP_VER")
        if expected_version is not None and version != expected_version:
            raise ValueError(
                f"VPK APP_VER is {version!r}; expected {expected_version!r}"
            )
        title = metadata.get("TITLE")
        if expected_name is not None and title != expected_name:
            raise ValueError(
                f"VPK TITLE is {title!r}; expected {expected_name!r}"
            )
        return {
            "title_id": title_id,
            "version": version,
            "title": title,
            "entries": len(infos),
            "bytes": sum(info.file_size for info in infos if not info.is_dir()),
        }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("vpk", type=Path)
    parser.add_argument("--title-id", required=True)
    parser.add_argument("--version")
    parser.add_argument("--name")
    args = parser.parse_args()
    try:
        result = verify_vpk(
            args.vpk,
            expected_title_id=args.title_id,
            expected_version=args.version,
            expected_name=args.name,
        )
    except ValueError as error:
        print(f"PSBBV VPK validation failed: {error}", file=sys.stderr)
        return 1
    print(
        "PSBBV VPK OK: "
        f"TITLE_ID={result['title_id']} APP_VER={result['version']} "
        f"TITLE={result['title']} entries={result['entries']} "
        f"payload_bytes={result['bytes']}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
