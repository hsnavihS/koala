#!/bin/bash

# this flag makes sure that the file exits as soon as a command exits with a non-zero status
set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
RESET='\033[0m'

KOALA_BINARY="./build/koala"
TEST_FILES_DIR="./tests"
OUTPUT_FILES_DIR="./tests/output"

mkdir -p "$OUTPUT_FILES_DIR"

if [ ! -f "$KOALA_BINARY" ]; then
  echo "[OUTPUTGEN] Koala binary not found, building the project"
  ./scripts/build.sh
fi

echo "[OUTPUTGEN] Generating outputs for all test files in $TEST_FILES_DIR"
echo ""

for file in "$TEST_FILES_DIR"/*.kol; do
  filename=$(basename "$file" .kol)
  output_file="$OUTPUT_FILES_DIR/$filename.out"

  echo "[OUTPUTGEN] Running $file -> $output_file"
  
  # pipe stdout and stderr to the output file
  if ! "$KOALA_BINARY" "$file" > "$output_file" 2>&1; then
    echo -e "${RED}[ERROR] Test failed for:${RESET} $file."
    echo ""
    cat "$output_file"
    exit 1
  else
    echo -e "${GREEN}[OK] Test passed:${RESET} $file"
  fi

  echo ""
done

echo -e "${GREEN}[OUTPUTGEN] All test files processed.${RESET}"
