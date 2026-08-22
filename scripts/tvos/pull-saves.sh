#!/usr/bin/env bash
# Back up the app's data container (settings + saves live under Library/Caches on tvOS).
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"
udid="$(tvos_device_udid)"
dest="${1:-$TVOS_FORK_ROOT/../backups/dusklight-tvos-$(date +%Y%m%d-%H%M%S)}"
mkdir -p "$dest"
xcrun devicectl device copy from --device "$udid" --domain-type appDataContainer --domain-identifier "$TVOS_BUNDLE_ID" \
    --source Library/Caches --destination "$dest/Caches"
tvos_log "backup written to $dest ($(du -sh "$dest" | cut -f1))"
find "$dest" -type f | head -20
