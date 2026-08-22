#!/usr/bin/env bash
# Launch the app on the Apple TV. --console streams stdout/stderr (Ctrl-C to stop streaming).
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"
udid="$(tvos_device_udid)"
console=""
[[ "${1:-}" == "--console" ]] && console="--console"
tvos_log "launching $TVOS_BUNDLE_ID on $TVOS_DEVICE_NAME"
xcrun devicectl device process launch --device "$udid" --terminate-existing ${console:+"$console"} "$TVOS_BUNDLE_ID"
