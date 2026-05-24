{
  description = "Dusklight — native PC port of the Twilight Princess decompilation";

  inputs.nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";

  outputs =
    { self, nixpkgs }:
    let
      inherit (nixpkgs) lib;

      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
      forAllSystems = lib.genAttrs supportedSystems;

      dawnVersion = "v20260423.175430";
      nodVersion = "v2.0.0-alpha.8";
      versionSuffix = "nix-" + (self.shortRev or self.dirtyShortRev or "dirty");

      dawnInfo = {
        "x86_64-linux" = {
          triple = "linux-x86_64";
          hash = "sha256-HXfKTLHtMPwupnFnaflCARtXVPuS/0PoCePXidjE5xs=";
        };
        "aarch64-linux" = {
          triple = "linux-aarch64";
          hash = "sha256-34yyFpfqBZUwoFXQ41F0AwAU78FaNihOSY0oriwn6B0=";
        };
        "aarch64-darwin" = {
          triple = "darwin-arm64";
          hash = "sha256-eQnzrBp6gjiBek1VYQ9A5W13ClYWrDDKjIqv/7eNTR4=";
        };
        "x86_64-darwin" = {
          triple = "darwin-x86_64";
          hash = "sha256-QGWiGdxiI9kci3NPXH6QFFirxn16851zB/w3jqhIBJ4=";
        };
      };

      nodPrebuiltInfo = {
        "x86_64-linux" = {
          triple = "linux-x86_64";
          hash = "sha256-mUqvLsbsqaZ+HAjMmHYPYO+MgtanGRTw7Gzn5uXR5rE=";
        };
        "aarch64-darwin" = {
          triple = "macos-arm64";
          hash = "sha256-UPy1ywCcv0K6VJOU3uUelJuUdBh3UNaPRlyP5LOBeDw=";
        };
      };

      perSystem =
        system:
        let
          pkgs = import nixpkgs { inherit system; };
          inherit (pkgs.stdenv.hostPlatform) isDarwin;
          hasNodPrebuilt = nodPrebuiltInfo ? ${system};

          aurora = pkgs.fetchFromGitHub {
            owner = "encounter";
            repo = "aurora";
            rev = "10006618ee493f248b8597e4dfa1d2871d76a1d9";
            hash = "sha256-lY2xuVyB7aPJ9+2wwLRB3F5U/BuPSxdSpegdG+qNd9o=";
          };

          dawn = pkgs.fetchzip {
            url = "https://github.com/encounter/dawn-build/releases/download/${dawnVersion}/dawn-${dawnInfo.${system}.triple}.tar.gz";
            hash = dawnInfo.${system}.hash;
            stripRoot = false;
          };

          corrosion = pkgs.fetchFromGitHub {
            owner = "corrosion-rs";
            repo = "corrosion";
            rev = "v0.6.1";
            hash = "sha256-ppuDNObfKhneD9AlnPAvyCRHKW3BidXKglD1j/LE9CM=";
          };

          nodFromSource = pkgs.stdenv.mkDerivation (finalAttrs: {
            pname = "nod";
            version = nodVersion;
            src = pkgs.fetchFromGitHub {
              owner = "encounter";
              repo = "nod";
              rev = nodVersion;
              hash = "sha256-+zrtVzjo0+X/6uMcNUn1+FaSR+jOhrcQSDNBFjw0NDs=";
            };
            cargoDeps = pkgs.rustPlatform.importCargoLock {
              lockFile = "${finalAttrs.src}/Cargo.lock";
            };
            postPatch = ''
              substituteInPlace CMakeLists.txt \
                --replace-warn "add_subdirectory(nod-ffi/examples)" ""
            '';
            nativeBuildInputs = [
              pkgs.cmake
              pkgs.ninja
              pkgs.rustPlatform.cargoSetupHook
              pkgs.cargo
              pkgs.rustc
            ];
            CARGO_NET_OFFLINE = "true";
            cmakeFlags = [
              "-DFETCHCONTENT_FULLY_DISCONNECTED=ON"
              "-DFETCHCONTENT_SOURCE_DIR_CORROSION=${corrosion}"
              "-DNOD_ENABLE_INSTALL=ON"
              "-DBUILD_SHARED_LIBS=OFF"
            ];
            doCheck = false;
          });

          nod =
            if hasNodPrebuilt then
              pkgs.fetchzip {
                url = "https://github.com/encounter/nod/releases/download/${nodVersion}/libnod-${
                  nodPrebuiltInfo.${system}.triple
                }.tar.gz";
                hash = nodPrebuiltInfo.${system}.hash;
                stripRoot = false;
              }
            else
              nodFromSource;

          fetchContentDirs = {
            DAWN_PREBUILT = dawn;
            NOD_PREBUILT = nod;
            CXXOPTS = pkgs.cxxopts.src;
            JSON = pkgs.nlohmann_json.src;
            XXHASH = pkgs.xxHash.src;
            ZSTD = pkgs.zstd.src;
            FMT = pkgs.fetchzip {
              url = "https://github.com/fmtlib/fmt/archive/refs/tags/11.1.4.tar.gz";
              hash = "sha256-sUbxlYi/Aupaox3JjWFqXIjcaQa0LFjclQAOleT+FRA=";
            };
            TRACY = pkgs.fetchzip {
              url = "https://github.com/wolfpld/tracy/archive/a64b9a20294d59421a2f57aeca3c6383d8c48169.tar.gz";
              hash = "sha256-hbNGOsGeyGSvCJ2No8RkwOib1lX2on3vNZSzyVkZdXw=";
            };
            IMGUI = pkgs.fetchFromGitHub {
              owner = "ocornut";
              repo = "imgui";
              rev = "v1.91.9b-docking";
              hash = "sha256-mQOJ6jCN+7VopgZ61yzaCnt4R1QLrW7+47xxMhFRHLQ=";
            };
            SQLITE3 = pkgs.fetchzip {
              url = "https://sqlite.org/2026/sqlite-amalgamation-3510300.zip";
              hash = "sha256-pNMR8zxaaqfAzQ0AQBOXMct4usdjey1Q0Gnitg06UhM=";
            };
            RMLUI = pkgs.fetchzip {
              url = "https://github.com/mikke89/RmlUi/archive/f9b8c9e2935d5df2c7dff2c190d3968e99b0c3dc.tar.gz";
              hash = "sha256-g4O/JZUrrcseOz8o2QJRt+2CeuiLnVeuDJc906xvuIg=";
            };
          };

          dusklight = pkgs.stdenv.mkDerivation {
            pname = "dusklight";
            version = versionSuffix;
            src = ./.;

            postUnpack = ''
              chmod -R u+w "$sourceRoot"
              rm -rf "$sourceRoot/extern/aurora"
              mkdir -p "$sourceRoot/extern"
              cp -r ${aurora} "$sourceRoot/extern/aurora"
              chmod -R u+w "$sourceRoot/extern/aurora"
              substituteInPlace "$sourceRoot/extern/aurora/CMakeLists.txt" \
                --replace-warn "add_subdirectory(tests)" ""
            '';

            nativeBuildInputs = [
              pkgs.cmake
              pkgs.ninja
              pkgs.pkg-config
              pkgs.python3
              pkgs.python3Packages.markupsafe
            ]
            ++ lib.optionals (!isDarwin) [ pkgs.autoPatchelfHook ];

            buildInputs = [
              pkgs.sdl3
              pkgs.freetype
              pkgs.zstd
              pkgs.cxxopts
              pkgs.nlohmann_json
              pkgs.xxHash
              pkgs.abseil-cpp
              pkgs.zlib
              pkgs.libpng
              pkgs.libjpeg_turbo
              pkgs.curl
              pkgs.openssl
            ]
            ++ lib.optionals isDarwin [
              pkgs.apple-sdk_15
              pkgs.libiconv
            ]
            ++ lib.optionals (!isDarwin) [
              pkgs.libGL
              pkgs.libGLU
              pkgs.libglvnd
              pkgs.vulkan-loader
              pkgs.libX11
              pkgs.libxcb
              pkgs.libXcursor
              pkgs.libxi
              pkgs.libxrandr
              pkgs.libxscrnsaver
              pkgs.libxtst
              pkgs.libxinerama
              pkgs.libxkbcommon
              pkgs.wayland
              pkgs.libdecor
              pkgs.alsa-lib
              pkgs.libpulseaudio
              pkgs.pipewire
              pkgs.dbus
              pkgs.udev
              pkgs.libusb1
              pkgs.libunwind
              pkgs.gtk3
            ];

            cmakeBuildType = "RelWithDebInfo";
            ninjaFlags = [ "dusklight" ];

            cmakeFlags = [
              "-DDUSK_VERSION_OVERRIDE=${versionSuffix}"
              "-DFETCHCONTENT_FULLY_DISCONNECTED=ON"
              "-DAURORA_DAWN_PROVIDER=package"
              "-DAURORA_DAWN_LINKAGE=static"
              "-DAURORA_NOD_PROVIDER=package"
              "-DAURORA_NOD_LINKAGE=static"
              "-DAURORA_SDL3_PROVIDER=system"
            ]
            ++ lib.mapAttrsToList (key: src: "-DFETCHCONTENT_SOURCE_DIR_${key}=${src}") fetchContentDirs;

            installPhase =
              if isDarwin then
                ''
                  runHook preInstall
                  mkdir -p "$out/Applications"
                  cp -r Dusklight.app "$out/Applications/Dusklight.app"
                  runHook postInstall
                ''
              else
                ''
                  runHook preInstall
                  install -Dm755 dusklight "$out/bin/dusklight"
                  cp -r "$src/res" "$out/bin/res"
                  install -Dm644 "$src/platforms/freedesktop/dev.twilitrealm.dusk.desktop" \
                    "$out/share/applications/dev.twilitrealm.dusk.desktop"
                  for size in 16 32 48 64 128 256 512 1024; do
                    install -Dm644 "$src/platforms/freedesktop/''${size}x''${size}/apps/dev.twilitrealm.dusk.png" \
                      "$out/share/icons/hicolor/''${size}x''${size}/apps/dev.twilitrealm.dusk.png"
                  done
                  runHook postInstall
                '';

            dontStrip = true;

            meta = {
              description = "Dusklight — native PC port of the Twilight Princess decompilation";
              homepage = "https://github.com/zeldaret/tp";
              platforms = supportedSystems;
              mainProgram = "dusklight";
            };
          };

          # Tooling common to every supported host (Linux and macOS).
          commonDevTools = [
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
          linuxDevDeps = [
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
          darwinShell = pkgs.mkShellNoCC {
            packages = commonDevTools;
            shellHook = ''
              echo "Dusklight dev shell (macOS)"
              echo "Requires Xcode Command Line Tools for Apple Clang and the macOS SDK."
              echo "Configure: cmake --preset macos-default-relwithdebinfo"
              echo "Build:     cmake --build --preset macos-default-relwithdebinfo"
            '';
          };

          linuxShell = pkgs.mkShell {
            packages = commonDevTools ++ linuxDevDeps;
            shellHook = ''
              echo "Dusklight dev shell (Linux)"
              echo "Configure: cmake --preset linux-default-relwithdebinfo"
              echo "           cmake --preset linux-clang-relwithdebinfo"
              echo "Build:     cmake --build --preset <preset>"
            '';
          };
        in
        {
          packages = {
            default = dusklight;
            dusklight = dusklight;
          }
          // lib.optionalAttrs (!hasNodPrebuilt) { nod = nodFromSource; };

          devShells.default = if isDarwin then darwinShell else linuxShell;
        };

      systems = forAllSystems perSystem;
    in
    {
      packages = lib.mapAttrs (_: s: s.packages) systems;
      devShells = lib.mapAttrs (_: s: s.devShells) systems;
    };
}
