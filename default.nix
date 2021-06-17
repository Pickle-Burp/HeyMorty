# default.nix
with import <nixpkgs> {};
stdenv.mkDerivation {
    name = "dev-environment"; # Probably put a more meaningful name here
    buildInputs = [ 
      pkg-config
      curl
      libtensorflow-bin
      glib
      json_c
      gtk3
      fftw
      libpng
    ];
}
