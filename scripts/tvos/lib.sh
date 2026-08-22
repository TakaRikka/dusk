#!/usr/bin/env bash
# Shared helpers for the tvOS scripts. Source this file; do not execute it.
set -euo pipefail

TVOS_TEAM_ID="${DUSK_TVOS_TEAM_ID:-<YOUR_TEAM_ID>}"
TVOS_BUNDLE_ID="${DUSK_TVOS_BUNDLE_ID:-dev.twilitrealm.dusk}"
TVOS_APP_NAME="Dusklight"
TVOS_DEVICE_NAME="${DUSK_TVOS_DEVICE_NAME:-<YOUR_DEVICE_NAME>}"
TVOS_FORK_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
TVOS_BUILD_DIR="$TVOS_FORK_ROOT/build/tvos-default"
TVOS_INSTALL_APP="$TVOS_FORK_ROOT/build/install/$TVOS_APP_NAME.app"
TVOS_LOG_DIR="$TVOS_FORK_ROOT/build/logs"
TVOS_PROFILE_DIR="$HOME/Library/Developer/Xcode/UserData/Provisioning Profiles"

tvos_log()  { printf '[tvos] %s\n' "$*"; }
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
    [[ $rc -eq 0 ]] || tvos_fail "no paired tvOS device named '$TVOS_DEVICE_NAME' (set DUSK_TVOS_DEVICE=<udid> or DUSK_TVOS_DEVICE_NAME)"
}

# Prints the path of a tvOS provisioning profile for TEAM.BUNDLE, newest first; empty if none.
tvos_profile_path() {
    local f plist
    for f in "$TVOS_PROFILE_DIR"/*.mobileprovision; do
        [[ -f "$f" ]] || continue
        plist="$(mktemp)"
        security cms -D -i "$f" >"$plist" 2>/dev/null || { rm -f "$plist"; continue; }
        python3 - "$f" "$plist" "$TVOS_TEAM_ID.$TVOS_BUNDLE_ID" <<'PY'
import plistlib, sys
p = plistlib.load(open(sys.argv[2], "rb"))
appid = p.get("Entitlements", {}).get("application-identifier", "")
if "tvOS" in p.get("Platform", []) and appid == sys.argv[3]:
    print(sys.argv[1])
PY
        rm -f "$plist"
    done | head -n 1
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
