#!/usr/bin/env bash
# One-time: create the tvOS development provisioning profile for TEAM.BUNDLE and register the Apple TV.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"
udid="$(tvos_device_udid)"
seed_src="$TVOS_FORK_ROOT/scripts/tvos/provision-seed"
seed_build="$TVOS_FORK_ROOT/build/provision-seed"
tvos_log "seeding profile for $TVOS_TEAM_ID.$TVOS_BUNDLE_ID on device $udid"
cmake -S "$seed_src" -B "$seed_build" -G Xcode -DCMAKE_SYSTEM_NAME=tvOS -DCMAKE_OSX_SYSROOT=appletvos \
      -DCMAKE_OSX_DEPLOYMENT_TARGET=14.5 -DCMAKE_OSX_ARCHITECTURES=arm64 \
      -DSEED_BUNDLE_ID="$TVOS_BUNDLE_ID" -DSEED_TEAM_ID="$TVOS_TEAM_ID" >"$TVOS_LOG_DIR/provision-configure.log" 2>&1 \
      || tvos_fail "seed configure failed — see $TVOS_LOG_DIR/provision-configure.log"
xcodebuild -project "$seed_build/ProvisionSeed.xcodeproj" -target ProvisionSeed -configuration Release \
      -destination "platform=tvOS,id=$udid" -allowProvisioningUpdates -allowProvisioningDeviceRegistration build \
      2>&1 | tee "$TVOS_LOG_DIR/provision-build.log" | grep -E 'error|warning: .*provision|Signing Identity|Provisioning Profile|BUILD (SUCCEEDED|FAILED)' || true
grep -q 'BUILD SUCCEEDED' "$TVOS_LOG_DIR/provision-build.log" || tvos_fail "xcodebuild did not succeed. If the log mentions 'No Account' or 'not signed in', sign in with the Apple ID of team $TVOS_TEAM_ID in Xcode > Settings > Accounts and rerun."
profile="$(tvos_profile_path)"
[[ -n "$profile" ]] || tvos_fail "no tvOS profile for $TVOS_TEAM_ID.$TVOS_BUNDLE_ID found in $TVOS_PROFILE_DIR after provisioning"
tvos_log "profile: $profile"
security cms -D -i "$profile" | plutil -p - | grep -E '"Name"|"ExpirationDate"|"TeamName"' | sed 's/^/[tvos]   /'
