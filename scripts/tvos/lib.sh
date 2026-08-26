#!/usr/bin/env bash
# Shared helpers for the tvOS scripts. Source this file; do not execute it.
set -euo pipefail

# Set these for your own Apple Developer team and Apple TV. They deliberately have no personal
# defaults: a hardcoded team id or device name would be wrong for everyone but its author, and
# would sign or install against the wrong target rather than failing.
TVOS_TEAM_ID="${DUSK_TVOS_TEAM_ID:-}"
TVOS_BUNDLE_ID="${DUSK_TVOS_BUNDLE_ID:-com.example.dusklight}"
TVOS_APP_NAME="Dusklight"
TVOS_DEVICE_NAME="${DUSK_TVOS_DEVICE_NAME:-}"
TVOS_FORK_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
TVOS_BUILD_DIR="$TVOS_FORK_ROOT/build/tvos-default"
TVOS_INSTALL_APP="$TVOS_FORK_ROOT/build/install/$TVOS_APP_NAME.app"
TVOS_LOG_DIR="$TVOS_FORK_ROOT/build/logs"
# Overridable so the profile lookup can be exercised against a mock directory without touching
# the real one. DUSK_TVOS_PROFILE_DIR takes precedence; the bare TVOS_PROFILE_DIR is kept for
# compatibility but, unlike every other knob here, was not namespaced -- an exported value would
# silently redirect Banjo's lookup too.
TVOS_PROFILE_DIR="${DUSK_TVOS_PROFILE_DIR:-${TVOS_PROFILE_DIR:-$HOME/Library/Developer/Xcode/UserData/Provisioning Profiles}}"

tvos_require_team_id() {
    [[ -n "$TVOS_TEAM_ID" ]] || tvos_fail "set DUSK_TVOS_TEAM_ID to your Apple Developer team id (Xcode > Settings > Accounts)"
}

tvos_log()  { printf '[tvos] %s\n' "$*"; }
# Like tvos_log but on stderr, for a function whose stdout a caller captures (tvos_profile_path).
tvos_note() { printf '[tvos] %s\n' "$*" >&2; }
tvos_fail() { printf '[tvos] ERROR: %s\n' "$*" >&2; exit 1; }

# Prints the UDID of the target Apple TV. Honors $DUSK_TVOS_DEVICE (UDID) first.
tvos_device_udid() {
    if [[ -n "${DUSK_TVOS_DEVICE:-}" ]]; then printf '%s\n' "$DUSK_TVOS_DEVICE"; return 0; fi
    local json; json="$(mktemp)"
    xcrun devicectl list devices --json-output "$json" >/dev/null 2>&1 || tvos_fail "devicectl failed; is Xcode installed?"
    local rc=0
    python3 - "$json" "$TVOS_DEVICE_NAME" <<'PY' || rc=$?
import json, sys
data = json.load(open(sys.argv[1]))
want = sys.argv[2]
for dev in data.get("result", {}).get("devices", []):
    hp = dev.get("hardwareProperties", {}); dp = dev.get("deviceProperties", {})
    if hp.get("platform") == "tvOS" and dp.get("name") == want:
        print(hp.get("udid", "")); sys.exit(0)
sys.exit(1)
PY
    rm -f "$json"
    [[ -n "$TVOS_DEVICE_NAME" || -n "${DUSK_TVOS_DEVICE:-}" ]] || tvos_fail "set DUSK_TVOS_DEVICE_NAME to your Apple TV's name, or DUSK_TVOS_DEVICE to its udid"
    [[ $rc -eq 0 ]] || tvos_fail "no paired tvOS device named '$TVOS_DEVICE_NAME' (set DUSK_TVOS_DEVICE=<udid> or DUSK_TVOS_DEVICE_NAME)"
}

