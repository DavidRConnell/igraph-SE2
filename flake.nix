{
  description = "Igraph SE2 implementation";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.11";
    nix-matlab = {
      url = "/home/voidee/clones/nix-matlab";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = { self, nixpkgs, nix-matlab }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      devShells.${system}.default =
        (pkgs.mkShell.override { stdenv = pkgs.gcc10Stdenv; }) {
          buildInputs = (with nix-matlab.packages.${system}; [
            matlab
            matlab-mlint
            matlab-mex
          ]) ++ (with pkgs; [
            astyle
            cmake
            ninja
            gdb
            # igraph dependencies
            bison
            flex
            libxml2
          ]);
          shellHook = ''
            export OMP_NUM_THREADS=16
          '';
        };
    };
}
