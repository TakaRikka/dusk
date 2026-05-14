#!/usr/bin/env python3
"""Validate and optionally repair Dusklight UI i18n token dictionaries.

The historical-value repair mode compares XML values against the UI string
literals that existed before the i18n-token commit. It is intentionally
conservative: if several old strings map to the same token key, the script
reports the ambiguity instead of guessing.
"""

from __future__ import annotations

import argparse
import html
import re
import subprocess
import sys
import xml.etree.ElementTree as ET
from pathlib import Path


DEFAULT_BASE_REF = "a8f7e3ffa0^"
LANGUAGE_FILES = ("en.xml", "zh-cn.xml", "fr.xml", "ja.xml")


def decode_cpp_literal(content: str) -> str:
    out: list[str] = []
    i = 0
    while i < len(content):
        ch = content[i]
        if ch != "\\" or i + 1 >= len(content):
            out.append(ch)
            i += 1
            continue

        nxt = content[i + 1]
        escapes = {
            "n": "\n",
            "r": "\r",
            "t": "\t",
            "\\": "\\",
            '"': '"',
            "'": "'",
            "0": "\0",
        }
        out.append(escapes.get(nxt, nxt))
        i += 2
    return "".join(out)


def extract_cpp_strings(source: str) -> list[str]:
    strings: list[str] = []
    # C++ concatenates adjacent string literals. This catches simple and raw
    # UI literals used in these files without trying to parse full C++.
    literal_group = re.compile(r'"(?:\\.|[^"\\])*"(?:\s*"(?:\\.|[^"\\])*")*')
    literal_part = re.compile(r'"((?:\\.|[^"\\])*)"')
    for match in literal_group.finditer(source):
        parts = literal_part.findall(match.group(0))
        value = "".join(decode_cpp_literal(part) for part in parts)
        if should_consider_history_string(value):
            strings.append(value)
    return strings


def should_consider_history_string(value: str) -> bool:
    if not value:
        return False
    if value != value.strip():
        return False
    if value in {"(none)"}:
        return False
    if "{}" in value:
        return False
    if value.endswith(("(", "[", "{")):
        return False
    if value.startswith(("res/", "http://", "https://")):
        return False
    if value in {"en", "zh-cn", "fr", "ja"}:
        return False
    if value in {"auto", "d3d11", "d3d12", "metal", "vulkan", "opengl", "opengles", "webgpu", "null"}:
        return False
    if re.fullmatch(r"[A-Za-z0-9_.:/\\-]+", value) and not re.search(r"[A-Z][a-z]| ", value):
        return False
    return True


def token_key(value: str) -> str:
    return re.sub(r"[^A-Za-z0-9]+", "_", value.upper()).strip("_")[:80]


def git_show(ref: str, path: str) -> str | None:
    try:
        return subprocess.check_output(
            ["git", "show", f"{ref}:{path}"],
            text=True,
            encoding="utf-8",
            errors="replace",
            stderr=subprocess.DEVNULL,
        )
    except subprocess.CalledProcessError:
        return None


def git_list_files(ref: str, pathspec: str) -> list[str]:
    try:
        output = subprocess.check_output(
            ["git", "ls-tree", "-r", "--name-only", ref, pathspec],
            text=True,
            encoding="utf-8",
            errors="replace",
            stderr=subprocess.DEVNULL,
        )
    except subprocess.CalledProcessError:
        return []
    return [
        line
        for line in output.splitlines()
        if Path(line).suffix in {".cpp", ".hpp"}
    ]


def load_entries(path: Path) -> list[tuple[str, str]]:
    text = path.read_text(encoding="utf-8")
    return re.findall(r'<entry key="([^"]+)">(.*?)</entry>', text, flags=re.S)


def load_raw_entry_map(path: Path) -> dict[str, str]:
    return dict(load_entries(path))


def load_entry_map(path: Path) -> dict[str, str]:
    return {key: html.unescape(value) for key, value in load_entries(path)}


def escape_xml_inner_preserving_rml(value: str) -> str:
    # Entry values intentionally include RML tags such as <br/> and <span>.
    # Preserve those tags, but escape raw ampersands so the i18n XML remains
    # parseable.
    return re.sub(r"&(?!#?[A-Za-z0-9]+;)", "&amp;", value)


def replace_entry_values(path: Path, replacements: dict[str, str]) -> int:
    text = path.read_text(encoding="utf-8")
    changed = 0

    def repl(match: re.Match[str]) -> str:
        nonlocal changed
        key = match.group(1)
        if key not in replacements:
            return match.group(0)
        changed += 1
        return f'<entry key="{key}">{escape_xml_inner_preserving_rml(replacements[key])}</entry>'

    text = re.sub(r'<entry key="([^"]+)">.*?</entry>', repl, text, flags=re.S)
    if changed:
        path.write_text(text, encoding="utf-8", newline="")
    return changed


