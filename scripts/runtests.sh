#!/bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
RESET='\033[0m'

failed=0

for file in tests/*.kol; do
  base=$(basename "$file" .kol)
  expected="tests/output/$base.out"

  if [ ! -f "$expected" ]; then
    echo -e "${RED}[TEST] Missing output file for ${base}${RESET}"
    echo ""
    failed=1
    continue
  fi

  echo -e "[TEST] Running test: ${base}"
  output=$(./build/koala "$file")

  # diff -y displays the differences side by side
  diff_output=$(diff -B -y <(echo "$output") "$expected")
  if [ $? -ne 0 ]; then
    echo -e "${RED}[ERROR] Output mismatch for $file${RESET}"
    echo ""
    echo "---------------- Side-by-Side Diff ----------------"
    echo -e "$diff_output"
    echo "---------------------------------------------------"
    echo ""
    failed=1
  else
    echo -e "${GREEN}[OK] $file passed${RESET}"
    echo ""
  fi
done

if [ $failed -ne 0 ]; then
  echo -e "${RED}[TEST FAIL] Some tests failed.${RESET}"
  exit 1
else
  echo -e "${GREEN}[TEST SUCCESS] All tests passed!${RESET}"
fi

