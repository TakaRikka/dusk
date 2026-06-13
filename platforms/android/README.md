# Android Shell

This directory contains a minimal SDLActivity-based Android app wrapper for Dusklight.

## Prerequisites

- Android SDK installed (`ANDROID_HOME`)
- Android NDK version used by CMake presets (`ANDROID_NDK_VERSION`)
- JDK 17+

Example:

```bash
export ANDROID_HOME="$HOME/Android/Sdk"
export ANDROID_NDK_VERSION="29.0.14206865"
export JAVA_HOME="/usr/lib/jvm/java-17-openjdk"
```

## Build Native Libraries

```bash
cmake --preset android-arm64
cmake --build --preset android-arm64

cmake --preset android-x86_64
cmake --build --preset android-x86_64
```

These builds produce:

- `build/android-arm64/Binaries/libmain.so`
- `build/android-x86_64/Binaries/libmain.so`

## Stage Libraries Into APK Project

```bash
./android/scripts/stage-jni-libs.sh
```

This copies:

- `libmain.so` -> `android/app/src/main/jniLibs/arm64-v8a/`
- `libmain.so` -> `android/app/src/main/jniLibs/x86_64/`

## Refresh SDL Java Shim (Optional)

If you update SDL and want to refresh the embedded Java shim files:

```bash
./android/scripts/sync-sdl-java.sh
```

## Build APK

```bash
cd android
./gradlew :app:assembleDebug
```

Output APK:

- `android/app/build/outputs/apk/debug/app-debug.apk`

## Sign Release APKs

Release builds are signed automatically. If no signing variables are present,
Gradle uses the default Android debug key so the APK can be installed for user
testing.

To sign with a custom keystore, provide:

```bash
export DUSK_ANDROID_KEYSTORE_FILE="/path/to/release.keystore"
export DUSK_ANDROID_KEYSTORE_PASSWORD="..."
export DUSK_ANDROID_KEY_ALIAS="..."
export DUSK_ANDROID_KEY_PASSWORD="..."
./gradlew :app:assembleRelease
```

CI can optionally use `DUSK_ANDROID_KEYSTORE_BASE64` instead of
`DUSK_ANDROID_KEYSTORE_FILE`. Store these as GitHub Actions secrets when custom
signing is needed:

- `DUSK_ANDROID_KEYSTORE_BASE64`
- `DUSK_ANDROID_KEYSTORE_PASSWORD`
- `DUSK_ANDROID_KEY_ALIAS`
- `DUSK_ANDROID_KEY_PASSWORD`

## Launch With Runtime Args (adb)

You can pass command-line args through the activity intent:

```bash
adb shell am start -n dev.twilitrealm.dusk/.DuskActivity \
  --es dusk_args "--backend vulkan"
```

Supported extras:

- `dusk_args`: single shell-like argument string
- `dusk_argv`: string-array argv
