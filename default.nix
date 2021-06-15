# default.nix
with import <nixpkgs> {};
stdenv.mkDerivation {
    name = "dev-environment"; # Probably put a more meaningful name here
    buildInputs = [ 
      pkg-config
      curl
      libtensorflow-bin
      gtk3
  ];
}
