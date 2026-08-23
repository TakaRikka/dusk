#!/usr/bin/env bash
# Sign the installed tvOS app with the personal-team development profile.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"
app="${1:-$TVOS_INSTALL_APP}"
[[ -d "$app" ]] || tvos_fail "app bundle not found: $app (run scripts/tvos/build.sh)"
profile="$(tvos_profile_path)"; [[ -n "$profile" ]] || tvos_fail "no tvOS profile for $TVOS_TEAM_ID.$TVOS_BUNDLE_ID — run scripts/tvos/provision.sh"
identity="$(tvos_identity_sha || true)"; [[ -n "$identity" ]] || tvos_fail "no 'Apple Development' identity for team $TVOS_TEAM_ID in the keychain"

# One path per temp file, created once and removed once: the old `ent="$(mktemp -t dusk-ent).plist"`
# made a temp file and then wrote to a *different* name, leaking the first and leaving the second
# behind on any failure. The trap covers every exit, including tvos_fail's.
ent="$(mktemp -t dusk-ent)"
prof="$(mktemp -t dusk-prof)"
trap 'rm -f "$ent" "$prof"' EXIT
# Decode to a file rather than piping into python: plistlib.load() seeks its stream, and a pipe
# cannot ("io.UnsupportedOperation: File or stream is not seekable"). lib.sh decodes the same way.
security cms -D -i "$profile" >"$prof" 2>/dev/null \
    || tvos_fail "could not decode $profile (security cms -D failed)"
# A team wildcard profile carries `application-identifier = TEAM.*` (and often the same wildcard in
# keychain-access-groups). Signing with those verbatim gives the installed app an identifier the
# Apple TV rejects, so narrow the wildcards -- and only the wildcards -- to the concrete bundle id.
# Every other entitlement is passed through untouched.
rewrites="$(python3 - "$prof" "$ent" "$TVOS_TEAM_ID.$TVOS_BUNDLE_ID" <<'PY'
import plistlib, sys

src, dst, appid = sys.argv[1], sys.argv[2], sys.argv[3]
ent = plistlib.load(open(src, "rb")).get("Entitlements")
if not isinstance(ent, dict):
    sys.stderr.write("the profile carries no Entitlements dictionary\n")
    sys.exit(1)

cur = ent.get("application-identifier")
if isinstance(cur, str) and cur.endswith(".*"):
    ent["application-identifier"] = appid
    print("application-identifier: %s -> %s" % (cur, appid))

groups = ent.get("keychain-access-groups")
if isinstance(groups, list):
    for i, g in enumerate(groups):
        if isinstance(g, str) and g.endswith(".*"):
            groups[i] = appid
            print("keychain-access-groups[%d]: %s -> %s" % (i, g, appid))

with open(dst, "wb") as f:
    plistlib.dump(ent, f)
PY
)" || tvos_fail "could not extract entitlements from $profile"
if [[ -n "$rewrites" ]]; then
    tvos_log "wildcard profile — narrowing entitlements to $TVOS_TEAM_ID.$TVOS_BUNDLE_ID:"
    printf '%s\n' "$rewrites" | sed 's/^/[tvos]   /'
fi
cp "$profile" "$app/embedded.mobileprovision"
tvos_log "entitlements: $(plutil -p "$ent" | grep application-identifier)"

# Sign nested code first (mods as Frameworks/*.so, any dylib/framework), then the bundle.
while IFS= read -r -d '' nested; do
    codesign --force --sign "$identity" --timestamp=none "$nested" >/dev/null
done < <(find "$app/Frameworks" \( -name '*.so' -o -name '*.dylib' -o -name '*.framework' \) -print0 2>/dev/null)
codesign --force --sign "$identity" --entitlements "$ent" --timestamp=none "$app"
codesign --verify --deep --strict --verbose=2 "$app" 2>&1 | tail -2
codesign -d --entitlements - "$app" 2>/dev/null | grep -E 'application-identifier|team-identifier|get-task-allow' -A1 | sed 's/^/[tvos]   /'
tvos_log "signed $app with $identity"