def replace_entry_raw_values(path: Path, replacements: dict[str, str]) -> int:
    text = path.read_text(encoding="utf-8")
    changed = 0

    def repl(match: re.Match[str]) -> str:
        nonlocal changed
        key = match.group(1)
        if key not in replacements:
            return match.group(0)
        new_value = replacements[key]
        if match.group(2) != new_value:
            changed += 1
        return f'<entry key="{key}">{new_value}</entry>'

    text = re.sub(r'<entry key="([^"]+)">(.*?)</entry>', repl, text, flags=re.S)
    if changed:
        path.write_text(text, encoding="utf-8", newline="")
    return changed


def source_tokens(root: Path) -> set[str]:
    tokens: set[str] = set()
    for path in (root / "src/dusk/ui").rglob("*"):
        if path.suffix not in {".cpp", ".hpp"}:
            continue
        text = path.read_text(encoding="utf-8", errors="replace")
        for literal in extract_cpp_strings(text):
            tokens.update(re.findall(r"\[([A-Z][A-Z0-9_]*)\]", literal))
    return tokens


def history_candidates(ref: str, files: list[str]) -> dict[str, set[str]]:
    candidates: dict[str, set[str]] = {}
    for file in files:
        source = git_show(ref, file)
        if source is None:
            continue
        for value in extract_cpp_strings(source):
            candidates.setdefault(token_key(value), set()).add(value)
    return candidates


def check_xml_files(i18n_dir: Path) -> int:
    errors = 0
    base_keys: set[str] | None = None
    for filename in LANGUAGE_FILES:
        path = i18n_dir / filename
        try:
            ET.parse(path)
        except ET.ParseError as exc:
            print(f"XML_PARSE_ERROR {path}: {exc}")
            errors += 1
            continue

        keys = [key for key, _ in load_entries(path)]
        duplicates = sorted(key for key in set(keys) if keys.count(key) > 1)
        if duplicates:
            print(f"DUPLICATE_KEYS {path}: {', '.join(duplicates)}")
            errors += len(duplicates)

        key_set = set(keys)
        if base_keys is None:
            base_keys = key_set
        elif key_set != base_keys:
            missing = sorted(base_keys - key_set)
            extra = sorted(key_set - base_keys)
            print(f"KEY_SET_DIFF {path}: missing={missing} extra={extra}")
            errors += len(missing) + len(extra)
    return errors


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path.cwd())
    parser.add_argument("--base-ref", default=DEFAULT_BASE_REF)
    parser.add_argument(
        "--history-file",
        action="append",
        dest="history_files",
        help="Limit historical text comparison to a source file. Can be repeated.",
    )
    parser.add_argument("--apply-history-values", action="store_true")
    parser.add_argument("--sync-languages", action="store_true")
    args = parser.parse_args()

    root = args.root.resolve()
    i18n_dir = root / "res/i18n"
    en_path = i18n_dir / "en.xml"

    errors = check_xml_files(i18n_dir)

    en_entries = load_entry_map(en_path)
    missing = sorted(source_tokens(root) - set(en_entries))
    if missing:
        print(f"MISSING_EN_KEYS {len(missing)}:")
        for key in missing:
            print(f"  {key}")
        errors += len(missing)

    history_files = args.history_files or git_list_files(args.base_ref, "src/dusk/ui")
    candidates = history_candidates(args.base_ref, history_files)
    repairs: dict[str, str] = {}
    ambiguous: dict[str, set[str]] = {}
    for key, current in en_entries.items():
        values = candidates.get(key)
        if not values:
            continue
        if current in values:
            continue
        if len(values) == 1:
            repairs[key] = next(iter(values))
        else:
            ambiguous[key] = values

    if repairs:
        print(f"HISTORY_VALUE_DIFF {len(repairs)}:")
        for key, expected in sorted(repairs.items()):
            print(f"  {key}: {en_entries[key]!r} -> {expected!r}")
        if args.apply_history_values:
            changed = replace_entry_values(en_path, repairs)
            print(f"APPLIED en.xml replacements: {changed}")

    if ambiguous:
        print(f"AMBIGUOUS_HISTORY_KEYS {len(ambiguous)}:")
        for key, values in sorted(ambiguous.items()):
            print(f"  {key}: {sorted(values)!r}")

    if args.sync_languages:
        current_en = load_raw_entry_map(en_path)
        for filename in LANGUAGE_FILES[1:]:
            path = i18n_dir / filename
            changed = replace_entry_raw_values(path, current_en)
            print(f"SYNCED {filename}: {changed}")

    if errors:
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
