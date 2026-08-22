#!/usr/bin/env bash
# One-time/idempotent toolchain setup for the tvOS build.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"

command -v cmake  >/dev/null || tvos_fail "cmake not found (brew install cmake)"
command -v ninja  >/dev/null || tvos_fail "ninja not found (brew install ninja)"
command -v python3 >/dev/null || tvos_fail "python3 not found"
command -v rustup >/dev/null || tvos_fail "rustup not found (https://rustup.rs)"

cmake_ver="$(cmake --version | head -n1 | awk '{print $3}')"
python3 -c "import sys; v=tuple(int(x) for x in '$cmake_ver'.split('.')[:2]); sys.exit(0 if v>=(3,25) else 1)" \
    || tvos_fail "cmake >= 3.25 required, found $cmake_ver"
xcodebuild -showsdks 2>/dev/null | grep -q appletvos || tvos_fail "tvOS SDK not found in Xcode"

# Having the SDK is not enough: device-targeted xcodebuild also needs the matching tvOS *platform*
# component, or -destination resolution fails with "tvOS <ver> is not installed". Warn only —
# the download is several GB, so leave it to the user to run explicitly.
tvos_sdk_ver="$(xcodebuild -showsdks 2>/dev/null | awk '/appletvos/ {print $NF}' | sed 's/appletvos//' | head -n 1)"
if [[ -n "$tvos_sdk_ver" ]] && ! xcrun simctl runtime list 2>/dev/null | grep -q "tvOS $tvos_sdk_ver"; then
    tvos_log "WARNING: the tvOS $tvos_sdk_ver platform component is not installed — device builds"
    tvos_log "         (provision.sh) will fail with 'tvOS $tvos_sdk_ver is not installed'."
    tvos_log "         Run: xcodebuild -downloadPlatform tvOS"
fi

tvos_log "installing Rust nightly + aarch64-apple-tvos target (no-op if present)"
rustup toolchain install nightly --profile minimal >/dev/null
rustup target add --toolchain nightly aarch64-apple-tvos >/dev/null
rustup target list --toolchain nightly --installed | grep -q '^aarch64-apple-tvos$' || tvos_fail "tvOS Rust target missing"

tvos_log "cmake $cmake_ver, ninja $(ninja --version), $(rustc +nightly --version)"
tvos_log "tvOS SDK: $(xcodebuild -showsdks | awk '/appletvos/ {print $NF}')"
if udid="$(tvos_device_udid 2>/dev/null)"; then tvos_log "target device '$TVOS_DEVICE_NAME' = $udid"; else tvos_log "WARNING: no paired device named '$TVOS_DEVICE_NAME' (needed from Task 5 on)"; fi
tvos_log "setup OK"
