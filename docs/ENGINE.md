# 🔩 معماری موتور FreeDink 109.6 (مرجع فنی پروژه)

> منبع: مطالعه مستقیم سورس `engine/src/` — این سند مرجع فازهای ۲ تا ۵ است.

## ۱. نقطه ورود و چرخه عمر
- `main()` → `AppFreeDink` (در `AppFreeDink.cpp`، وارث `app.cpp`)
- `app.cpp`: پارس آرگومان‌ها (`--refdir`, `--window`, `--debug`, `--software`, `--truecolor`, `--version`)، init کردن SDL، ساخت نمایشگر، سپس حلقه اصلی
- حلقه اصلی هر فریم `update_frame.cpp::updateFrame()` را صدا می‌زند: پردازش ورودی → اجرای مغزهای اسپرایت‌ها (brains) → اسکریپت‌ها → رندر
- خروج/pause: رویدادهای SDL در `input.cpp` (نکته اندروید: هوک‌های lifecycle باید در فاز ۳ اینجا وصل شوند)

## ۲. پشته گرافیک (دو رندرر!)
| کلاس | فایل | توضیح |
|---|---|---|
| `IOGfxDisplaySW` | `IOGfxDisplaySW.cpp` | رندر نرم‌افزاری کلاسیک (blit روی سرفیس) |
| `IOGfxDisplayGL2` | `IOGfxDisplayGL2.cpp` | رندر **OpenGL ES 2** با شیدر (از `glm` استفاده می‌کند) — پایه فاز ۴-۳ ما برای ۶۰fps اندروید |
- `ImageLoader.cpp`: لود BMP (از فایل آزاد یا از داخل fastfile) + پالت ۸بیتی
- `gfx_sprites.cpp`: جدول سراسری اسپرایت‌ها؛ `dink.ini` در دیتا تعریف seq/frame هاست
- `gfx_fonts.cpp`: رندر متن با **SDL2_ttf + fontconfig** — نقطه ورود فارسی‌سازی فاز ۲-۲ (اینجا باید شکل‌دهی RTL اضافه شود). فونت پیش‌فرض: LiberationSans (داخل `share/`)
- `BgTilesetsManager.cpp`: ۴۱ تایل‌ست پس‌زمینه (`Tiles/Ts*.bmp`)

## ۳. فرمت آرشیو `dir.ff` (FastFile) — از `fastfile.cpp`
```
uint32 LE  N (تعداد ورودی)
N × { uint32 LE offset;  char name[13];  /* nul-terminated */ }
داده‌ها پشت‌سرهم؛ طول ورودی i = offset(i+1) − offset(i)
ورودی آخر sentinel است (اشاره به EOF)
```
- خواندن با `mmap` در یونیکس (سریع) یا stdio
- ابزار ما: `tools/ffextract.py` (list/extract/pack/selftest) — رفت‌وبرگشت تست‌شده روی هر ۱۴۲ آرشیو (۱۴۱ بایت‌به‌بایت یکسان، ۱۰۰٪ محتوا یکسان؛ تنها `inter/Text-box/dir.ff` چیدمان جدولش غیرمتعارف است)

## ۴. مفسر DinkC
- `dinkc.cpp`: پارسر/مفسر اسکریپت‌های `Story/*.c` — بدون کامپایل، در لحظه
- `dinkc_bindings.cpp`: ~۲۰۰ تابع built-in (مثل `create_sprite`, `say_xy`, `playmidi`, `set_mode`)
- `dinkc_console.cpp`: کنسول دیباگ داخل بازی (کلید `~` در حالت debug — برای تست فاز ۵ طلاست)
- الگوی مهم: هر اسپرایت می‌تواند اسکریپت داشته باشد با callbacks: `main/click/touch/talk/hit/die/buttonon/buttonoff`
- **برای ریمیک:** API جدید `journal_add()` و امثالش باید در `dinkc_bindings.cpp` ثبت شوند (فاز ۲-۷)

