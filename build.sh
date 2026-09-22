#!/bin/bash

set -e

export CC=clang
export CXX=clang++

mkdir -p build

cd build

cmake -G Ninja \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_C_FLAGS="-O0 -g" \
    -DCMAKE_CXX_FLAGS="-O0 -g" \
    -DCMAKE_C_COMPILER_LAUNCHER=ccache \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    ..

MAX_THREADS=2
THREADS=$(nproc)

if [ "$THREADS" -gt "$MAX_THREADS" ]; then
    THREADS=$MAX_THREADS
fi

ninja -j "$THREADS"

clear

./ayla run ../src/ayla/src/main.ayla --dump ast