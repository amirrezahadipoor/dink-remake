#!/usr/bin/env python3
"""ممیز سراسری داستان/فنی — «بازیکن خودکار بدبین».
چک‌ها: ارجاع اسکریپت‌ها (spawn/map)، goto/label، seq ها در برابر Dink.ini،
warp های &player_map به صفحه‌های موجود، فلگ‌های یتیم، ژورنال بی‌پایان، دکمه‌های بی‌اسکریپت."""
import glob, os, re, struct, sys

ERR, WARN = [], []
SD = 'pigkeeper/Story'
scripts = {os.path.splitext(os.path.basename(f))[0].lower(): f for f in glob.glob(f'{SD}/*.c')}

# --- Dink.ini: سکانس‌های تعریف‌شده ---
seqs = set()
for line in open('gamedata/dink/Dink.ini', encoding='latin1'):
    m = re.match(r'\s*load_sequence(?:_now)?\s+\S+\s+(\d+)', line, re.I)
    if m: seqs.add(int(m.group(1)))
    m = re.match(r'\s*set_sprite_info\s+(\d+)', line, re.I)
    if m: seqs.add(int(m.group(1)))

# --- dink.dat: صفحه‌های موجود ---
dat = open('pigkeeper/dink.dat','rb').read()
locs = struct.unpack('<769i', dat[20:20+769*4])
live_screens = {i for i in range(1,769) if 0 < locs[i]}

# --- map.dat: اسکریپت‌های پایه صفحه‌ها و اسپرایت‌های نقشه ---
SS=31280
m = open('pigkeeper/map.dat','rb').read()
for scr in sorted(live_screens):
    off = (locs[scr]-1)*SS
    base = m[off+SS-21-1018-1:off+SS-1018-1].split(b'\0')[0].decode('latin1').strip().lower()
    if base and base not in scripts and not os.path.exists(f'gamedata/dink/Story/{base.upper()}.c') and not os.path.exists(f'gamedata/dink/Story/{base}.c'):
        ERR.append(f"map screen {scr}: base script '{base}' وجود ندارد")

flag_sets, flag_gets = {}, {}
j_add, j_done = {}, {}
all_waits = {}

for name, f in sorted(scripts.items()):
    txt = open(f, encoding='utf-8').read()
    lines = txt.split('\n')
    nc = re.sub(r'//.*', '', txt)  # بدون کامنت

    # spawn/load_script
    for t in re.findall(r'(?:spawn|load_script)\s*\(\s*"([^"]+)"', nc):
        if t.lower() not in scripts and not os.path.exists(f'gamedata/dink/Story/{t.upper()}.c'):
            ERR.append(f"{name}: spawn('{t}') — اسکریپت نیست")
    # sp_script
    for tg in re.findall(r'sp_script\s*\(\s*&?\w+\s*,\s*"([^"]+)"', nc):
        if tg.lower() not in scripts and not os.path.exists(f'gamedata/dink/Story/{tg.upper()}.c'):
            ERR.append(f"{name}: sp_script('{tg}') — اسکریپت نیست")
    # button_script
    for t in re.findall(r'button_script\s*\(\s*"([^"]+)"', nc):
        if t.lower() not in scripts:
            ERR.append(f"{name}: button_script('{t}') — اسکریپت نیست")
    # goto/label
    labels = set(re.findall(r'^\s*(\w+):\s*$', nc, re.M))
    for g in re.findall(r'goto\s+(\w+)\s*;', nc):
        if g not in labels:
            ERR.append(f"{name}: goto {g} — label نیست")
    # seq ها
    for args in re.findall(r'create_sprite\s*\(([^)]*)\)', nc):
        p = [a.strip() for a in args.split(',')]
        if len(p) == 5 and p[3].isdigit() and int(p[3]) not in seqs:
            ERR.append(f"{name}: create_sprite seq {p[3]} در Dink.ini نیست")
    for s in re.findall(r'(?:preload_seq|sp_seq|sp_pseq)\s*\(\s*(?:&?\w+\s*,\s*)?(\d+)\s*[),]', nc):
        if int(s) not in (0,) and int(s) not in seqs: ERR.append(f"{name}: seq {s} در Dink.ini نیست")
    # warp ها
    for w in re.findall(r'&player_map\s*=\s*(\d+)', nc):
        if int(w) not in live_screens:
            ERR.append(f"{name}: warp به صفحه {w} — صفحه خالی/ناموجود")
    # فلگ/ژورنال
    for fl in re.findall(r'flag_set\s*\(\s*"([^"]+)"', nc): flag_sets.setdefault(fl, []).append(name)
    for fl in re.findall(r'flag_get\s*\(\s*"([^"]+)"', nc): flag_gets.setdefault(fl, []).append(name)
    for j in re.findall(r'journal_add\s*\(\s*"([^"]+)"', nc): j_add.setdefault(j, []).append(name)
    for j in re.findall(r'journal_done\s*\(\s*"([^"]+)"', nc): j_done.setdefault(j, []).append(name)
    # کد رنگ نامعتبر
    for c in re.findall(r'say[^"]*"`(.)', txt):
        if c not in '0123456789$%#@!':
            ERR.append(f"{name}: کد رنگ ناشناخته `{c}")

# فلگ‌های یتیم
for fl, users in flag_gets.items():
    if fl not in flag_sets:
        ERR.append(f"flag '{fl}' خوانده می‌شود ({','.join(users)}) ولی هیچ‌جا set نمی‌شود")
for fl, users in flag_sets.items():
    if fl not in flag_gets:
        WARN.append(f"flag '{fl}' فقط set می‌شود ({','.join(set(users))}) — خوانده نمی‌شود")
# ژورنال بی‌پایان
for j, users in j_add.items():
    if j not in j_done:
        WARN.append(f"کوئست ژورنال '{j}' هرگز done نمی‌شود ({','.join(users)})")
for j, users in j_done.items():
    if j not in j_add:
        ERR.append(f"journal_done('{j}') بدون journal_add ({','.join(users)})")

print(f"— ممیزی {len(scripts)} اسکریپت، {len(live_screens)} صفحه، {len(seqs)} سکانس —")
for e in ERR: print("ERR:", e)
for w in WARN: print("warn:", w)
print(f"\n{len(ERR)} خطا، {len(WARN)} هشدار")
sys.exit(1 if ERR else 0)
