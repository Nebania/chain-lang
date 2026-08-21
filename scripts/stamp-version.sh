#!/usr/bin/env bash
# Stamps a version string (and release label) into every file that hardcodes
# it, so the built binary, --version output, and README always match the
# release tag.
#
# Usage: scripts/stamp-version.sh 0.6.0 ["Full Release"|"Pre Release"]
#   - Second arg is optional and defaults to "Full Release".
set -euo pipefail

VERSION="${1:-}"
RELEASE_LABEL="${2:-Full Release}"

if [ -z "$VERSION" ]; then
  echo "error: no version given (usage: stamp-version.sh 0.6.0 [\"Full Release\"|\"Pre Release\"])" >&2
  exit 1
fi

if [ "$RELEASE_LABEL" != "Full Release" ] && [ "$RELEASE_LABEL" != "Pre Release" ]; then
  echo "error: release label must be 'Full Release' or 'Pre Release' (got: '$RELEASE_LABEL')" >&2
  exit 1
fi

echo "Stamping version: $VERSION ($RELEASE_LABEL)"

# Tracks whether every expected substitution actually matched something.
# A pattern that silently matches zero lines is worse than a script that
# errors out loudly, since it would otherwise ship a binary whose
# --version output disagrees with the release tag.
FAILED=0

stamp() {
  local file="$1"
  local pattern="$2"
  local replacement="$3"
  local label="$4"

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

  sed -i -E "s/$pattern/$replacement/" "$file"
}

# src/help.cpp
#   Chain Version 0.5.2 (Full Release)   -> or (Pre Release)
#   NEBANIA CHAIN v0.5 (Full Release) HELP -> or (Pre Release) HELP
#
# The version-label suffix is matched as EITHER "(Full Release)" or
# "(Pre Release)" (whichever is currently stamped), and rewritten using
# the RELEASE_LABEL requested for this run — so re-stamping a beta after
# a full release (or vice versa) works correctly.
stamp src/help.cpp \
  '(Chain Version )[0-9]+(\.[0-9]+)*(-[A-Za-z0-9.]+)? \((Full Release|Pre Release)\)' \
  "\1${VERSION} (${RELEASE_LABEL})" \
  "Chain Version ... (Full Release|Pre Release)"

stamp src/help.cpp \
  '(NEBANIA CHAIN v)[0-9]+(\.[0-9]+)*(-[A-Za-z0-9.]+)? \((Full Release|Pre Release)\)( HELP)' \
  "\1${VERSION} (${RELEASE_LABEL})\5" \
  "NEBANIA CHAIN v... (Full Release|Pre Release) HELP"

# src/main.cpp
#   Nebania Chain v0.5 (Analyzer)
# No release-type label here, so this pattern is unchanged.
stamp src/main.cpp \
  '(Nebania Chain v)[0-9]+(\.[0-9]+)*(-[A-Za-z0-9.]+)?( \(Analyzer\))' \
  '\1'"${VERSION}"'\4' \
  "Nebania Chain v... (Analyzer)"

# README.md
#   Version   : 0.5 (Full Release)   -> or (Pre Release)
#   Advanced Features (v0.5 Major Update)
stamp README.md \
  '(Version[[:space:]]*: )[0-9]+(\.[0-9]+)*(-[A-Za-z0-9.]+)? \((Full Release|Pre Release)\)' \
  "\1${VERSION} (${RELEASE_LABEL})" \
  "Version   : ... (Full Release|Pre Release)"

stamp README.md \
  '(Advanced Features \(v)[0-9]+(\.[0-9]+)*(-[A-Za-z0-9.]+)?( Major Update\))' \
  '\1'"${VERSION}"'\4' \
  "Advanced Features (v... Major Update)"

if [ "$FAILED" -ne 0 ]; then
  echo "error: one or more version patterns failed to match — aborting so a mismatched binary isn't built" >&2
  exit 1
fi

echo "Done. Changed lines:"
grep -n "$VERSION" src/help.cpp src/main.cpp README.md || true
