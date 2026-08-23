# Dusklight on Apple TV (tvOS) — personal fork notes

Status: 2026-08-23 — configures and builds via the `tvos-default` CMake preset; an unsigned
`Dusklight.app` is produced under `build/install/`. Provisioning, signing, install, and the first
on-device launch are all scripted (`provision.sh`, `sign.sh`, `install.sh`, `launch.sh`) but
**not yet run against the device**: the paired Apple TV ("<YOUR_DEVICE_NAME>", confirmed via
`xcrun devicectl list devices` as an Apple TV 4K (3rd generation) / `AppleTV14,1`) is asleep —
`connectionProperties.tunnelState` reads `unavailable`. No tvOS provisioning profile exists *for
this app's team* (`<YOUR_TEAM_ID>`) yet — `provision.sh` mints one during the device session. This Mac
does already hold one unrelated tvOS profile: a wildcard (`application-identifier = <REDACTED_TEAM_ID>.*`)
for a different team, "<REDACTED>" (`<REDACTED_TEAM_ID>`), created 2026-03-09. `tvos_profile_path`
correctly ignores it, since it matches neither `<YOUR_TEAM_ID>.dev.twilitrealm.dusk` nor `<YOUR_TEAM_ID>.*`.
To sign with that other team instead, override `DUSK_TVOS_TEAM_ID`. Not upstreamed.

## One-time setup
1. `scripts/tvos/setup.sh` — CMake/Ninja/Python/Xcode tvOS SDK checks, Rust nightly +
   `aarch64-apple-tvos`. It also warns (without downloading anything) when the tvOS *device*
   platform matching the SDK version is missing — it looks for the versioned SDK under
   `AppleTVOS.platform/Developer/SDKs/`, since a *simulator*-runtime listing
   (`xcrun simctl runtime list`) proves nothing about the device side. What that check still cannot
   prove without the Apple TV is that `-destination platform=tvOS,id=<udid>` resolves for the
   paired device.
2. `xcodebuild -downloadPlatform tvOS` — **required for `provision.sh`**. Having the tvOS SDK is
   not enough: device-targeted `xcodebuild` resolves `-destination` against the installed platform
   and otherwise refuses with *"tvOS 26.5 is not installed. Please download and install the
   platform from Xcode ▸ Settings ▸ Components"*. Run on this Mac on **2026-08-23** (3.76 GB
   download; `xcrun simctl runtime list` now shows `tvOS 26.5 (23L470)`).
3. Pair the Apple TV with Xcode (Settings ▸ Remotes and Devices ▸ Remote App and Devices), keep it awake.
4. `scripts/tvos/provision.sh` — creates the tvOS development profile for `dev.twilitrealm.dusk` (team
   <YOUR_TEAM_ID>) and registers the device. Requires the Apple ID of that team in Xcode ▸ Settings ▸
   Accounts. The seed project is generated with `-DCMAKE_XCODE_GENERATE_SCHEME=ON` and built with
   `-scheme` rather than `-target`: xcodebuild silently ignores `-destination` when only a target is
   given ("Ignoring provided run destination because no scheme was passed"), which would leave
   `-allowProvisioningDeviceRegistration` with no device to register.

Every device-facing script (`provision.sh`, `install.sh`, `launch.sh`, `pull-saves.sh`,
`push-saves.sh`, `push-disc.sh`) first calls `tvos_require_device_reachable` and stops with
`Apple TV '<name>' is not reachable (tunnelState=<value>) — wake it with the remote and make sure
it is on the same network` when the Apple TV has no devicectl tunnel.

## Build, sign, install, launch
- `scripts/tvos/build.sh [--disc <path>] [--no-disc] [--no-mods] [--configure-only]` — configures
  with CMake preset `tvos-default`, builds with `--target all` (the preset's default target list
  omits the in-tree mods that the install step needs), then `cmake --install`s into
  `build/install/Dusklight.app`. Exports `TVOS_DEPLOYMENT_TARGET=14.5` for the build — without it,
  the Rust `cc` crate stamps libnod's C objects with the wrong SDK version and the link step warns.
  - Disc selection when neither `--disc` nor `--no-disc` is given: the script looks for disc images
    (`.iso .gcm .ciso .gcz .nfs .rvz .wbfs .wia .tgc` — the same set as `kDiscExtensions` in
    `src/dusk/disc_discovery_rules.hpp`) in the directory that contains the fork. Exactly one
    match is bundled as-is; with several candidates and exactly one `.rvz` among them, the `.rvz` is
    preferred (smallest install) even if e.g. an `.iso` sits alongside it; any other combination is a
    hard error asking for an explicit `--disc <path>` or `--no-disc`.
  - `--no-mods` passes `-DDUSK_ENABLE_CODE_MODS=OFF`, for when the in-tree mods fail to build.
