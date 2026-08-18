#!/usr/bin/env bash
# Stamps a version string into every file that hardcodes it, so the
# built binary, --version output, and README always match the release tag.
#
# Usage: scripts/stamp-version.sh 0.6.0
set -euo pipefail

VERSION="${1:-}"
if [ -z "$VERSION" ]; then
  echo "error: no version given (usage: stamp-version.sh 0.6.0)" >&2
  exit 1
fi

echo "Stamping version: $VERSION"

# Tracks whether every expected substitution actually matched something.
# A pattern that silently matches zero lines is worse than a script that
# errors out loudly, since it would otherwise ship a binary whose
# --version output disagrees with the release tag.
FAILED=0

stamp() {
  local file="$1"
  local pattern="$2"
  local label="$3"

  if [ ! -f "$file" ]; then
    echo "error: $file not found (looking for $label)" >&2
    FAILED=1
    return
  fi

  local before
  before="$(grep -cE "$pattern" "$file" || true)"

  if [ "$before" -eq 0 ]; then
    echo "error: pattern for '$label' not found in $file (nothing was stamped)" >&2
    FAILED=1
    return
  fi

  sed -i -E "s/$pattern/\1${VERSION}\4/" "$file"
}

# src/help.cpp
#   Chain Version 0.5.2 (Full Release)
#   NEBANIA CHAIN v0.5 (Full Release) HELP
stamp src/help.cpp \
  '(Chain Version )[0-9]+(\.[0-9]+)*(-[A-Za-z0-9.]+)?( \(Full Release\))' \
  "Chain Version ... (Full Release)"

stamp src/help.cpp \
  '(NEBANIA CHAIN v)[0-9]+(\.[0-9]+)*(-[A-Za-z0-9.]+)?( \(Full Release\) HELP)' \
  "NEBANIA CHAIN v... (Full Release) HELP"

# src/main.cpp
#   Nebania Chain v0.5 (Analyzer)
stamp src/main.cpp \
  '(Nebania Chain v)[0-9]+(\.[0-9]+)*(-[A-Za-z0-9.]+)?( \(Analyzer\))' \
  "Nebania Chain v... (Analyzer)"

# README.md
#   Version   : 0.5 (Full Release)
#   Advanced Features (v0.5 Major Update)
stamp README.md \
  '(Version[[:space:]]*: )[0-9]+(\.[0-9]+)*(-[A-Za-z0-9.]+)?( \(Full Release\))' \
  "Version   : ... (Full Release)"

stamp README.md \
  '(Advanced Features \(v)[0-9]+(\.[0-9]+)*(-[A-Za-z0-9.]+)?( Major Update\))' \
  "Advanced Features (v... Major Update)"

if [ "$FAILED" -ne 0 ]; then
  echo "error: one or more version patterns failed to match — aborting so a mismatched binary isn't built" >&2
  exit 1
fi

echo "Done. Changed lines:"
grep -n "$VERSION" src/help.cpp src/main.cpp README.md || true