## ۵. دنیا و نقشه
- `DMod.cpp` + `EditorMap.cpp`: لود `dink.dat` (ایندکس) و `Map.dat` (۷۶۸ صفحه × هر صفحه ۱۲×۸ تایل + لیست اسپرایت)
- `hardness_tiles.cpp` + `Hard.dat`: برخورد
- `live_screen.cpp` / `live_sprites_manager.cpp` / `live_sprite.cpp`: وضعیت زنده صفحه فعلی
- `brain.cpp` + `brains/`(داخل update): هوش‌های آماده (brain 1=دینک، 9=دشمن زمینی، ...)

## ۶. صدا
- `sfx.cpp`: SDL2_mixer؛ `load_sound`/`playsound` از DinkC
- `bgm.cpp`: موسیقی midi/ogg؛ `playmidi("1003.mid")` → اگر فایل `1003.ogg` باشد همان را می‌زند (کلید فاز ۴-۲: جایگزینی midi با ogg بدون دست زدن به اسکریپت‌ها)

## ۷. مسیرها و D-Mod (کلید ریمیک!)
- `paths.cpp`: `--refdir` ریشه دیتا (شامل `dink/`)؛ `--game <dir>` یک D-Mod را لود می‌کند که **روی دیتای پایه سوار می‌شود**: هر فایلی که در D-Mod باشد جایگزین نسخه پایه می‌شود (fallback به `dink/`)
- یعنی ریمیک «آخرین خوک‌چران» = پوشه `pigkeeper/` با Story/نقشه/گرافیک خودش، بدون کپی کردن کل دیتا — دقیقاً معماری‌ای که برای حجم کم APK لازم داریم

## ۸. تست‌های موجود
- `test_*.cpp` + `Test*.cxx` (cxxtest): دینک‌سی، فونت‌ها، نقشه، یکپارچگی — در CI کامپایل می‌شوند (فاز بعدی: اجرایشان در جاب جدا)
- `FakeIOGfxDisplay` برای تست بدون گرافیک واقعی

## ۹. نکات اندروید (برای فاز ۳)
- `SDL_android_main.c` موجود است؛ `IOTouchDragAnywhere.cpp` مکانیک «درگ از هر جای صفحه» برای لمس دارد — پایه خوب کنترل لمسی v1
- `app.cpp` هینت‌های لمس/ماوس را ست می‌کند (پچ ما برای SDL ≥ 2.0.10 اعمال شد)
- رندرر GL2 همان مسیر OpenGL ES موبایل است؛ روی اندروید باید پیش‌فرض شود

## ۱۰. درس‌های میدانی D-Mod (از CI، نشست فاز ۲)
- `--game` مسیر **نسبت به CWD** را می‌گیرد نه refdir — همیشه مسیر مطلق بدهید (روی اندروید هم absolute path داخل حافظه داخلی)
- D-Mod برای دنیا باید `dink.dat` + `map.dat` + `hard.dat` **خودش** را داشته باشد؛ فال‌بک به دیتای پایه فقط برای گرافیک/صدا/اسکریپت است، نه نقشه
- اسکریپت‌های Story در dmod به‌صورت فایل‌به‌فایل override می‌شوند (START.c فارسی ما بدون دست زدن به بقیه کار کرد)
- خطای `Cannot find X file!!!` از `editor_screen.cpp::load_screen_to` یعنی مشکل مسیر dmod، نه خرابی دیتا