- `scripts/tvos/sign.sh [app-path]` — embeds the profile created by `provision.sh` as
  `embedded.mobileprovision`, derives entitlements from it, and code-signs nested code (mods under
  `Frameworks/*.so`, `*.dylib`, `*.framework`) before signing the app bundle itself.
  - **Which profile.** `lib.sh`'s `tvos_profile_path` prefers a tvOS profile whose
    `application-identifier` is exactly `<YOUR_TEAM_ID>.dev.twilitrealm.dusk`; failing that it falls back
    to a team wildcard, `<YOUR_TEAM_ID>.*` — which is what Xcode actually mints for an entitlement-free
    tvOS app ("tvOS Team Provisioning Profile: *"). An exact match wins even when a wildcard profile
    is newer. The function prints which kind it chose on **stderr**, because callers capture its
    stdout as the path. The directory searched is
    `~/Library/Developer/Xcode/UserData/Provisioning Profiles` unless `TVOS_PROFILE_DIR` overrides it.
  - **Wildcard entitlements are narrowed before signing.** `application-identifier`, and any
    `keychain-access-groups` entry ending in `.*`, are rewritten to `<YOUR_TEAM_ID>.dev.twilitrealm.dusk`;
    signing with `<YOUR_TEAM_ID>.*` verbatim produces an app the Apple TV rejects. Everything else in
    the entitlements is passed through untouched, and every rewrite is logged.
  - **It refuses an incomplete bundle.** Before signing anything it asserts that the bundle holds a
    non-empty `disc/` directory **and** `Assets.car`, and otherwise stops with the list of what is
    missing and the rebuild command (`scripts/tvos/build.sh`). A stale or half-built bundle
    otherwise signs and installs happily and only fails on the TV. `--allow-incomplete` skips the
    check — that is the flag for a deliberate `--no-disc` build you intend to feed with
    `push-disc.sh`.
- `scripts/tvos/install.sh [app]` — `xcrun devicectl device install app` to the paired device.
  It checks device reachability *before* the local `embedded.mobileprovision` check: a sleeping
  Apple TV needs a walk to the living room, while "is not signed" is fixed by rerunning `sign.sh`
  at the keyboard, so the slow fix surfaces first and both can proceed in parallel.
- `scripts/tvos/launch.sh [--console | --console-seconds N]` —
  `xcrun devicectl device process launch --terminate-existing` for `dev.twilitrealm.dusk`.
  `--console` attaches and streams stdout/stderr until Ctrl-C. `--console-seconds N` streams for N
  seconds and then kills the local console, recording the stream to
  `build/logs/launch-console.log`. It exists because this Mac has neither `timeout(1)` nor
  `gtimeout(1)`, so `timeout 60 scripts/tvos/launch.sh --console` is not available.
  - **`--console-seconds` is for capturing a bounded boot log, not for starting a play session.**
    devicectl's console is attached to the app, and **the app may be terminated when the console
    detaches.** An earlier version of this page claimed the app keeps running; that was wrong. The
    script sent `SIGTERM` to the local `devicectl` process, and devicectl forwards `SIGTERM` on to
    the app, so the app died every time. It now sends `SIGKILL`, which cannot be forwarded — but
    whether the app survives *losing* its console has not been tested on the device.
  - **How to tell which happened:** look at the TV. Still on screen, the app survived the detach;
    back at the tvOS home screen, it did not.
  - **Use plain `scripts/tvos/launch.sh` for the gameplay run.** It attaches no console, so there
    is nothing to detach later.
  - The detach sends `SIGKILL` to the local `devicectl`, which gives it no chance to flush, so the
    last buffered lines of `build/logs/launch-console.log` can be missing. For a complete capture
    use plain `--console`.

## How the disc is found
No file dialog on tvOS. `src/dusk/disc_discovery*` scans `Dusklight.app/disc/` then
`<data dir>/discs/` and `<data dir>/`; one candidate → verified automatically on the pre-launch
screen; several → chooser; none → explanation. `scripts/tvos/push-disc.sh <disc image>` copies a
disc into `<data dir>/discs/` on the device, for apps built with `--no-disc`.

