#!/usr/bin/env bash
# Sign the installed tvOS app with the personal-team development profile.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"
usage() {
    cat <<'USAGE'
usage: scripts/tvos/sign.sh [--allow-incomplete] [--] [app bundle]
  app bundle           defaults to the built build/install/Dusklight.app
  --allow-incomplete   sign anyway when the completeness check below fails
  --                   end of options; the next argument is the app bundle path even
                       if it begins with -
USAGE
}

allow_incomplete=0
app=""
app_given=0
terminated=0
while [[ $# -gt 0 ]]; do
    if [[ $terminated -eq 1 ]]; then
        [[ $app_given -eq 0 ]] || { usage >&2; tvos_fail "unexpected extra argument: $1"; }
        app="$1"; app_given=1; shift; continue
    fi
    case "$1" in
        --) terminated=1; shift ;;
        --allow-incomplete) allow_incomplete=1; shift ;;
        -h|--help) usage; exit 0 ;;
        -*) usage >&2; tvos_fail "unknown argument: $1" ;;
        *) [[ $app_given -eq 0 ]] || { usage >&2; tvos_fail "unexpected extra argument: $1"; }
           app="$1"; app_given=1; shift ;;
    esac
done
if [[ $app_given -eq 1 ]]; then
    # An explicit "" is a mistake, not a request for the default -- e.g. a shell variable that
    # expanded empty. Silently falling back with ${app:-$TVOS_INSTALL_APP} would sign the wrong
    # bundle without telling anyone.
    [[ -n "$app" ]] || tvos_fail "app bundle path must not be empty"
else
    app="$TVOS_INSTALL_APP"
fi
[[ -d "$app" ]] || tvos_fail "app bundle not found: $app (run scripts/tvos/build.sh)"

# A stale or half-built bundle signs and installs perfectly happily, and only fails on the TV --
# an hour into a device session that cannot be repeated cheaply. Assert here, at the keyboard,
# that the bundle holds what this app cannot run without.
if [[ $allow_incomplete -eq 0 ]]; then
    missing=""
    # The disc image: tvOS has no file dialog, and disc discovery scans Dusklight.app/disc/ first.
    # A deliberate --no-disc build is the case --allow-incomplete exists for (push it with
    # scripts/tvos/push-disc.sh afterwards).
    if [[ ! -d "$app/disc" ]]; then
        missing="$missing"$'\n'"  - disc/ is missing — tvOS has no file dialog, so a bundled disc is the only one discovery finds without push-disc.sh"
    # A non-empty disc/ is not enough: a Finder visit alone drops a .DS_Store in it, and `ls -A`
    # counted that as content. Require an actual disc image -- same extension set as build.sh's
    # workspace scan and kDiscExtensions in src/dusk/disc_discovery_rules.hpp.
    elif ! find "$app/disc" -maxdepth 1 -type f \( -iname '*.iso' -o -iname '*.gcm' -o -iname '*.ciso' -o -iname '*.gcz' -o -iname '*.nfs' \
            -o -iname '*.rvz' -o -iname '*.wbfs' -o -iname '*.wia' -o -iname '*.tgc' \) -print -quit 2>/dev/null | grep -q .; then
        missing="$missing"$'\n'"  - disc/ has no disc image — tvOS has no file dialog, so a bundled disc is the only one discovery finds without push-disc.sh (a stray .DS_Store or similar doesn't count; need one of .iso/.gcm/.ciso/.gcz/.nfs/.rvz/.wbfs/.wia/.tgc)"
    fi
    # The compiled asset catalog: Info.plist's CFBundleIcons and TVTopShelfImage name catalog sets,
    # which only resolve inside one. tvOS draws no icon without it and a device can refuse install.
    [[ -f "$app/Assets.car" ]] \
        || missing="$missing"$'\n'"  - Assets.car is missing — Info.plist names asset-catalog sets that only resolve inside a compiled catalog"
    [[ -z "$missing" ]] || tvos_fail "$app is incomplete:$missing

Rebuild with: scripts/tvos/build.sh
(or re-run with --allow-incomplete to sign it as it stands)"
fi
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
