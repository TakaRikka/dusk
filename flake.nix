{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };
  outputs = { self, nixpkgs }:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
      pkgsFor = system: import nixpkgs { inherit system; };

      # Dependencies that are not packaged in nixpkgs (used by the Linux package build):
      buildSources = pkgs: {
        aurora-src = pkgs.fetchFromGitHub {
          owner = "encounter";
          repo = "aurora";
          rev = "63606a43265a3bc18dafd500ab4d7a2108f109e6";
          hash = "sha256-xBvnAwGwNzav67Ac6oUz7RqDUwqgL2bsME3OOMn8Tqw=";
        };
        dawn-src = pkgs.fetchzip {
          url = "https://github.com/encounter/dawn-build/releases/download/v20260423.175430/dawn-linux-x86_64.tar.gz";
          hash = "sha256-HXfKTLHtMPwupnFnaflCARtXVPuS/0PoCePXidjE5xs=";
          stripRoot = false;
        };
        nod-src = pkgs.fetchzip {
          url = "https://github.com/encounter/nod/releases/download/v2.0.0-alpha.8/libnod-linux-x86_64.tar.gz";
          hash = "sha256-mUqvLsbsqaZ+HAjMmHYPYO+MgtanGRTw7Gzn5uXR5rE=";
          stripRoot = false;
        };
        # The version of imgui on nixpkgs does not map cleanly.
        imgui-src = pkgs.fetchFromGitHub {
          owner = "ocornut";
          repo = "imgui";
          rev = "v1.91.9b-docking";
          hash = "sha256-mQOJ6jCN+7VopgZ61yzaCnt4R1QLrW7+47xxMhFRHLQ=";
        };
        sqlite-src = pkgs.fetchzip {
          url = "https://sqlite.org/2026/sqlite-amalgamation-3510300.zip";
          hash = "sha256-pNMR8zxaaqfAzQ0AQBOXMct4usdjey1Q0Gnitg06UhM=";
        };
        rmlui-src = pkgs.fetchzip {
          url = "https://github.com/mikke89/RmlUi/archive/f9b8c9e2935d5df2c7dff2c190d3968e99b0c3dc.tar.gz";
          hash = "sha256-g4O/JZUrrcseOz8o2QJRt+2CeuiLnVeuDJc906xvuIg=";
        };
      };

      # Dusklight Actual (Linux x86_64 only — relies on prebuilt dawn/nod binaries)
      mkDusklight = pkgs:
        let srcs = buildSources pkgs;
          versionSuffix = if self ? shortRev && self.shortRev != null
            then "nix-${self.shortRev}"
            else "nix-dirty";
        in
        pkgs.stdenv.mkDerivation {
          name = "dusklight";
          src = ./.;
          postUnpack = ''
            mkdir -p $sourceRoot/extern/aurora
            cp -r ${srcs.aurora-src}/. $sourceRoot/extern/aurora/
            chmod -R u+w $sourceRoot/extern/aurora
            sed -i '/add_subdirectory(tests)/d' $sourceRoot/extern/aurora/CMakeLists.txt
          '';
          # Remove last line to re-enable tests
          cmakeFlags = [
            "-DDUSK_VERSION_OVERRIDE=${versionSuffix}"
            "-DFETCHCONTENT_FULLY_DISCONNECTED=ON"
            "-DFETCHCONTENT_SOURCE_DIR_CXXOPTS=${pkgs.cxxopts.src}"
            "-DFETCHCONTENT_SOURCE_DIR_JSON=${pkgs.nlohmann_json.src}"
            "-DFETCHCONTENT_SOURCE_DIR_DAWN_PREBUILT=${srcs.dawn-src}"
            "-DFETCHCONTENT_SOURCE_DIR_XXHASH=${pkgs.xxHash.src}"
            "-DFETCHCONTENT_SOURCE_DIR_FMT=${pkgs.fmt.src}"
            "-DFETCHCONTENT_SOURCE_DIR_TRACY=${pkgs.tracy.src}"
            "-DAURORA_SDL3_PROVIDER=system"
            "-DFETCHCONTENT_SOURCE_DIR_NOD_PREBUILT=${srcs.nod-src}"
            "-DAURORA_NOD_PROVIDER=package"
            "-DFETCHCONTENT_SOURCE_DIR_FREETYPE=${pkgs.freetype.src}"
            "-DFETCHCONTENT_SOURCE_DIR_ZSTD=${pkgs.zstd.src}"
            "-DFETCHCONTENT_SOURCE_DIR_SQLITE3=${srcs.sqlite-src}"
            "-DFETCHCONTENT_SOURCE_DIR_IMGUI=${srcs.imgui-src}"
            "-DFETCHCONTENT_SOURCE_DIR_RMLUI=${srcs.rmlui-src}"
            "-DCMAKE_CROSSCOMPILING=ON" # Tests are not working as I didn't want to work through getting google's test suite working as well. This is the only guard I could find to disable it.
          ];
          installPhase = ''
            mkdir -p $out/bin
            cp dusklight $out/bin/dusklight
            cp -r ./res $out/bin/res

            mkdir -p $out/share/applications
            cp $src/platforms/freedesktop/dusklight.desktop $out/share/applications/dusklight.desktop

            for size in 16 32 48 64 128 256 512 1024; do
              install -Dm644 $src/platforms/freedesktop/''${size}x''${size}/apps/dusklight.png \
                $out/share/icons/hicolor/''${size}x''${size}/apps/dusklight.png
            done
          '';
          nativeBuildInputs = [
            pkgs.cmake
            pkgs.pkg-config
            pkgs.wayland
          ];
          buildInputs = [
            pkgs.libGL
            pkgs.libX11
            pkgs.libXcursor
            pkgs.libxi
            pkgs.libxcb
            pkgs.libxrandr
            pkgs.libxscrnsaver
            pkgs.libxtst
            pkgs.libjpeg8
            pkgs.libxkbcommon
            pkgs.libglvnd
            pkgs.cxxopts
            pkgs.abseil-cpp
            pkgs.sdl3
            pkgs.fmt
            pkgs.tracy
            pkgs.freetype
            pkgs.zstd
          ];
        };

      # Tooling common to every supported host (Linux and macOS).
      commonDevTools = pkgs: [
        pkgs.cmake
        pkgs.ninja
        pkgs.pkg-config
        pkgs.git
        pkgs.python3
        pkgs.python3Packages.markupsafe
        pkgs.rustc
        pkgs.cargo
        pkgs.sccache
      ];

      # Linux-only system libraries — mirrors the apt deps from .github/workflows/build.yml
      # so the cmake presets resolve the same set of headers as CI.
      linuxDevDeps = pkgs: [
        # Compilers / linkers
        pkgs.clang
        pkgs.lld
        # C/C++ utilities
        pkgs.curl
        pkgs.openssl
        pkgs.zlib
        pkgs.libpng
        pkgs.libjpeg_turbo
        pkgs.freetype
        pkgs.zstd
        pkgs.fmt
        pkgs.tracy
        pkgs.cxxopts
        pkgs.abseil-cpp
        pkgs.sdl3
        pkgs.ncurses
        pkgs.libunwind
        pkgs.libusb1
        pkgs.fuse
        # Wayland / display server
        pkgs.wayland
        pkgs.wayland-protocols
        pkgs.libxkbcommon
        pkgs.libdecor
        # OpenGL / Vulkan
        pkgs.libGL
        pkgs.libGLU
        pkgs.libglvnd
        pkgs.vulkan-headers
        pkgs.vulkan-loader
        # X11
        pkgs.libX11
        pkgs.libxcb
        pkgs.libXcursor
        pkgs.libxi
        pkgs.libxrandr
        pkgs.libxscrnsaver
        pkgs.libxtst
        pkgs.libxinerama
        # Audio
        pkgs.alsa-lib
        pkgs.libpulseaudio
        pkgs.pipewire
        # System integration
        pkgs.dbus
        pkgs.udev
        pkgs.gtk3
      ];

      # On macOS we deliberately avoid pulling Nix's cc-wrapper so CMake picks up
      # Apple Clang and the Xcode SDK directly, matching the macOS CI workflow.
      mkDarwinShell = pkgs:
        pkgs.mkShellNoCC {
          packages = commonDevTools pkgs;
          shellHook = ''
            echo "Dusklight dev shell (macOS)"
            echo "Requires Xcode Command Line Tools for Apple Clang and the macOS SDK."
            echo "Configure: cmake --preset macos-default-relwithdebinfo"
            echo "Build:     cmake --build --preset macos-default-relwithdebinfo"
          '';
        };

      mkLinuxShell = pkgs:
        pkgs.mkShell {
          packages = (commonDevTools pkgs) ++ (linuxDevDeps pkgs);
          shellHook = ''
            echo "Dusklight dev shell (Linux)"
            echo "Configure: cmake --preset linux-default-relwithdebinfo"
            echo "           cmake --preset linux-clang-relwithdebinfo"
            echo "Build:     cmake --build --preset <preset>"
          '';
        };

      mkDevShell = pkgs:
        if pkgs.stdenv.isDarwin
        then mkDarwinShell pkgs
        else mkLinuxShell pkgs;
    in {
      packages.x86_64-linux.default = mkDusklight (pkgsFor "x86_64-linux");

      devShells = forAllSystems (system: {
        default = mkDevShell (pkgsFor system);
      });
    };
}