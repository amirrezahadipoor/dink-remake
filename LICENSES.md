# Q9 — شناسنامه حقوقی «آخرین خوک‌چران» / Licensing of The Last Pigkeeper

## اجزا و لایسنس‌ها

| جزء | منبع | لایسنس | تعهد ما |
|---|---|---|---|
| موتور (engine/) | GNU FreeDink (gnu.org/software/freedink) + پچ‌های این ریپو | **GPL-3.0-or-later** | کل سورس پچ‌شده در همین ریپو عمومی است ✅ |
| گرافیک/صدا/موسیقی پایه | freedink-data 1.08.20190120 (ftp.gnu.org، sha256 پین‌شده در `fetch-data.sh`) | **GPL-3.0-or-later** (جزئیات per-file در COPYING خود بسته) | بدون تغییر بازتوزیع می‌شود؛ بازفشرده‌سازی ogg فقط تبدیل فرمت است ✅ |
| داستان/اسکریپت/نقشه‌های ریمیک (pigkeeper/) | ساخته همین پروژه | **GPL-3.0-or-later** (هم‌خانواده موتور) | متن‌های فارسی و نقشه‌ها آزادند ✅ |
| فونت فارسی (fonts/Vazirmatn-Regular.ttf) | Vazirmatn — Saber Rastikerdar (github.com/rastikerdar/vazirmatn) | **SIL OFL 1.1** | نام فونت تغییر نکرده؛ OFL اجازه embedding می‌دهد ✅ |
| ابزارها (tools/) و CI | همین پروژه | GPL-3.0-or-later | ✅ |
| SDL2 و کتابخانه‌ها (بیلد اندروید) | libsdl.org | zlib | سازگار با GPL ✅ |

## بازتولیدپذیری سورس (الزام GPL)
- `fetch-data.sh` دیتای پایه را با هش پین‌شده می‌آورد؛ `build-android.sh` + ورک‌فلوی `android.yml` بیلد APK را end-to-end بازتولید می‌کنند — هیچ باینری دست‌سازی در ریپو نیست.
- نسخه APK = `0.1.{run_number}-alpha`؛ هر APK منتشرشده به کامیت و ران CI قابل‌ردیابی است.

## چک‌لیست انتشار (فاز ۷)
- [ ] فایل COPYING (متن کامل GPL-3) در ریشه ریپو عمومی
- [ ] صفحه «درباره» درون بازی: نام FreeDink + Vazirmatn + لینک سورس (الزام OFL/GPL)
- [ ] متادیتای F-Droid: لایسنس GPL-3.0-or-later
