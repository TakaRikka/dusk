#!/usr/bin/env bash
# Copy a disc image into the app's data container (<data dir>/discs) for builds made with --no-disc.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"

# Data dir = SDL pref path = Library/Caches/<orgName>/<appName>/ ; discs/ underneath it is scanned
# by disc discovery. orgName/appName come from dusk::AppInfo (src/dusk/app_info.hpp) and are passed
# straight to SDL_GetPrefPath by borealis (extern/borealis/src/data.cpp), which on tvOS formats
# "<NSCachesDirectory>/<org>/<app>/". Derived, not yet confirmed on-device: check the data path in
# the launch console output (scripts/tvos/launch.sh --console) and fix this one line if it differs.
TVOS_DATA_SUBDIR="Library/Caches/TwilitRealm/Dusklight"

disc="${1:?usage: push-disc.sh <disc image>}"
[[ -f "$disc" ]] || tvos_fail "not found: $disc"
udid="$(tvos_device_udid)"
tvos_require_device_reachable "$udid"
dest="$TVOS_DATA_SUBDIR/discs/$(basename "$disc")"
xcrun devicectl device copy to --device "$udid" --domain-type appDataContainer --domain-identifier "$TVOS_BUNDLE_ID" \
    --source "$disc" --destination "$dest"
tvos_log "copied to $dest — relaunch the app to pick it up"
