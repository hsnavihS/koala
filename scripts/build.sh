#!/bin/bash

set -e

echo "[BUILD] Starting CMake configuration and build"
echo ""

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B ./build
cmake --build ./build

echo ""
echo "[BUILD] Build completed"
