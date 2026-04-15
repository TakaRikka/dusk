{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };
  outputs = { self, nixpkgs }:
    let
      pkgs = import nixpkgs { system = "x86_64-linux"; };
      dusk = pkgs.stdenv.mkDerivation {
        name = "dusk";
        src = ./.;
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
        ];
      };
    in {
      packages.x86_64-linux.default = dusk;
    };
}
