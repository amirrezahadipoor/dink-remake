#!/bin/sh
# Fetch and trim the freedink base game data (runtime assets only).
# Produces ./gamedata (~84MB). Used locally and by CI.
set -e
URL="https://ftp.gnu.org/gnu/freedink/freedink-data-1.08.20190120.tar.gz"
TMP="${TMPDIR:-/tmp}/freedink-data.tar.gz"

if [ -d gamedata/dink ]; then
  echo "gamedata already present, skipping."
  exit 0
fi

echo "Downloading base game data..."
curl -sL -o "$TMP" "$URL"
tar xzf "$TMP"
rm -f "$TMP"
mv freedink-data-1.08.20190120 gamedata
# runtime does not need raw art sources / tests / packaging extras
rm -rf gamedata/src gamedata/soundtest gamedata/debian gamedata/dink/l10n gamedata/ChangeLog
echo "OK: $(du -sh gamedata | cut -f1) in ./gamedata"
