#!/usr/bin/env bash
# Emulator boot + touch E2E proof for the Pigkeeper APK.
# The emulator APK embeds the CI autostart script -> reaches gameplay alone.
set -e

adb logcat -G 16M || true
adb install android-project/app/build/outputs/apk/debug/app-debug.apk
adb logcat -c || true
adb shell am start -n ir.pigkeeper.dink/.SplashActivity

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
adb logcat -P "$APPPID" || true

# boot (~8s) + CI autostart fires at +15s -> wait well into gameplay
sleep 45

# ---- touch E2E: swipe in move zone, tap attack & talk zones ----
SIZE=$(adb shell wm size | grep -oE '[0-9]+x[0-9]+' | tail -1)
W=${SIZE%x*}; H=${SIZE#*x}
# اپ landscape-قفل است؛ اگر ابعاد پرتره گزارش شد، فضای نمایشِ چرخیده را فرض کن
if [ "$W" -lt "$H" ]; then TMPD=$W; W=$H; H=$TMPD; fi
echo "screen(landscape space): ${W}x${H}"
adb shell dumpsys input | grep -i "SurfaceOrientation" | head -2 || true
adb shell input touchscreen swipe $((W*18/100)) $((H*55/100)) $((W*33/100)) $((H*55/100)) 1200
sleep 2
adb shell input touchscreen tap $((W*875/1000)) $((H*78/100))   # حمله
sleep 1
adb shell input touchscreen tap $((W*875/1000)) $((H*78/100))
sleep 2
adb shell input touchscreen tap $((W*72/100)) $((H*60/100))     # گفتگو
sleep 3

adb exec-out screencap -p > emu_boot.png || true
# 4-5: مصرف حافظه
MEMKB=$(adb shell dumpsys meminfo ir.pigkeeper.dink 2>/dev/null | grep -m1 "TOTAL PSS:" | grep -oE '[0-9]+' | head -1)
echo "PERF mem_total_pss_kb=$MEMKB"
test -n "$MEMKB" && test "$MEMKB" -le 500000
# 3-9: چرخه عمر — رفتن به پس‌زمینه باید اتوسیو بزند
adb shell input keyevent 3
sleep 4
adb logcat -d > logcat.txt || true
echo "--- pigkeeper:";       grep -a "I pigkeeper" logcat.txt | head -6 || true
echo "--- engine(SDL/APP):"; grep -a "SDL/APP" logcat.txt | head -12 || true
echo "--- touch:";           grep -a "TOUCH_" logcat.txt | head -12 || true

# ===== strict asserts =====
grep -aq "Extracting game data" logcat.txt
grep -aq "Game data extracted" logcat.txt
grep -aq "SDL/APP" logcat.txt
! grep -aq "FATAL EXCEPTION" logcat.txt
! grep -aq "UnsatisfiedLinkError" logcat.txt
grep -aq "refdir" logcat.txt || grep -aq "AAudioStream" logcat.txt
# گیم‌پلی واقعی (autostart داخل APK امولاتور)
grep -aq "CI_AUTOSTART_OK" logcat.txt
# لمس: فعال + حرکت با سوایپ + تپ دکمه (لچ رویدادی)
grep -aq "TOUCH_CONTROLS_ON" logcat.txt
grep -aq "TOUCH_STATE mv\[r1" logcat.txt
grep -aq "TOUCH_TAP" logcat.txt
grep -aq "TOUCH_LAYOUT" logcat.txt
# 3-9: اتوسیو پس‌زمینه
grep -aq "LIFECYCLE_AUTOSAVE" logcat.txt

# perf: گزارش fps روی امولاتور (swiftshader) + کف محافظه‌کارانه
grep -a "PERF fps=" logcat.txt | tail -3 || true
FPSV=$(grep -a "PERF fps=" logcat.txt | tail -1 | grep -oE 'fps=[0-9]+' | cut -d= -f2)
echo "measured fps(emulator): ${FPSV:-none}"
test -n "$FPSV" && test "$FPSV" -ge 20

APPPID2=$(adb shell pidof ir.pigkeeper.dink | tr -d '\r')
echo "app pid after soak: $APPPID2"
test -n "$APPPID2"
echo "emulator boot+touch checks passed ✓"
