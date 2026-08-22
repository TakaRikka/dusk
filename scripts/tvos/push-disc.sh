#!/usr/bin/env bash
# Copy a disc image into the app's data container (<data dir>/discs) for builds made with --no-disc.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"

# Data dir = SDL pref path = Library/Caches/<org>/<app>/ ; discs/ underneath it is scanned by disc discovery.
# ASSUMPTION (org "Twilit Realm", app "Dusklight") — NOT yet confirmed on-device. Check the data path
# printed in the launch console output (scripts/tvos/launch.sh --console) and fix this one line if it differs.
TVOS_DATA_SUBDIR="Library/Caches/Twilit Realm/Dusklight"

disc="${1:?usage: push-disc.sh <disc image>}"
[[ -f "$disc" ]] || tvos_fail "not found: $disc"
udid="$(tvos_device_udid)"
dest="$TVOS_DATA_SUBDIR/discs/$(basename "$disc")"
xcrun devicectl device copy to --device "$udid" --domain-type appDataContainer --domain-identifier "$TVOS_BUNDLE_ID" \
    --source "$disc" --destination "$dest"
tvos_log "copied to $dest — relaunch the app to pick it up"
