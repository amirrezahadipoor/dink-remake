#!/bin/sh
# Fetch and trim the freedink base game data (runtime assets only).
# Produces ./gamedata (~84MB). Used locally and by CI.
set -e
URL="https://ftp.gnu.org/gnu/freedink/freedink-data-1.08.20190120.tar.gz"
MIRROR="https://ftpmirror.gnu.org/freedink/freedink-data-1.08.20190120.tar.gz"
TMP="${TMPDIR:-/tmp}/freedink-data.tar.gz"

if [ -d gamedata/dink ]; then
  echo "gamedata already present, skipping."
  exit 0
fi

# supply-chain pin (audit #2): official GNU tarball, sha256-verified
SHA256="715f44773b05b73a9ec9b62b0e152f3f281be1a1512fbaaa386176da94cffb9d"

echo "Downloading base game data..."
curl -sL --retry 5 --retry-all-errors --connect-timeout 20 --max-time 300 -o "$TMP" "$URL" || \
curl -sL --retry 5 --retry-all-errors --connect-timeout 20 --max-time 300 -o "$TMP" "$MIRROR"

echo "$SHA256  $TMP" | sha256sum -c - || { echo "CHECKSUM MISMATCH - aborting"; rm -f "$TMP"; exit 1; }
tar xzf "$TMP"
rm -f "$TMP"
mv freedink-data-1.08.20190120 gamedata
# runtime does not need raw art sources / tests / packaging extras
rm -rf gamedata/src gamedata/soundtest gamedata/debian gamedata/dink/l10n gamedata/ChangeLog
echo "OK: $(du -sh gamedata | cut -f1) in ./gamedata"
