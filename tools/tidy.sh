#!/usr/bin/env bash
set -euo pipefail

# Ensure build exists and compile_commands.json is present
PRESET="${1:-debug}"

cmake --preset "$PRESET"
cmake --build --preset "$PRESET"

BUILD_DIR="build/$PRESET"
CCJSON="$BUILD_DIR/compile_commands.json"

if [ ! -f "$CCJSON" ]; then
  echo "Missing $CCJSON. Ensure CMAKE_EXPORT_COMPILE_COMMANDS is ON."
  exit 1
fi

# Run clang-tidy over tracked source files
FILES=$(git ls-files | grep -E '^simcore/.*\.(cpp|cc|cxx)$' || true)
if [ -z "$FILES" ]; then
  echo "No .cpp files to clang-tidy."
  exit 0
fi

clang-tidy -p "$BUILD_DIR" $FILES
echo "clang-tidy passed."