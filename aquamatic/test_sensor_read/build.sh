#!/bin/bash

CC="zig cc"
WFLAGS="-Wall -Wextra -pedantic"
SRC="main.c temperature.c init_serial_port.c"
OUT="main"
TARGET="--target=aarch64-linux-gnu"

echo "Building $OUT..."
$CC $SRC $WFLAGS $TARGET -o $OUT 

if [ $? -eq 0 ]; then
  echo "Build Succeeded: ./$OUT"
else
  echo "Build Failed"
fi
