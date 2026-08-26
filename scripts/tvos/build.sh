#!/usr/bin/env bash
# Configure + build + install the tvOS app. Usage:
#   scripts/tvos/build.sh [--disc <path>] [--no-disc] [--no-mods] [--configure-only]
# Disc default: $DUSK_TVOS_DISC, else the single disc image found in the parent dir of the fork.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"

# NOTE: macOS ships bash 3.2 — no mapfile; avoid expanding possibly-empty arrays under set -u.
disc="${DUSK_TVOS_DISC:-}"; no_disc=0; mods_flag=""; configure_only=0
while [[ $# -gt 0 ]]; do
    case "$1" in
        --disc) disc="$2"; shift 2 ;;
        --no-disc) no_disc=1; shift ;;
        --no-mods) mods_flag="-DDUSK_ENABLE_CODE_MODS=OFF"; shift ;;
        --configure-only) configure_only=1; shift ;;
        *) tvos_fail "unknown argument: $1" ;;
    esac
done
# Default disc: the single image in the workspace dir (parent of the fork); if several, prefer .rvz (smallest install).
if [[ $no_disc -eq 0 && -z "$disc" ]]; then
    workspace="$(cd "$TVOS_FORK_ROOT/.." && pwd)"
    found=(); rvz=()
    while IFS= read -r f; do
        found+=("$f")
        case "$f" in *.rvz|*.RVZ) rvz+=("$f");; esac
    done < <(find "$workspace" -maxdepth 1 -type f \( -iname '*.iso' -o -iname '*.gcm' -o -iname '*.ciso' -o -iname '*.gcz' -o -iname '*.nfs' \
        -o -iname '*.rvz' -o -iname '*.wbfs' -o -iname '*.wia' -o -iname '*.tgc' \) | sort)
    if [[ ${#found[@]} -eq 1 ]]; then disc="${found[0]}"
    elif [[ ${#rvz[@]} -eq 1 ]]; then disc="${rvz[0]}"; tvos_log "several disc images in $workspace — preferring the .rvz"
    else tvos_fail "found ${#found[@]} disc images in $workspace (and ${#rvz[@]} .rvz); pass --disc <path> or --no-disc"; fi
fi
disc_arg="-DDUSK_BUNDLED_DISC="
if [[ $no_disc -eq 0 ]]; then [[ -f "$disc" ]] || tvos_fail "disc not found: $disc"; disc_arg="-DDUSK_BUNDLED_DISC=$disc"; tvos_log "bundling disc: $disc"; fi

cd "$TVOS_FORK_ROOT"
# The Rust `cc` crate stamps libnod's C objects with the SDK version unless this is set (linker warnings otherwise).
export TVOS_DEPLOYMENT_TARGET="${TVOS_DEPLOYMENT_TARGET:-14.5}"
# `set -e` aborts on a failing pipeline before PIPESTATUS can be read, and the build pipeline's
# trailing grep exits 1 whenever it matches nothing (i.e. on success) -- so each pipeline runs with
# -e off just long enough to capture the *first* stage's status.
# Compile the icon catalog. Assets.car is a build artifact, not a tracked file: CMake globs it at
# configure time, so it has to exist before the configure below. Assets.local.xcassets is an
# optional personal override (git-ignored) -- when present it wins, so a personal build can carry
# its own artwork without that artwork ever being committable.
icon_src="$TVOS_FORK_ROOT/platforms/tvos/Assets.xcassets"
if [[ -d "$TVOS_FORK_ROOT/platforms/tvos/Assets.local.xcassets" ]]; then
    icon_src="$TVOS_FORK_ROOT/platforms/tvos/Assets.local.xcassets"
    tvos_log "icons: using the local override (Assets.local.xcassets)"
else
    tvos_log "icons: using the committed catalog (Assets.xcassets)"
fi
icon_tmp="$(mktemp -d)"
xcrun actool --compile "$icon_tmp" --platform appletvos --target-device tv \
    --minimum-deployment-target 14.5 --app-icon "App Icon" \
    --output-partial-info-plist "$icon_tmp/partial.plist" \
    --enable-on-demand-resources NO "$icon_src" >"$TVOS_LOG_DIR/actool.log" 2>&1 \
    || tvos_fail "actool failed — see $TVOS_LOG_DIR/actool.log"
[[ -f "$icon_tmp/Assets.car" ]] || tvos_fail "actool produced no Assets.car — see $TVOS_LOG_DIR/actool.log"
cp "$icon_tmp/Assets.car" "$TVOS_FORK_ROOT/platforms/tvos/Assets.car"
rm -rf "$icon_tmp"

tvos_log "configure (preset tvos-default, bundle id $TVOS_BUNDLE_ID)"
set +e
cmake --preset tvos-default -DDUSK_BUNDLE_IDENTIFIER="$TVOS_BUNDLE_ID" "$disc_arg" ${mods_flag:+"$mods_flag"} 2>&1 | tee "$TVOS_LOG_DIR/configure.log" | tail -3
rc=${PIPESTATUS[0]}
set -e
[[ $rc -eq 0 ]] || tvos_fail "configure failed (cmake exit $rc) — see $TVOS_LOG_DIR/configure.log"
[[ $configure_only -eq 1 ]] && { tvos_log "configure only — done"; exit 0; }
tvos_log "build (all targets — the preset's default target list omits the in-tree mods that install needs)"
set +e
cmake --build --preset tvos-default --target all 2>&1 | tee "$TVOS_LOG_DIR/build.log" | grep -E 'error:|FAILED|ninja: build stopped'
rc=${PIPESTATUS[0]}
set -e
[[ $rc -eq 0 ]] || tvos_fail "build failed (cmake exit $rc) — see $TVOS_LOG_DIR/build.log"
if grep -qE 'FAILED|ninja: build stopped' "$TVOS_LOG_DIR/build.log"; then
    tvos_fail "build failed — see $TVOS_LOG_DIR/build.log"
fi
tvos_log "install -> $TVOS_INSTALL_APP"
rm -rf "$TVOS_INSTALL_APP"
set +e
cmake --install "$TVOS_BUILD_DIR" 2>&1 | tee "$TVOS_LOG_DIR/install.log" | tail -2
rc=${PIPESTATUS[0]}
set -e
[[ $rc -eq 0 ]] || tvos_fail "install failed (cmake exit $rc) — see $TVOS_LOG_DIR/install.log"
[[ -x "$TVOS_INSTALL_APP/$TVOS_APP_NAME" ]] || tvos_fail "install did not produce $TVOS_INSTALL_APP"
tvos_log "done: $(du -sh "$TVOS_INSTALL_APP" | cut -f1) at $TVOS_INSTALL_APP"
