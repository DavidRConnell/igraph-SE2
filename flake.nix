{
  description = "Igraph SE2 implementation";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
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
      devShells.${system}.default = pkgs.mkShell {
        buildInputs = (with nix-matlab.packages.${system}; [
          matlab
          matlab-mlint
          matlab-mex
        ]) ++ (with pkgs; [
          astyle
          bear
          gdb
          gnumake
          igraph

          # igraph dependencies
          arpack
          bison
          blas
          cmake
          flex
          glpk
          gmp
          lapack
          libxml2
          plfit
        ]);
        shellHook = ''
          export OMP_NUM_THREADS=4
          export C_INCLUDE_PATH=${pkgs.igraph.dev}/include/igraph
        '';
      };
    };
}
