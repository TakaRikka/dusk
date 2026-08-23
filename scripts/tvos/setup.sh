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

# Having the SDK is not enough: device-targeted xcodebuild also needs the matching tvOS *device
# platform*, or -destination resolution fails with "tvOS <ver> is not installed".
# This used to read `xcrun simctl runtime list`, which enumerates *simulator* runtimes and says
# nothing whatsoever about the device side. Check the device platform itself instead: AppleTVOS
# .platform must carry a versioned SDK for the version xcodebuild advertises.
# What that proves: a tvOS device SDK of that version is on disk. What it cannot prove without the
# Apple TV: that `-destination platform=tvOS,id=<udid>` actually resolves for the paired device.
# Warning only, and nothing is downloaded from here — `xcodebuild -downloadPlatform tvOS` is
# several GB and stays the user's call.
tvos_sdk_ver="$(xcodebuild -showsdks 2>/dev/null | awk '/-sdk appletvos/ {print $NF}' | sed 's/.*appletvos//' | head -n 1)"
tvos_platform_dir="$(xcrun --sdk appletvos --show-sdk-platform-path 2>/dev/null || true)"
if [[ -z "$tvos_sdk_ver" || -z "$tvos_platform_dir" \
      || ! -d "$tvos_platform_dir/Developer/SDKs/AppleTVOS$tvos_sdk_ver.sdk" ]]; then
    tvos_log "WARNING: no tvOS ${tvos_sdk_ver:-<ver>} *device* SDK under AppleTVOS.platform — device"
    tvos_log "         builds (scripts/tvos/provision.sh) will fail with 'tvOS ${tvos_sdk_ver:-<ver>} is not installed'."
    tvos_log "         Run: xcodebuild -downloadPlatform tvOS"
else
    tvos_log "tvOS device platform: $tvos_platform_dir/Developer/SDKs/AppleTVOS$tvos_sdk_ver.sdk"
fi

tvos_log "installing Rust nightly + aarch64-apple-tvos target (no-op if present)"
rustup toolchain install nightly --profile minimal >/dev/null
rustup target add --toolchain nightly aarch64-apple-tvos >/dev/null
rustup target list --toolchain nightly --installed | grep -q '^aarch64-apple-tvos$' || tvos_fail "tvOS Rust target missing"

tvos_log "cmake $cmake_ver, ninja $(ninja --version), $(rustc +nightly --version)"
tvos_log "tvOS SDK: $(xcodebuild -showsdks | awk '/appletvos/ {print $NF}')"
if udid="$(tvos_device_udid 2>/dev/null)"; then tvos_log "target device '$TVOS_DEVICE_NAME' = $udid"; else tvos_log "WARNING: no paired device named '$TVOS_DEVICE_NAME' (needed from Task 5 on)"; fi
tvos_log "setup OK"
