#!/usr/bin/env bash
set -euo pipefail

FILES=$(git ls-files | grep -E '^simcore/.*\.(hpp|h|cpp|cc|cxx)$' || true)
if [ -z "$FILES" ]; then
  echo "No files to check."
  exit 0
fi

clang-format --dry-run --Werror $FILES
echo "clang-format check passed."