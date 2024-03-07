{
  description = "Igraph SE2 implementation";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.11";
    nix-matlab = {
      url = "/home/voidee/clones/nix-matlab";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    poetry2nix = {
      url = "github:nix-community/poetry2nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = { self, nixpkgs, nix-matlab, poetry2nix }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs {
        inherit system;
        overlays = [ poetry2nix.overlays.default ];
      };

      pythonEnv = pkgs.poetry2nix.mkPoetryEnv {
        projectDir = ./python;
        editablePackageSources = { speakeasy2 = ./python; };
        preferWheels = true;
      };
    in {
      devShells.${system}.default =
        (pkgs.mkShell.override { stdenv = pkgs.gcc10Stdenv; }) {
          packages = (with nix-matlab.packages.${system}; [
            matlab
            matlab-mlint
            matlab-mex
          ]) ++ [ pythonEnv ] ++ (with pkgs; [
            astyle
            cmake
            ninja
            gdb
            # igraph dependencies
            bison
            flex
            libxml2
            # Poetry dependencies
            poetry
          ]);
          shellHook = ''
            export OMP_NUM_THREADS=16
          '';
        };
    };
}
