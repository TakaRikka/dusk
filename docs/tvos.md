# Dusklight on Apple TV (tvOS) — personal fork notes

Status: 2026-08-22 — configures and builds via the `tvos-default` CMake preset; an unsigned
`Dusklight.app` is produced under `build/install/`. Signing, install, and the first on-device
launch are scripted (`sign.sh`, `install.sh`, `launch.sh`) but **not yet run against the device**:
the paired Apple TV ("<YOUR_DEVICE_NAME>", confirmed via `xcrun devicectl list devices` as an
Apple TV 4K (3rd generation) / `AppleTV14,1`) is currently asleep/unavailable, and no tvOS
provisioning profile exists yet (Task 5, `scripts/tvos/provision.sh`, not written yet). Not
upstreamed.

## One-time setup
1. `scripts/tvos/setup.sh` — CMake/Ninja/Python/Xcode tvOS SDK checks, Rust nightly + `aarch64-apple-tvos`.
2. Pair the Apple TV with Xcode (Settings ▸ Remotes and Devices ▸ Remote App and Devices), keep it awake.
3. `scripts/tvos/provision.sh` — creates the tvOS development profile for `dev.twilitrealm.dusk` (team <YOUR_TEAM_ID>) and registers the device. Requires the Apple ID of that team in Xcode ▸ Settings ▸ Accounts.

## Build, sign, install, launch
- `scripts/tvos/build.sh [--disc <path>] [--no-disc] [--no-mods] [--configure-only]` — configures
  with CMake preset `tvos-default`, builds with `--target all` (the preset's default target list
  omits the in-tree mods that the install step needs), then `cmake --install`s into
  `build/install/Dusklight.app`. Exports `TVOS_DEPLOYMENT_TARGET=14.5` for the build — without it,
  the Rust `cc` crate stamps libnod's C objects with the wrong SDK version and the link step warns.
  - Disc selection when neither `--disc` nor `--no-disc` is given: the script looks for disc images
    (`.iso .rvz .gcm .wbfs .wia .ciso .gcz`) in the directory that contains the fork. Exactly one
    match is bundled as-is; with several candidates and exactly one `.rvz` among them, the `.rvz` is
    preferred (smallest install) even if e.g. an `.iso` sits alongside it; any other combination is a
    hard error asking for an explicit `--disc <path>` or `--no-disc`.
  - `--no-mods` passes `-DDUSK_ENABLE_CODE_MODS=OFF`, for when the in-tree mods fail to build.
- `scripts/tvos/sign.sh [app-path]` — embeds the Task 5 provisioning profile as
  `embedded.mobileprovision`, derives entitlements from it, and code-signs nested code (mods under
  `Frameworks/*.so`, `*.dylib`, `*.framework`) before signing the app bundle itself.
- `scripts/tvos/install.sh [app]` — `xcrun devicectl device install app` to the paired device; fails
  fast if the app has no `embedded.mobileprovision` (i.e. `sign.sh` hasn't run yet).
- `scripts/tvos/launch.sh [--console]` — `xcrun devicectl device process launch --terminate-existing`
  for `dev.twilitrealm.dusk`; `--console` attaches and streams stdout/stderr until Ctrl-C.

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

`push-disc.sh`'s destination (`Library/Caches/Twilit Realm/Dusklight/discs/<file>`) assumes the
SDL pref path uses org name `"Twilit Realm"` and app name `"Dusklight"` — **this is an assumption,
not yet confirmed on-device**. It is isolated to one variable, `TVOS_DATA_SUBDIR`, near the top of
the script; once the real data path is known (it is printed in the `launch.sh --console` output),
fixing that one line is enough.

## Observed on device
Observed on device: pending — to be filled after the first device session (not yet run as of 2026-08-22).

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
