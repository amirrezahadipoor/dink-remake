#!/usr/bin/env python3
"""Q3 — ممیزی خودکار: هیچ متن لاتینِ قابل‌نمایش در دیالوگ‌ها/منوهای pigkeeper.
خطوط say/say_xy/say_stop و خطوط گزینه ("...") داخل choice بررسی می‌شوند.
مجاز: متغیرها (&savegameinfo)، کدهای رنگ (`$)، اعداد، نشانه‌ها."""
import glob, re, sys

bad = []
for f in sorted(glob.glob('pigkeeper/Story/*.c')):
    if 'sample-' in f: continue
    in_choice = False
    for i, l in enumerate(open(f, encoding='utf-8'), 1):
        s = l.strip()
        if s.startswith('//'): continue
        if 'choice_start' in s: in_choice = True
        if 'choice_end' in s: in_choice = False
        texts = re.findall(r'say(?:_stop|_xy)?\s*\(\s*"([^"]*)"', s)
        if in_choice:
            m = re.match(r'^"([^"]*)"', s)
            if m: texts.append(m.group(1))
        for t in texts:
            t2 = re.sub(r'&\w+', '', t)        # متغیرها
            t2 = t2.replace('FreeDink', '')     # نام موتور — اعتبار GPL در صفحه عنوان، مجاز
            t2 = re.sub(r'`.', '', t2)          # کد رنگ
            if re.search(r'[A-Za-z]{2,}', t2):
                bad.append(f"{f}:{i}: {t}")
if bad:
    print("PERSIAN AUDIT FAIL — متن لاتین در گیم‌پلی:")
    print("\n".join(bad)); sys.exit(1)
print("persian-audit OK (همه متن‌های نمایشی فارسی‌اند)")
