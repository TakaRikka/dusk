## Dusk

### Building
#### Prerequisites
* [CMake 3.30+](https://cmake.org)
    * Windows: Install `CMake Tools` in Visual Studio
    * macOS: `brew install cmake`
* [Python 3+](https://python.org)
    * Windows: [Microsoft Store](https://go.microsoft.com/fwlink?linkID=2082640)
        * Verify it's added to `%PATH%` by typing `python` in `cmd`.
    * macOS: `brew install python@3`
* **[Windows]** [Visual Studio 2022 Community](https://www.visualstudio.com/en-us/products/visual-studio-community-vs.aspx)
    * Select `C++ Development` and verify the following packages are included:
        * `Windows 10 SDK`
        * `CMake Tools`
        * `C++ Clang Compiler`
        * `C++ Clang-cl`
* **[macOS]** [Xcode 11.5+](https://developer.apple.com/xcode/download/)
* **[Linux]** Actively tested on Ubuntu 20.04, Arch Linux & derivatives.
    * Ubuntu 20.04+ packages
      ```
      build-essential curl git ninja-build clang lld zlib1g-dev libcurl4-openssl-dev \
      libglu1-mesa-dev libdbus-1-dev libvulkan-dev libxi-dev libxrandr-dev libasound2-dev libpulse-dev \
      libudev-dev libpng-dev libncurses5-dev cmake libx11-xcb-dev python3 python-is-python3 \
      libclang-dev libfreetype-dev libxinerama-dev libxcursor-dev python3-markupsafe libgtk-3-dev
      ```
     * Arch Linux packages
       ```
       base-devel cmake ninja llvm vulkan-headers python python-markupsafe clang lld alsa-lib libpulse libxrandr freetype2
       ```
     * Fedora packages
       ```
       cmake vulkan-headers ninja-build clang-devel llvm-devel libpng-devel
       ```
         * It's also important that you install the developer tools and libraries
           ```
           sudo dnf groupinstall "Development Tools" "Development Libraries"
           ```
#### Setup
1. Clone and initialize the Dusk repository
```sh
git clone --recursive https://github.com/TakaRikka/dusk.git
cd dusk
git pull
git submodule update --recursive
```
2. Generate assets
```sh
cp /path/to/GZ2E01.iso orig/GZ2E01/.
python3 ./configure.py                                                                                          
ninja
```

#### Building
**Visual Studio (Recommended for Windows)**
```sh
cmake -B build/dusk -G "Visual Studio 17 2022" -A x64 # Win32 for 32bit
```
**ninja (Windows/macOS/Linux)**
```sh
cmake -B build/dusk -GNinja
ninja -C build/dusk
```

After building the executable, extract the original game files to a folder named `data` in the same folder as the executable.
