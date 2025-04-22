#!/bin/bash

set -e

echo "[BUILD] Starting CMake configuration and build"
echo ""

if [ -d "build" ]; then
  echo "[BUILD] Removing existing build directory"
  rm -rf build
fi

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B ./build
cmake --build ./build

echo ""
echo "[BUILD] Native build completed"
