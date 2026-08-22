#!/usr/bin/env bash
# Sign the installed tvOS app with the personal-team development profile.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"
app="${1:-$TVOS_INSTALL_APP}"
[[ -d "$app" ]] || tvos_fail "app bundle not found: $app (run scripts/tvos/build.sh)"
profile="$(tvos_profile_path)"; [[ -n "$profile" ]] || tvos_fail "no tvOS profile for $TVOS_TEAM_ID.$TVOS_BUNDLE_ID — run scripts/tvos/provision.sh"
identity="$(tvos_identity_sha || true)"; [[ -n "$identity" ]] || tvos_fail "no 'Apple Development' identity for team $TVOS_TEAM_ID in the keychain"

ent="$(mktemp -t dusk-ent).plist"
security cms -D -i "$profile" | python3 -c '
import plistlib, sys
p = plistlib.load(sys.stdin.buffer)
plistlib.dump(p["Entitlements"], sys.stdout.buffer)' > "$ent"
cp "$profile" "$app/embedded.mobileprovision"
tvos_log "entitlements: $(plutil -p "$ent" | grep application-identifier)"

# Sign nested code first (mods as Frameworks/*.so, any dylib/framework), then the bundle.
while IFS= read -r -d '' nested; do
    codesign --force --sign "$identity" --timestamp=none "$nested" >/dev/null
done < <(find "$app/Frameworks" \( -name '*.so' -o -name '*.dylib' -o -name '*.framework' \) -print0 2>/dev/null)
codesign --force --sign "$identity" --entitlements "$ent" --timestamp=none "$app"
codesign --verify --deep --strict --verbose=2 "$app" 2>&1 | tail -2
codesign -d --entitlements - "$app" 2>/dev/null | grep -E 'application-identifier|team-identifier|get-task-allow' -A1 | sed 's/^/[tvos]   /'
rm -f "$ent"
tvos_log "signed $app with $identity"