## Storage caveat
tvOS keeps app data in purgeable `Library/Caches` (SDL pref path). Back up the whole data
container with `scripts/tvos/pull-saves.sh [dest dir]` (default destination:
`../backups/dusklight-tvos-<timestamp>/Caches`, next to the fork); restore with
`scripts/tvos/push-saves.sh <backup dir>`. Both treat `Library/Caches` as one tree — config,
save/card files, and any cached disc data all move together.

`push-disc.sh`'s destination is `Library/Caches/TwilitRealm/Dusklight/discs/<file>`, derived (not
guessed) from the source: `dusk::AppInfo` sets `orgName = "TwilitRealm"` / `appName = "Dusklight"`
(`src/dusk/app_info.hpp`), borealis passes both straight to `SDL_GetPrefPath`
(`extern/borealis/src/data.cpp`), and SDL's tvOS implementation formats
`"<NSCachesDirectory>/<org>/<app>/"`. On tvOS the data path is the preference path: borealis' only
other default (`useDocumentsOnIOS`) is gated on `isIOS`, which is `TARGET_OS_IOS && !TARGET_OS_TV`.
Still **to be confirmed on-device** — the real path is printed by disc discovery in the
`launch.sh --console` output. It is isolated to one variable, `TVOS_DATA_SUBDIR`, near the top of
the script, so fixing that one line is enough if it differs.

## App icon and top shelf
`platforms/tvos/Info.plist.in` declares `CFBundleIcons ▸ CFBundlePrimaryIcon = "App Icon"` and
`TVTopShelfImage`, so the bundle needs a compiled asset catalog or tvOS has nothing to draw.
`platforms/tvos/Assets.xcassets` holds **placeholder** solid-colour art (App Icon 400×240 / 800×480
as a two-layer parallax stack — actool rejects a single layer with *"The image stack 'App Icon'
must have at least 2 layers"* — plus Top Shelf Image 1920×720 and Top Shelf Image Wide 2320×720,
each with a @2x variant). `platforms/tvos/Assets.car` is the compiled result, picked up by the
`file(GLOB_RECURSE DUSK_RESOURCE_FILES ...)` in `CMakeLists.txt`. Regenerate after editing the
catalog:

```sh
xcrun actool platforms/tvos/Assets.xcassets --compile platforms/tvos \
    --platform appletvos --minimum-deployment-target 14.5 --target-device tv \
    --app-icon "App Icon" --output-partial-info-plist /tmp/tvos-assets-partial.plist
```

The partial plist is only a cross-check that the generated keys match `Info.plist.in`; it is not
consumed by the build.

## Observed on device
Observed on device: pending — to be filled after the first device session (not yet run as of
2026-08-23; the Apple TV has been asleep throughout, and nothing has been installed or launched
on it).

## Fixes needed on top of upstream 41d5148
Two configure-time fixes were needed to get the tvOS cross-compile working at all, both already
committed on this branch on top of upstream commit `41d5148` ("UiService: Dialog controls (#2332)"):

- `8b3809a` — **Dawn: disable protobuf when cross-compiling.** There is no prebuilt Dawn package
  for tvOS, so Aurora's provider builds Dawn from source. Dawn's `third_party/protobuf.cmake`
  raises a `FATAL_ERROR` when `CMAKE_CROSSCOMPILING` is set and no host `protoc` is supplied.
  protobuf there is only pulled in for `TINT_BUILD_IR_BINARY`, which isn't used, so it's turned off.
- `005624e` — **sqlite3: build the amalgamation.** CMake's `FindSQLite3` module calls
  `pkg_check_modules()` without checking whether the preceding `find_package(PkgConfig QUIET)`
  succeeded. The tvOS preset sets `CMAKE_DISABLE_FIND_PACKAGE_PkgConfig`, so that module never
  loads in this scope and `PKG_CONFIG_VERSION` stays undefined — but FreeType's
  `include(FindPkgConfig)` has already defined `pkg_check_modules` globally, so the call is reached
  anyway and its cache-check `if()` fails to parse (`CMake Error at Modules/FindPkgConfig.cmake:870
  (if): Unknown arguments specified`). Disabling `find_package(SQLite3)` makes Aurora build the
  sqlite3 amalgamation instead, matching what android-base already does for zstd.

No further upstream-compatibility fixes are known to be needed; more may surface once the first
real device run (Task 10) happens.
