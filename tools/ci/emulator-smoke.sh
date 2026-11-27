#!/usr/bin/env bash
# E2E دوفازی APK خوک‌چران روی امولاتور.
# فاز A: مسیر واقعی کاربر — لانچ ساده، عنوان فارسی dmod، تپ «شروع بازی».
# فاز B: دموی کامل via extra ci=99 (همان APK، بدون پچ).
set -e

adb logcat -G 16M || true
adb install android-project/app/build/outputs/apk/debug/app-debug.apk

wait_pid() {
  APPPID=""
  for k in $(seq 1 15); do
    sleep 3
    APPPID=$(adb shell pidof ir.pigkeeper.dink | tr -d '\r')
    [ -n "$APPPID" ] && break
  done
  echo "app pid: $APPPID"
  [ -n "$APPPID" ]
}

SIZE=$(adb shell wm size | grep -oE '[0-9]+x[0-9]+' | tail -1)
W=${SIZE%x*}; H=${SIZE#*x}
if [ "$W" -lt "$H" ]; then TMPD=$W; W=$H; H=$TMPD; fi
echo "screen(landscape): ${W}x${H}"
# مختصات letterbox بازی 640x480 داخل صفحه (همان فرمول flipStretch)
if [ $((W*3)) -gt $((H*4)) ]; then DH=$H; DW=$((H*4/3)); DX=$(((W-DW)/2)); DY=0; else DW=$W; DH=$((W*3/4)); DX=0; DY=$(((H-DH)/2)); fi
g2px() { echo $((DX + $1 * DW / 640)); }
g2py() { echo $((DY + $1 * DH / 480)); }

# ================= فاز A: مسیر کاربر =================
adb logcat -c || true
adb shell am start -n ir.pigkeeper.dink/.SplashActivity
wait_pid
sleep 25   # استخراج دیتا + بوت + عنوان
adb exec-out screencap -p > emu_title.png || true
adb logcat -d > logcat_a.txt || true
echo "--- phase A boot:"; grep -a "pigkeeper\|PIGKEEPER" logcat_a.txt | head -8 || true
# dmod ریمیک واقعاً لود شده (نه منوی قدیمی دینک!)
grep -aq "PIGKEEPER_BOOT_OK" logcat_a.txt
grep -aq "MAKE_BUTTON btnstart" logcat_a.txt
grep -aq "TOUCH_CONTROLS_ON" logcat_a.txt
# عنوان نباید خودکار وارد بازی شود (۲۵ ثانیه گذشته!)
! grep -aq "PIGKEEPER_AUTOSTART" logcat_a.txt
# تپ روی «شروع بازی» (مرکز دکمه: 320,243 در مختصات بازی)
adb shell input touchscreen tap "$(g2px 320)" "$(g2py 243)"
sleep 6
adb logcat -d > logcat_a.txt || true
grep -aq "TOUCH_TITLE_TAP" logcat_a.txt
grep -aq "BTN_START_CLICKED" logcat_a.txt
grep -aq "PIGKEEPER_AUTOSTART" logcat_a.txt   # ورود به بازی پس از تپ
grep -aq "CH1_FARM_OK" logcat_a.txt
echo "PHASE A (real user path) PASSED ✓"
adb shell am force-stop ir.pigkeeper.dink
sleep 3

# ================= فاز B: دموی کامل =================
adb logcat -c || true
adb shell am start -n ir.pigkeeper.dink/.SplashActivity --es ci 99
wait_pid
adb logcat -P "$APPPID" || true
sleep 45   # عنوان ۱۲.۶ ثانیه + زنجیره دمو

# لمس گیم‌پلی: سوایپ حرکت + تپ حمله/گفتگو
adb shell input touchscreen swipe $((W*18/100)) $((H*55/100)) $((W*33/100)) $((H*55/100)) 1200
sleep 2
adb shell input touchscreen tap $((W*875/1000)) $((H*78/100))
sleep 1
adb shell input touchscreen tap $((W*875/1000)) $((H*78/100))
sleep 2
adb shell input touchscreen tap $((W*72/100)) $((H*60/100))
sleep 3

adb exec-out screencap -p > emu_boot.png || true
MEMKB=$(adb shell dumpsys meminfo ir.pigkeeper.dink 2>/dev/null | grep -m1 "TOTAL PSS:" | grep -oE '[0-9]+' | head -1)
echo "PERF mem_total_pss_kb=$MEMKB"
test -n "$MEMKB" && test "$MEMKB" -le 500000
adb shell input keyevent 3
sleep 4
adb logcat -d > logcat.txt || true
echo "--- pigkeeper:"; grep -a "I pigkeeper" logcat.txt | head -6 || true
echo "--- touch:";     grep -a "TOUCH_" logcat.txt | head -10 || true

# ===== assert های فاز B =====
grep -aq "Extracting game data" logcat_a.txt || grep -aq "Game data extracted" logcat_a.txt || true
grep -aq "CI demo mode: PIG_DEMO_STOP=99" logcat.txt
grep -aq "PIGKEEPER_BOOT_OK" logcat.txt
grep -aq "PIGKEEPER_INGAME" logcat.txt
grep -aq "CI_TELEPORT_PORT" logcat.txt
! grep -aq "FATAL EXCEPTION" logcat.txt
! grep -aq "UnsatisfiedLinkError" logcat.txt
grep -aq "TOUCH_CONTROLS_ON" logcat.txt
grep -aq "TOUCH_STATE mv\[r1" logcat.txt
grep -aq "TOUCH_TAP" logcat.txt
grep -aq "TOUCH_LAYOUT" logcat.txt
grep -aq "LIFECYCLE_AUTOSAVE" logcat.txt
# فریم‌ریت
FPS=$(grep -a "PERF fps=" logcat.txt | tail -1 | grep -oE 'fps=[0-9]+' | grep -oE '[0-9]+')
echo "PERF fps=$FPS"
test -n "$FPS" && test "$FPS" -ge 20
echo "PHASE B (full demo) PASSED ✓"
