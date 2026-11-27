#!/usr/bin/env bash
# Emulator boot-proof for the Pigkeeper APK (runs inside android-emulator-runner).
set -e

adb logcat -G 16M || true
adb install android-project/app/build/outputs/apk/debug/app-debug.apk
adb logcat -c || true
adb shell am start -n ir.pigkeeper.dink/.MainActivity

# resilient wait for process (cold start can be slow on emulator)
APPPID=""
for k in $(seq 1 15); do
  sleep 3
  APPPID=$(adb shell pidof ir.pigkeeper.dink | tr -d '\r')
  [ -n "$APPPID" ] && break
done
echo "app pid: $APPPID"
if [ -z "$APPPID" ]; then
  echo "::error::app never started - dumping diagnostics"
  adb logcat -d > logcat.txt || true
  adb exec-out screencap -p > emu_boot.png || true
  grep -aE "pigkeeper|FATAL|AndroidRuntime" logcat.txt | tail -30 || true
  exit 1
fi

# keep engine logs out of chatty's dedup
adb logcat -P "$APPPID" || true
sleep 90

adb logcat -d > logcat.txt || true
adb exec-out screencap -p > emu_boot.png || true
echo "--- pigkeeper:";      grep -a "I pigkeeper" logcat.txt | head -6 || true
echo "--- engine(SDL/APP):"; grep -a "SDL/APP" logcat.txt | head -20 || true

# ===== strict asserts =====
grep -aq "Extracting game data" logcat.txt
grep -aq "Game data extracted" logcat.txt
grep -aq "SDL/APP" logcat.txt
! grep -aq "FATAL EXCEPTION" logcat.txt
! grep -aq "UnsatisfiedLinkError" logcat.txt
grep -aq "refdir" logcat.txt || grep -aq "AAudioStream" logcat.txt

APPPID2=$(adb shell pidof ir.pigkeeper.dink | tr -d '\r')
echo "app pid after soak: $APPPID2"
test -n "$APPPID2"
echo "emulator boot checks passed ✓"
