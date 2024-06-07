{
  description = "Flake for AI class";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }: 
    flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs { inherit system; };
    in {
      devShell = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
          python310
          clang
          clang-tools
          valgrind
          gdb
          lldb
          gnumake
        ];
      };
    }
  );
}
