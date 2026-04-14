#!/bin/bash -ex
shopt -s extglob

# Get linuxdeploy
cd "$RUNNER_WORKSPACE"
curl -fOL https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-$(uname -m).AppImage
chmod +x linuxdeploy-$(uname -m).AppImage

# Build AppImage
cd "$GITHUB_WORKSPACE"
mkdir -p build/appdir/usr/{bin,share/{applications,icons/hicolor}}
cp -r build/install/!(*.*) build/appdir/usr/bin
cp -r platforms/freedesktop/{16x16,32x32,48x48,64x64,128x128,256x256,512x512,1024x1024} build/appdir/usr/share/icons/hicolor
cp platforms/freedesktop/dusk.desktop build/appdir/usr/share/applications

cd build/install
VERSION="$DUSK_VERSION" NO_STRIP=1 "$RUNNER_WORKSPACE"/linuxdeploy-$(uname -m).AppImage \
  -l /usr/lib/x86_64-linux-gnu/libusb-1.0.so --appdir "$GITHUB_WORKSPACE"/build/appdir --output appimage
