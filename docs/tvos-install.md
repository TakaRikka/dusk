# Installing Dusklight on Apple TV (tvOS)

tvOS has no file manager and no document picker, so getting a build and a disc image onto an Apple
TV works differently to the other platforms. There are two ways to do it:

- **Bundle the disc at build time.** Simplest, and the disc lives in the read-only app bundle so it
  survives tvOS reclaiming storage. The resulting build contains your game data, so it is personal
  to you.
- **Build without a disc and upload one over your network.** The build can be shared, and the disc
  is sent from a browser on a phone or laptop. Nothing but the Apple TV and a browser is needed.

Both use the same signing and install steps.

## Prerequisites

- macOS with Xcode 16.4 or later, and the tvOS platform installed
  (`xcodebuild -downloadPlatform tvOS`). See [building.md](building.md).
- An Apple Developer account. A free account works; builds expire after 7 days.
- Your Apple TV paired with Xcode: **Settings ▸ Remotes and Devices ▸ Remote App and Devices** on
  the Apple TV, then **Window ▸ Devices and Simulators** in Xcode.
- The Apple TV awake and on the same network as your Mac. Every device-facing script stops with
  `is not reachable (tunnelState=…)` when it is asleep.

Set your own signing details once, in `scripts/tvos/config.local.sh` (git-ignored):

```sh
DUSK_TVOS_TEAM_ID=YOURTEAMID
DUSK_TVOS_DEVICE_NAME="Your Apple TV"
```

The bundle identifier defaults to `dev.twilitrealm.dusk` and only needs overriding
(`DUSK_TVOS_BUNDLE_ID`) if you want to install alongside an existing copy.

## 1. Create a provisioning profile

```sh
scripts/tvos/provision.sh
```

This mints a tvOS development profile for your team and registers the Apple TV. It needs the Apple
ID for that team signed in under **Xcode ▸ Settings ▸ Accounts**.

## 2. Build

```sh
scripts/tvos/build.sh --disc /path/to/disc.rvz   # bundle a disc
scripts/tvos/build.sh --no-disc                  # shareable build, upload a disc later
```

With neither flag the script looks for a single disc image beside the repository and bundles it.
See [building.md](building.md) for the full flag list.

## 3. Sign

```sh
scripts/tvos/sign.sh
```

Signing uses a profile matching `<team>.dev.twilitrealm.dusk`, or the team wildcard `<team>.*` that
Xcode mints for an entitlement-free app. Wildcard entitlements are narrowed to the concrete bundle
id before signing, because an app signed with `<team>.*` verbatim is rejected by the device.

## 4. Install and launch

```sh
scripts/tvos/install.sh
scripts/tvos/launch.sh
```

Installing over an existing copy preserves the app's data. **Do not delete the app to reinstall it**
— that destroys its data container, taking saves and settings with it.

To capture a boot log, use `scripts/tvos/launch.sh --console-seconds 90`. Use the plain form for
actually playing: the console is attached to the app, and detaching it can terminate the app.

## 5. Supplying a disc to a `--no-disc` build

Launch the app. With no disc anywhere it shows a URL such as `http://192.168.1.42:8080`. Open that
on a phone or laptop on the same network and choose your disc image.

The page checks the file before sending it: a raw `.iso` has its game id read from the header and is
rejected immediately if it is the wrong game. Compressed containers (`.rvz`, `.gcz`, `.wia`,
`.ciso`) can only be checked for format in the browser, so those are verified on the Apple TV after
upload. Either way the disc is verified against the supported-release list before it is accepted.

Large uploads resume: if the transfer is interrupted, reopen the page and choose the same file.

If you have a Mac to hand, `scripts/tvos/push-disc.sh <disc image>` copies one across directly
instead.

## Backing up saves

App data lives in purgeable storage, and tvOS can reclaim it when space runs low.

```sh
scripts/tvos/pull-saves.sh [destination]   # back up
scripts/tvos/push-saves.sh <backup dir>    # restore
```

Take a backup before uninstalling, and before installing a build with a different bundle
identifier — a different identifier means a different data container, and nothing is carried across.
