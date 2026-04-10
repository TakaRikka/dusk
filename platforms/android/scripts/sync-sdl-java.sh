#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/../../.." && pwd)"
SRC_DEFAULT="$ROOT_DIR/build/android-arm64/_deps/sdl-src/android-project/app/src/main/java/org/libsdl/app"
SRC_DIR="${1:-$SRC_DEFAULT}"
DST_DIR="$ROOT_DIR/platforms/android/app/src/main/java/org/libsdl/app"

if [[ ! -d "$SRC_DIR" ]]; then
  echo "SDL Java source directory not found: $SRC_DIR" >&2
  exit 1
fi

mkdir -p "$DST_DIR"
cp -f "$SRC_DIR"/*.java "$DST_DIR"/
echo "Synced SDL Java sources from $SRC_DIR to $DST_DIR"
