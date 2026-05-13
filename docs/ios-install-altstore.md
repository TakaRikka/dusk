# Installing Dusklight on iOS via AltStore

## Prerequisites

- Mac with Homebrew installed
- iPhone connected via USB
- Dusklight IPA file (download the latest `Dusklight-vX.X.X-ios-arm64.ipa` from the [releases page](https://github.com/TwilitRealm/dusk/releases))
- Game disc - `GZ2E01` (Gamecube USA) or `GZ2PE01` (Gamecube PAL)

## 1. Install AltServer

```sh
brew install altserver
open -a AltServer
```

AltServer will appear in your menu bar.

## 2. Enable Developer Mode (iOS 16+)

- On your iPhone, go to **Settings > Privacy & Security > Developer Mode**
- Toggle it on and restart when prompted

## 3. Install AltStore on Your iPhone

- Click AltServer in the menu bar
- Click **Install AltStore > [Your iPhone]**
- Enter your Apple ID credentials when prompted
- On your iPhone, go to **Settings > General > VPN & Device Management**
- Tap your Apple ID under "Developer App" and tap **Trust**

## 4. Copy Files to Your iPhone

Transfer the IPA and game disc to your iPhone so they're accessible in the Files app. A few ways to do this:

- **AirDrop** - Right-click the files on your Mac and choose Share > AirDrop
- **iCloud Drive** - Place files in iCloud Drive on your Mac and they'll sync to Files on your iPhone
- **USB transfer** - Connect your iPhone and drag files via Finder's sidebar
- **Cloud storage** - Upload to Google Drive, Dropbox, etc. and download on your iPhone

## 5. Install via AltStore

- Open **AltStore** on your iPhone
- Go to the **My Apps** tab
- Tap the **+** button (top left)
- Open the **Files** app and select the `.ipa` file