# Fails unless the target Apple TV currently has a devicectl tunnel. Every device operation
# (install/launch/copy/provision) needs one, and a sleeping or off-network Apple TV reports
# tunnelState "unavailable" -- otherwise the underlying tool fails much later with a vaguer error.
# Optional arg: the device UDID (matched exactly); without it the device is matched by name.
tvos_require_device_reachable() {
    local want_udid="${1:-}" json state
    json="$(mktemp)"
    xcrun devicectl list devices --json-output "$json" >/dev/null 2>&1 \
        || { rm -f "$json"; tvos_fail "devicectl failed; is Xcode installed?"; }
    state="$(python3 - "$json" "$TVOS_DEVICE_NAME" "$want_udid" <<'PY'
import json, sys
data = json.load(open(sys.argv[1]))
want_name, want_udid = sys.argv[2], sys.argv[3]
for dev in data.get("result", {}).get("devices", []):
    hp = dev.get("hardwareProperties", {})
    dp = dev.get("deviceProperties", {})
    if want_udid:
        if hp.get("udid") != want_udid:
            continue
    elif hp.get("platform") != "tvOS" or dp.get("name") != want_name:
        continue
    print(dev.get("connectionProperties", {}).get("tunnelState", "unknown"))
    break
else:
    print("device not listed")
PY
)"
    rm -f "$json"
    case "$state" in
        unavailable|"device not listed")
            tvos_fail "Apple TV '$TVOS_DEVICE_NAME' is not reachable (tunnelState=$state) — wake it with the remote and make sure it is on the same network" ;;
    esac
}

# Prints the path of the newest tvOS provisioning profile usable for TEAM.BUNDLE; prints nothing
# if there is none. Two kinds are accepted, in this order of preference:
#   1. an exact `application-identifier` of TEAM.BUNDLE;
#   2. a team wildcard, `application-identifier` = TEAM.* -- which is what Xcode actually mints for
#      an entitlement-free tvOS app ("tvOS Team Provisioning Profile: *"). Signing with the
#      wildcard entitlements verbatim would give the app `application-identifier = TEAM.*`, which
#      the device rejects, so sign.sh narrows them to TEAM.BUNDLE before calling codesign.
# An exact match wins even when a wildcard profile is newer, so the whole directory is scanned
# before falling back. The note saying which kind was chosen goes to *stderr*: callers capture this
# function's stdout as the path.
# `ls -t` gives newest-first -- no `| head`, which would close the pipe under the still-running
# loop (BrokenPipe noise from security/python3).
tvos_profile_path() {
    local f plist kind wildcard=""
    while IFS= read -r f; do
        [[ -f "$f" ]] || continue
        plist="$(mktemp)"
        security cms -D -i "$f" >"$plist" 2>/dev/null || { rm -f "$plist"; continue; }
        kind=0
        python3 - "$plist" "$TVOS_TEAM_ID.$TVOS_BUNDLE_ID" "$TVOS_TEAM_ID.*" 2>/dev/null <<'PY' || kind=$?
import plistlib, sys
p = plistlib.load(open(sys.argv[1], "rb"))
if "tvOS" not in p.get("Platform", []):
    sys.exit(1)
appid = p.get("Entitlements", {}).get("application-identifier", "")
if appid == sys.argv[2]:
    sys.exit(0)   # exact TEAM.BUNDLE
if appid == sys.argv[3]:
    sys.exit(3)   # team wildcard TEAM.*
sys.exit(1)
PY
        rm -f "$plist"
        case $kind in
            0) tvos_note "profile: exact match for $TVOS_TEAM_ID.$TVOS_BUNDLE_ID — $f"
               printf '%s\n' "$f"; return 0 ;;
            3) [[ -n "$wildcard" ]] || wildcard="$f" ;;
        esac
    done < <(ls -t "$TVOS_PROFILE_DIR"/*.mobileprovision 2>/dev/null)
    if [[ -n "$wildcard" ]]; then
        tvos_note "profile: no exact match for $TVOS_TEAM_ID.$TVOS_BUNDLE_ID — falling back to the"
        tvos_note "         team wildcard $TVOS_TEAM_ID.* — $wildcard"
        tvos_note "         (sign.sh narrows its entitlements to $TVOS_TEAM_ID.$TVOS_BUNDLE_ID)"
        printf '%s\n' "$wildcard"
    fi
    return 0
}

# Prints the SHA-1 of the Apple Development identity for the team (OU match).
tvos_identity_sha() {
    security find-identity -v -p codesigning 2>/dev/null | awk '/Apple Development/ {print $2}' | while read -r sha; do
        if security find-certificate -a -Z -p -c "Apple Development" 2>/dev/null | awk -v want="$sha" '
            /SHA-1 hash:/ { cur=$3 } /BEGIN CERTIFICATE/ { grab=(cur==want) } grab { print } /END CERTIFICATE/ { grab=0 }' \
            | openssl x509 -noout -subject -nameopt sep_multiline 2>/dev/null | grep -q "OU=$TVOS_TEAM_ID"; then
            printf '%s\n' "$sha"; return 0
        fi
    done
}

mkdir -p "$TVOS_LOG_DIR"
