# Installing Dusklight on iOS via iloader

## Prerequisites

- A Windows, Linux, or macOS device 
- iOS device connected to computer via USB 
- Dusklight IPA file (download the latest `Dusklight-vX.X.X-ios-arm64.ipa` from the [releases page](https://github.com/TwilitRealm/dusk/releases))
- Game disc - `GZ2E01` (Gamecube USA) or `GZ2PE01` (Gamecube PAL)

## 1. Install AltServer

- Executable bundles can be installed from [iloader's main page](https://iloader.app/) or [their GitHub](https://github.com/nab138/iloader) for Windows, Linux, and macOS.
- Windows WILL require iTunes to be installed
- Linux WILL require usbmuxd to be installed, this is installed by default in most distros though

## 2. Enable Developer Mode (iOS 16+)

- On your iPhone, go to **Settings > Privacy & Security > Developer Mode**
- Toggle it on and restart when prompted

## 3. Install Dusklight on Your iPhone

1. Sign into your Apple ID (this is required for registering app IDs, it is sent securely and not stored by iloader)
    * You may be prompted to put in a code from your iOS device, do so
2. Plug in your iOS device via USB into your PC. If you're missing a dependency, an error pop-up will tell you to install it
    * You will need to hit `Refresh` after plugging it in at this stage so that it can be detected, it does not automatically refresh
3. Leave settings unchanged (the Anisette server should stay Sidestore (.io))
    3.(a) Installing SideStore directly is not required, but provides you a way to install Dusklight on your phone without being plugged into a computer later
4. Press `Import IPA` and choose your downloaded `Dusk-v.X.X.X-ios-arm64.ipa`, it will begin installing on your device

**NOTE:** *At various stages, you may be prompted to trust your device, do so*

## 3. Getting Dusk trusted
When installing sideloaded iOS applications, at first you will need to manually trust the app due to Apple's security policies
* Go to **Settings > General > VPN & Device Management**
* Tap the Apple ID you signed into iloader with under "Developer App" and tap **Trust**
* Tap **Allow** on the pop-up

## 4. Copy Files to Your iPhone

Transfer the IPA and game disc to your iPhone so they're accessible in the Files app. A few ways to do this:

- **AirDrop** - Right-click the files on your Mac and choose Share > AirDrop
- **iCloud Drive** - Place files in iCloud Drive on your Mac and they'll sync to Files on your iPhone
- **USB transfer** - Connect your iPhone and drag files via Finder's sidebar
- **Cloud storage** - Upload to Google Drive, Dropbox, etc. and download on your iPhone

You may now use Dusklight on iOS, iPadOS!