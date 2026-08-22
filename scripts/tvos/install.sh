#!/usr/bin/env bash
# Install the signed app onto the Apple TV over the network.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"
app="${1:-$TVOS_INSTALL_APP}"
# Device first: a sleeping Apple TV needs a walk to the living room, while "not signed" is fixed
# by rerunning sign.sh at the keyboard -- surfacing the slow fix first lets both proceed in parallel.
udid="$(tvos_device_udid)"
tvos_require_device_reachable "$udid"
[[ -f "$app/embedded.mobileprovision" ]] || tvos_fail "$app is not signed — run scripts/tvos/sign.sh"
tvos_log "installing $(du -sh "$app" | cut -f1) to $TVOS_DEVICE_NAME ($udid) — large bundles take a few minutes"
# devicectl's own exit status decides, not the log text: "installed"/"success" can appear in
# progress lines of a run that later fails. -e is off just long enough to read PIPESTATUS[0].
set +e
xcrun devicectl device install app --device "$udid" "$app" 2>&1 | tee "$TVOS_LOG_DIR/install-device.log" | tail -5
rc=${PIPESTATUS[0]}
set -e
[[ $rc -eq 0 ]] || tvos_fail "install failed (devicectl exit $rc) — see $TVOS_LOG_DIR/install-device.log (is the Apple TV awake and on the same network?)"
tvos_log "installed $TVOS_BUNDLE_ID on $TVOS_DEVICE_NAME"
