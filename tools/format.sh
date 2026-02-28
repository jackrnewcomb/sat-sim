#!/usr/bin/env bash
set -euo pipefail

# Format all headers/sources under simcore
FILES=$(git ls-files | grep -E '^simcore/.*\.(hpp|h|cpp|cc|cxx)$' || true)
if [ -z "$FILES" ]; then
  echo "No files to format."
  exit 0
fi

clang-format -i $FILES
echo "Formatted:"
echo "$FILES"