## ۱۱. API های جدید ریمیک در DinkC (مرجع فاز ۵)
| تابع | کار | ماندگاری |
|---|---|---|
| `journal_add("عنوان")` | ثبت مأموریت در دفترچه | فایل `journalN.txt` کنار هر اسلات |
| `journal_done("عنوان")` | علامت انجام‌شد (برگشتی: ۱ اگر تازه تیک خورد) | ✓ |
| `journal_say()` | نمایش دفترچه با فونت فارسی | — |
| `flag_set("نام", عدد)` | فلگ داستانی (پایه انتخاب‌ها و ۳ پایان) | فایل `flagsN.txt` |
| `&x = flag_get("نام")` | خواندن فلگ (نبود = ۰) | ✓ |
| `&h = get_hour()` / `set_hour(h)` | ساعت بازی ۰..۲۳ | با فلگ‌ها (خط CLOCK) |
| `&n = is_night()` | شب؟ (۲۰ تا ۵) — تاریکی خودکار صفحه در truecolor | — |

**متغیرهای محیطی موتور (برای تست/اندروید):**
`FREEDINK_FONT` (مسیر فونت) · `FREEDINK_TYPEWRITER=0` (خاموشی تایپ‌رایتر) · `FREEDINK_AUTOSAVE=ثانیه` (اتوسیو، ۰=خاموش، پیش‌فرض ۱۸۰) · `FREEDINK_DAYLEN=ثانیه` (طول شبانه‌روز، پیش‌فرض ۱۲۰۰، ۰=خاموش)

**الگوی برنامه NPC:** `pigkeeper/Story/sample-npc-schedule.c` — حلقه ۲ثانیه‌ای: موقعیت بر اساس `get_hour()`، مخفی‌شدن شبانه، دیالوگ وابسته به ساعت.

## ۱۲. یادداشت‌های عملکرد (دورهای ۱ و ۲)
- رندرر پیش‌فرض GL2 است (`dinkgl=true` در app.cpp) — روشنایی/شب = شیدر، اسکیل = GPU
- تله‌متری: `PERF fps=` (هر ۵s) · `PERF first_frame_ms=` · `PERF save_ms=` — کف‌های CI: دسکتاپ fps≥30، امولاتور ≥20، بوت ≤15s
- الگوهای ممنوع (تجربه‌شده): ساخت/آزادسازی تکسچر در هر فریم (لیبل‌ها کش شدند)، حلقه پیکسلی CPU برای روشنایی (ColorMod)، fopen در هر تعویض صفحه (هندل کش شد)، نوشتن دیسک بی‌تغییر در اتوسیو (dirty-tracking + گارد دریفت ساعت ۶۰دقیقه‌ای)
- باگ ارثی فیکس‌شده: invalidation معکوس کش متن + ذخیره‌نشدن رنگ کش (text.cpp)

## ۱۳. فرمت باینری نقشه (ترنسکرایب‌شده برای mapgen)
- `map.dat`: هر صفحه ۳۱۲۸۰ بایت: name[20] · ۹۷×تایل{idx LE4, prop4, althard4, 6, pad2, buff60}=80B · v[160] · s[80] · ۱۰۱×اسپرایت[220] · script[21] · 1018 · pad1
- `dink.dat`: name[20] · loc[769]·music[769]·indoor[769] (LE int) · 2240
- تایل: `square_full_idx0 = tileset*128 + square` (Ts01 چمن → 128+)
- خوک: brain=4، seq walk پایه 40 (41/43/47/49) — از Dink.ini
- درس D-Mod تکمیلی (CI #72): `dink.ini` **فقط از خود D-Mod** خوانده می‌شود (بی‌فال‌بک) — بدون آن هیچ سکانس lazy ای تعریف نمی‌شود (اسپرایت‌های اسکریپتی نامرئی). بسته‌بندی باید dink.ini را هم مثل hard.dat کپی کند.

- 🐞 ایشوی باز DinkC: callback های `wait()` در اسکریپت‌های spawn-شده گاهی به اسلات ۱ سنجاق و با مرگ اسکریپت والد کشته می‌شوند (لاگ CI #81) — الگوی فعلی: هر کار حیاتی قبل از اولین wait؛ ریشه‌یابی در بک‌لاگ.
