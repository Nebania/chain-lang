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
#   Chain Version 0.5.2 (Full Release)   -> or (Pre Release), (Beta Release), etc.
#   NEBANIA CHAIN v0.5 (Full Release) HELP -> or any other label. This literally was a nightmare to deal with.
#
# The parenthesized label is matched as WHATEVER text is currently there
# (not just "Full Release"/"Pre Release" literally) and overwritten with
# the RELEASE_LABEL requested for this run. This makes stamping resilient
# to label drift (e.g. someone hand-edits it to "Beta Release") instead of
# silently failing to match.
stamp src/help.cpp \
  '(Chain Version )[0-9]+(\.[0-9]+)*(-[A-Za-z0-9.]+)? \([^()]*\)' \
  "\1${VERSION} (${RELEASE_LABEL})" \
  "Chain Version ... (<any label>)"

stamp src/help.cpp \
  '(NEBANIA CHAIN v)[0-9]+(\.[0-9]+)*(-[A-Za-z0-9.]+)? \([^()]*\)( HELP)' \
  "\1${VERSION} (${RELEASE_LABEL})\4" \
  "NEBANIA CHAIN v... (<any label>) HELP"

# src/main.cpp
#   Nebania Chain v0.5 (Analyzer)
# No release-type label here, so this pattern is unchanged.
stamp src/main.cpp \
  '(Nebania Chain v)[0-9]+(\.[0-9]+)*(-[A-Za-z0-9.]+)?( \(Analyzer\))' \
  '\1'"${VERSION}"'\4' \
  "Nebania Chain v... (Analyzer)"

# README.md
#   Version   : 0.5 (Full Release)   -> or (Pre Release), (Beta Release), etc.
#   Advanced Features (v0.5 Major Update)
stamp README.md \
  '(Version[[:space:]]*: )[0-9]+(\.[0-9]+)*(-[A-Za-z0-9.]+)? \([^()]*\)' \
  "\1${VERSION} (${RELEASE_LABEL})" \
  "Version   : ... (<any label>)"

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
