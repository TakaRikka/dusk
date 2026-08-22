#!/usr/bin/env bash
# Restore a backup made by pull-saves.sh: scripts/tvos/push-saves.sh <backup dir>
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"
src="${1:?usage: push-saves.sh <backup dir containing Caches/>}"
[[ -d "$src/Caches" ]] || tvos_fail "$src/Caches not found"
udid="$(tvos_device_udid)"
tvos_require_device_reachable "$udid"
xcrun devicectl device copy to --device "$udid" --domain-type appDataContainer --domain-identifier "$TVOS_BUNDLE_ID" \
    --source "$src/Caches" --destination Library/Caches
tvos_log "restored $src to the device"
