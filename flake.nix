{
  description = "bamboo tea devShell";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    nixpkgs,
    flake-utils,
    ...
  }:
    flake-utils.lib.eachDefaultSystem (
      system: let
        overlays = [];
        pkgs = import nixpkgs {
          inherit system overlays;
        };
      in
        with pkgs; {
          devShells.default = mkShell {
            buildInputs = [
              arduino-cli
              nixpkgs-fmt
              influxdb2-cli
              influxdb2
              pkg-config
              python3
              python312Packages.pyserial
              screen
            ];

            shellHook = ''
            '';
          };
        }
    );
}
