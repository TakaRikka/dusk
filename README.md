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
Clone and initialize the Dusk repository
```sh
git clone --recursive https://github.com/TakaRikka/dusk.git
cd dusk
git pull
git submodule update --recursive
```

#### Building
**Visual Studio (Recommended for Windows)**
```sh
cmake -B build/dusk -G "Visual Studio 17 2022" -A x64 # Win32 for 32bit
```

**Visual Studio Code with ninja**

Project has support for the CMakeTools extension with variants and has a debug launch target

**ninja (Windows/macOS/Linux)**
```sh
cmake -B build/dusk -GNinja
ninja -C build/dusk
```

#### Running
Pass the disc image as a positional argument. Supported formats: ISO (GCM), RVZ, WIA, WBFS, CISO, GCZ
```sh
build/dusk/dusk /path/to/game.rvz
```
If no path is specified, Dusk defaults to `game.iso` in the current working directory.

#### 30 FPS on Debug
When compiled fully in a Debug the game runs too slowly to hit playable 30 FPS. To avoid this, you can set a CMake cache variable to optimize specific critical files without hampering debuggability in the rest of the program: `-DDUSK_SELECTED_OPT=ON`. When building for MSVC (Windows) you must also modify `CMAKE_CXX_FLAGS_DEBUG` and `CMAKE_C_FLAGS_DEBUG` to remove `/RTC1` from the flags, like so: `-DCMAKE_CXX_FLAGS_DEBUG="/MDd /Zi /Ob0 /Od" -DCMAKE_C_FLAGS_DEBUG="/MDd /Zi /Ob0 /Od"`
