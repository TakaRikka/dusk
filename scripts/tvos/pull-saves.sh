#!/usr/bin/env bash
# Back up the app's data container (settings + saves live under Library/Caches on tvOS).
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"
udid="$(tvos_device_udid)"
tvos_require_device_reachable "$udid"
dest="${1:-$TVOS_FORK_ROOT/../backups/dusklight-tvos-$(date +%Y%m%d-%H%M%S)}"
mkdir -p "$dest"
xcrun devicectl device copy from --device "$udid" --domain-type appDataContainer --domain-identifier "$TVOS_BUNDLE_ID" \
    --source Library/Caches --destination "$dest/Caches"
tvos_log "backup written to $dest ($(du -sh "$dest" | cut -f1))"
# `| head -20` closes the pipe under a still-running find, which dies of SIGPIPE; under
# `set -o pipefail` that made a *successful* backup exit 141. sed reads its input to the end, so
# nothing is signalled and the script's status is the backup's.
find "$dest" -type f | sed -n '1,20p'
