#!/bin/bash

set -e

echo "[BUILD] Starting emscripten configuration and build"
echo ""

if [ -d "build" ]; then
  echo "[BUILD] Removing existing build directory"
  rm -rf build
fi

emcmake cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B ./build
cmake --build ./build

if [ ! -d "site" ]; then
  echo "[BUILD] Creating site directory"
  mkdir site
fi
mv ./build/koala.js ./build/koala.wasm ./site

echo ""
echo "[BUILD] Web build completed"

