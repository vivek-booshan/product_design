let
  nativePkgs = import <nixpkgs> {};
  crossPkgs = import <nixpkgs> { crossSystem = {
      config = "aarch64-unknown-linux-gnu";
    # config = "armv7l-unknown-linux-gnueabihf"; # if 32 bit OS
    # gnu - eabi (embedded ABI) - hf (hard float)
  };
};
in
nativePkgs.mkShell {
  nativeBuildInputs = with nativePkgs; [
    zig
    zls
    pkg-config
    clang-tools
    cmake
  ];
  buildInputs = [
    crossPkgs.wiringpi
    crossPkgs.ncurses
    nativePkgs.ncurses
    # nativePkgs.tmux
    # nativePkgs.zig
    # nativePkgs.zls
  ];

  shellHook = ''
    export WIRINGPI_PATH="${crossPkgs.wiringpi}"
    export NCURSES_INCLUDE="${nativePkgs.ncurses.dev}/include"
    export NCURSES_PATH="${nativePkgs.ncurses}"
    export NCURSES_PATH_ARM="${crossPkgs.ncurses}"
  '';
}
