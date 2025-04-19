#!/bin/bash

CC="zig cc"
WFLAGS="-Wall -Wextra -pedantic"
SRC="main.c temperature.c init_serial_port.c threads.c"
OUT="main"
TARGET="--target=aarch64-linux-gnu"
SCP_PATH="mafia@raspberrypi.local:/home/mafia/repos"

echo "Building $OUT..."
$CC $SRC $WFLAGS $TARGET -o $OUT 

if [ $? -eq 0 ]; then
  echo "Build Succeeded: ./$OUT"
else
  echo "Build Failed"
fi

echo "copying to pi"
scp $OUT $SCP_PATH
