#!/usr/bin/env bash
# Launch the app on the Apple TV. Usage:
#   scripts/tvos/launch.sh                       launch and return immediately
#   scripts/tvos/launch.sh --console             launch and stream stdout/stderr until Ctrl-C
#   scripts/tvos/launch.sh --console-seconds N   stream for N seconds, then detach
# --console-seconds exists because this Mac has neither timeout(1) nor gtimeout(1), so
# `timeout 60 scripts/tvos/launch.sh --console` is not available. Detaching only stops the
# stream; the app keeps running on the device.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"

usage() {
    cat <<'USAGE'
usage: scripts/tvos/launch.sh [--console | --console-seconds N]
  (no flag)            launch and return immediately
  --console            launch and stream stdout/stderr until Ctrl-C
  --console-seconds N  stream for N seconds, then detach (the app keeps running)
USAGE
}

console=0
seconds=""
while [[ $# -gt 0 ]]; do
    case "$1" in
        --console) console=1; shift ;;
        --console-seconds)
            [[ $# -ge 2 ]] || tvos_fail "--console-seconds needs a number of seconds"
            seconds="$2"; console=1; shift 2 ;;
        -h|--help) usage; exit 0 ;;
        *) usage >&2; tvos_fail "unknown argument: $1" ;;
    esac
done
if [[ -n "$seconds" ]]; then
    [[ "$seconds" =~ ^[0-9]+$ && "$seconds" -gt 0 ]] \
        || tvos_fail "--console-seconds wants a positive whole number of seconds, got '$seconds'"
fi

udid="$(tvos_device_udid)"
tvos_require_device_reachable "$udid"
tvos_log "launching $TVOS_BUNDLE_ID on $TVOS_DEVICE_NAME"

if [[ -n "$seconds" ]]; then
    log="$TVOS_LOG_DIR/launch-console.log"
    tvos_log "streaming the console for ${seconds}s -> $log"
    # Backgrounded without a pipeline so $! is devicectl's own pid; the process substitution
    # keeps the stream on the terminal while also recording it.
    xcrun devicectl device process launch --device "$udid" --terminate-existing --console \
        "$TVOS_BUNDLE_ID" > >(tee "$log") 2>&1 &
    pid=$!
    trap 'kill "$pid" 2>/dev/null || true' EXIT INT TERM
    sleep "$seconds"
    kill "$pid" 2>/dev/null || true
    wait "$pid" 2>/dev/null || true
    trap - EXIT INT TERM
    tvos_log "console detached after ${seconds}s (the app keeps running on the device) — log: $log"
elif [[ $console -eq 1 ]]; then
    xcrun devicectl device process launch --device "$udid" --terminate-existing --console "$TVOS_BUNDLE_ID"
else
    xcrun devicectl device process launch --device "$udid" --terminate-existing "$TVOS_BUNDLE_ID"
fi
