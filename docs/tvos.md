# Dusklight on Apple TV (tvOS) — personal fork notes

Status: 2026-08-23 — configures and builds via the `tvos-default` CMake preset; an unsigned
`Dusklight.app` is produced under `build/install/`. Provisioning, signing, install, and the first
on-device launch are all scripted (`provision.sh`, `sign.sh`, `install.sh`, `launch.sh`) but
**not yet run against the device**: the paired Apple TV (your paired Apple TV, confirmed via
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
   download; confirmed the device-side way `setup.sh` now checks — not the simulator-runtime
   listing this page just warned proves nothing — `AppleTVOS.platform/Developer/SDKs/AppleTVOS26.5.sdk`
   exists under Xcode).
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
**Confirmed on device** (2026-08-24): disc discovery printed
`.../Library/Caches/TwilitRealm/Dusklight/`, exactly as derived. `TVOS_DATA_SUBDIR` near the top of
the script is correct and needs no change.

## App icon and top shelf
`platforms/tvos/Info.plist.in` declares `CFBundleIcons ▸ CFBundlePrimaryIcon = "App Icon"` and
`TVTopShelfImage`, so the bundle needs a compiled asset catalog or tvOS has nothing to draw.
`platforms/tvos/Assets.xcassets` holds the final artwork (App Icon 400×240 / 800×480
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
Device: Apple TV 4K (3rd gen), tvOS 26.5, UDID `<YOUR_DEVICE_UDID>`. Sessions 2026-08-23
through 2026-08-25.

### Save persistence is proven end-to-end
The mirror can reconstitute a save from `NSUserDefaults` alone. Verified 2026-08-25 by the strict
form of the test, not the lenient one:

1. `devicectl device uninstall app` — destroys the whole data container.
2. Reinstall, then confirm `Library/Caches` **and** `Library/Preferences` both list `0 files`.
3. Copy back *only* `Library/Preferences/dev.twilitrealm.dusk.plist` (40 KB). Nothing into `Caches`.
4. Launch, boot to the file-select screen.

Result: all seven config entries restored at startup, then at card mount

```
[save-mirror] card ready: game GZ2P01, GCI folder format
[save-mirror] restore(save): restored EUR/Card A/01-GZ2P-gczelda2.gci (32832 bytes, a2ee8ee5b725)
              from mirror sequence 93 written at unix 1787666465
[save-mirror] restore(save): 1 restored, 0 already present
```

The restored file was **byte-identical** to a backup taken before the wipe (sha256
`a2ee8ee5b7253bb58abdbfee36a4cc17e382418a0377124eb0d084919afa8f61`), and the game listed the save
on its file-select screen. `0 already present` matters: this was a real restore, not the mirror
declining because a file happened to exist.

Two assumptions this retired. `cfprefsd` does **not** clobber a plist written underneath it into a
fresh container — it is read intact on first launch. And the restore does not lose a race with the
game's own card creation: `card ready` fires first, then `restore(save)`, then the snapshot.

### devicectl cannot delete files in a data container
There is no `remove`/`delete` subcommand — only `copy`, `info`, `install`, `notification`,
`orientation`, `process`, `reboot`, `sysdiagnose`, `uninstall`. `copy to` merges over the
destination and will not remove a file that is already there. So "purge `Library/Caches` and
relaunch" **cannot** be run on a physical device; the uninstall/reinstall/restore-only-the-mirror
sequence above is the workable substitute. On the simulator the container is on the host
filesystem, where `rm -rf` works.

### Install as an update; never delete first
`scripts/tvos/install.sh` runs a plain `devicectl device install app` with no
`--remove-existing-content`, which preserves the data container — verified across several
reinstalls. Passing `--remove-existing-content`, or uninstalling first, destroys `Library/Caches`
and `Library/Preferences` **together**, taking the save and the mirror that would rescue it. Always
`pull-saves.sh` first, and pull `Library/Preferences` too — `pull-saves.sh` covers only `Caches`.

### Known rough edges
- The bundle UUID changes on every install, so the disc path stored in `config.json` fails
  validation afterwards (`Saved DVD image path failed validation, clearing configured path`). Disc
  discovery self-heals this by rescanning and auto-selecting, so it is cosmetic.
- A flush that runs before `card ready` logs `its file is not on disk` for the save even when the
  game is running, because the game id is not resolved yet. The carry-forward rule preserves the
  mirrored bytes, so it is harmless — but it is a false negative in the file-presence check, and
  file presence is what the restore decision keys on. Worth tightening.
- SIGTERM is ignored by the app; SIGKILL is needed to terminate it from `devicectl`.

## Supplying a disc without a Mac

A build made with `--no-disc` carries no game data and can be handed to anyone: on first launch the
app shows a URL, and a disc image is uploaded to it from a browser on the same network. No Xcode, no
`devicectl`, no rebuild. `--disc` still works exactly as before and is still the right choice for a
personal build, because a bundled disc lives in the read-only app bundle and therefore survives a
tvOS purge, while an uploaded one sits in purgeable storage.

**Verified end to end on device, 2026-08-26** (Apple TV 4K 3rd gen, tvOS 26.5):

```
disc discovery: 0 candidate(s)      <- no disc anywhere, onboarding shown
disc discovery: 1 candidate(s)      <- after publish, without relaunching
Disc verification status: verified
Loading DVD image: .../discs/Legend of Zelda_ The - ... .rvz
```

The uploaded 902.7 MB `.rvz` was published into `<data dir>/discs/`, `.incoming/` was left empty,
the picker picked it up without a restart, and the game ran.

### How it fits together

`src/dusk/transfer/` holds the whole subsystem. `http_parse.cpp` and `upload_core.cpp` are pure and
covered by host tests in `tests/transfer/`; `server.cpp` owns the sockets. `src/dusk/ui/onboarding.cpp`
puts it on screen and is reached from `prelaunch.cpp` when discovery returns nothing — first run and
post-purge recovery are the same code path, because to the user they are the same situation.

Bytes stage in `<data dir>/discs/.incoming/<id>` and are renamed into `discs/` only after
`dusk::iso::validate` passes. That matters: `disc_discovery.cpp` matches on extension, so a partial
file sitting in `discs/` would be offered as playable. It is safe to keep `.incoming/` inside
`discs/` only because `collect()` uses a non-recursive `directory_iterator` and skips directories —
switching it to `recursive_directory_iterator` would silently start offering partial uploads.

The server runs only while the onboarding screen is up. The modal owns it, so dismissing the screen
stops it; it never listens while the game is running.

### Validation is two-tier, and the browser half is only a guard

The page reads the six-byte game id out of a **raw `.iso`** header and rejects a wrong game before
uploading anything. It cannot do this for `.rvz`, `.gcz`, `.wia` or `.ciso`: `borealis::disc` hashes
the *decoded* disc (`extern/borealis/src/disc.cpp:295` feeds `api.read(...)` into XXH3-128), which is
why one catalog hash covers every container format — and why a browser cannot reproduce it without
porting those decoders. For compressed containers the page checks only the container magic, so a
wrong `.rvz` still costs a full upload before being rejected on device with a specific reason.

The accepted ids come from `dusk::iso::accepted_game_ids_json()`, derived from the same
`AcceptedDiscs` table `validate` uses, so a disc the app accepts can never be one the page rejects.
Served on device as `["GZ2E01","GZ2J01","GZ2P01","RZDE01","RZDJ01","RZDP01"]` — six ids from seven
catalog rows, because `RZDE01` has two revisions and the page needs each id once.

### Things worth knowing

- **Resume is driven by the server, not the client.** The uploader advances by the offset the server
  acknowledges, never by what it sent, and treats `409` as "re-sync and continue". `judge_chunk`
  requires `offset == received`, which is what turns a retried chunk whose response was lost into a
  refusal rather than a silent double append.
- **The published filename is sanitised**, so `Legend of Zelda, The - Twilight Princess (Europe)
  (En,Fr,De,Es,It).rvz` lands as `Legend of Zelda_ The - Twilight Princess _Europe_ _En_Fr_De_Es_It_.rvz`.
  Commas and parentheses are outside `[A-Za-z0-9 ._-]`. Uglier, but the client-supplied name never
  reaches a filesystem call unsanitised.
- **`sign.sh` no longer refuses a build with no `disc/`.** Its completeness check used to treat that
  as fatal because "a bundled disc is the only one discovery finds"; that is no longer true. It
  reports a note instead. Do not reach for `--allow-incomplete` here — that flag exists to bypass
  genuine incompleteness and would mask real problems in every future shareable build.
- **Sizes:** a `--no-disc` bundle is 73M against 976M with the disc embedded.
- **`HashMismatch` is not a transfer failure.** A correct upload of a bad dump lands there, and the
  message says to re-dump rather than re-upload, because re-uploading cannot succeed.

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
