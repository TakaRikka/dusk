#!/usr/bin/env bash
# Launch the app on the Apple TV. Usage:
#   scripts/tvos/launch.sh                       launch and return immediately
#   scripts/tvos/launch.sh --console             launch and stream stdout/stderr until Ctrl-C
#   scripts/tvos/launch.sh --console-seconds N   stream for N seconds, then detach
# --console-seconds exists because this Mac has neither timeout(1) nor gtimeout(1), so
# `timeout 60 scripts/tvos/launch.sh --console` is not available. It is for capturing a bounded
# boot log, NOT for starting a play session: devicectl's console is attached to the app, and the
# app may be terminated when that console goes away. Detaching kills the *local* devicectl process
# with SIGKILL -- SIGTERM was worse, devicectl forwards it to the app and that always killed it --
# but whether the app survives losing its console has not been checked on the device. Tell which
# happened by looking at the TV: still on screen means it survived; back at the home screen means
# it did not. Either way, the gameplay run is plain `scripts/tvos/launch.sh`, which attaches no
# console at all.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"

usage() {
    cat <<'USAGE'
usage: scripts/tvos/launch.sh [--console | --console-seconds N]
  (no flag)            launch and return immediately
  --console            launch and stream stdout/stderr until Ctrl-C
  --console-seconds N  stream for N seconds, then kill the local console. For a bounded boot
                       log: the app on the device may be terminated along with the console, so
                       use the no-flag form for the gameplay run.
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
    # SIGKILL, not SIGTERM: devicectl forwards SIGTERM on to the app on the device, so the old
    # `kill "$pid"` terminated the very app this was supposed to leave running. SIGKILL cannot be
    # forwarded, so only the local process dies -- though the app may still go down with its
    # console. The trap keeps Ctrl-C and SIGTERM from leaving devicectl behind. Tradeoff: SIGKILL
    # gives devicectl no chance to flush, so the last buffered console lines can be missing from
    # $log -- use plain --console for a complete capture.
    trap 'kill -KILL "$pid" 2>/dev/null || true' EXIT INT TERM
    sleep "$seconds"
    kill -KILL "$pid" 2>/dev/null || true
    wait "$pid" 2>/dev/null || true
    trap - EXIT INT TERM
    tvos_log "console detached after ${seconds}s — log: $log"
    tvos_log "the app may have been terminated with the console — look at the TV: still on screen"
    tvos_log "means it survived, back at the home screen means it did not. For the gameplay run use"
    tvos_log "plain scripts/tvos/launch.sh, which attaches no console."
elif [[ $console -eq 1 ]]; then
    xcrun devicectl device process launch --device "$udid" --terminate-existing --console "$TVOS_BUNDLE_ID"
else
    xcrun devicectl device process launch --device "$udid" --terminate-existing "$TVOS_BUNDLE_ID"
fi
