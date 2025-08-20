{
  description = "NixOS configuration for DeMoD Communication Framework with Determinate Systems setup";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    dcf-src = {
      url = "github:ALH477/DeMoD-Communication-Framework";
      flake = false;  # Source-only input
    };
    det-nix.url = "github:DeterminateSystems/nix-installer";  # For Determinate tools
  };

  outputs = { self, nixpkgs, flake-utils, dcf-src, det-nix, ... }@inputs:
    let
      system = "x86_64-linux";  # Adjust as needed (e.g., aarch64-linux for ARM)
      pkgs = import nixpkgs { inherit system; };
    in {
      nixosConfigurations.dcf-host = nixpkgs.lib.nixosSystem {
        inherit system;
        modules = [
          # Core NixOS config
          ({ config, pkgs, ... }: {
            boot.loader.systemd-boot.enable = true;
            boot.loader.efi.canTouchEfiVariables = true;

            networking.hostName = "dcf-host";

            users.users.root.initialPassword = "root";  # Change in production

            environment.systemPackages = with pkgs; [
              # DCF C SDK package (built from mono repo source)
              (pkgs.stdenv.mkDerivation rec {
                pname = "dcf-c-sdk";
                version = "5.0.0";
                src = dcf-src + "/c_sdk";  # Subdir from mono repo
                nativeBuildInputs = [ pkgs.cmake pkgs.pkg-config ];
                buildInputs = [
                  pkgs.protobufc  # For libprotobuf-c
                  pkgs.libuuid
                  pkgs.libcjson  # Assuming cjson package
                  pkgs.ncurses
                  pkgs.dlfcn     # For libdl
                ];
                cmakeFlags = [
                  "-DCMAKE_BUILD_TYPE=Release"
                ];
                installPhase = ''
                  make install DESTDIR=$out
                '';
                meta = {
                  description = "C SDK for DeMoD Communication Framework";
                  license = pkgs.lib.licenses.gpl3Only;
                };
              })

              # Determinate Systems tools
              det-nix.packages.${system}.default  # nix-installer package
            ];

            # Enable Determinate Nixd service (custom module for daemon)
            # Based on Determinate docs; adjust config at /etc/determinate/nixd.json as needed
            services.determinate-nixd = {
              enable = true;
              package = pkgs.stdenv.mkDerivation {
                pname = "determinate-nixd";
                version = "latest";
                src = pkgs.fetchurl {
                  url = "https://install.determinate.systems/nixd";  # Placeholder; use actual binary URL or build from source
                  sha256 = "sha256-AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=";  # Replace with real hash (nix-prefetch-url)
                };
                dontUnpack = true;
                installPhase = ''
                  mkdir -p $out/bin
                  cp $src $out/bin/nixd
                  chmod +x $out/bin/nixd
                '';
              };
            };

            # Enable flakes for deterministic builds
            nix = {
              package = pkgs.nixFlakes;
              extraOptions = "experimental-features = nix-command flakes";
            };

            system.stateVersion = "24.05";  # Adjust to your NixOS version
          })
        ];
      };
    };
}
