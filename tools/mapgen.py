#!/usr/bin/env python3
"""mapgen.py — generate pigkeeper's own dink.dat/map.dat (phase 5).

Byte layout transcribed 1:1 from engine/src/editor_screen.cpp
(load_screen_to) and EditorMap.cpp (save): screen record = 31280 bytes.
"""
import struct, sys, os

SCREEN_SIZE = 31280
TILES = 96  # engine reads 97 records; index 0..95 drawn (12x8)

def le(i): return struct.pack('<i', i)

def sprite_record(x=0, y=0, seq=0, frame=1, stype=0, size=100, active=0,
                  brain=0, script='', speed=0, base_walk=-1, timing=33,
                  que=0, hard=1, is_warp=0, warp_map=0, warp_x=0, warp_y=0,
                  touch_damage=0, vision=0, nohit=0):
    b = bytearray()
    b += le(x) + le(y) + le(seq) + le(frame) + le(stype) + le(size)
    b += bytes([1 if active else 0]) + b'\x00'*3
    b += le(0) + le(0) + le(brain)
    sc = script.encode('ascii')[:13]
    b += sc + b'\x00' * (14 - len(sc))
    b += b'\x00' * 38
    b += le(speed) + le(base_walk) + le(-1) + le(-1) + le(-1) + le(timing) + le(que) + le(hard)
    b += le(0)*4                      # alt rect
    b += le(is_warp) + le(warp_map) + le(warp_x) + le(warp_y) + le(-1)  # parm_seq
    b += le(-1) + le(0) + le(0) + le(0) + le(0) + le(0) + le(-1) + le(vision)
    b += le(nohit) + le(touch_damage)
    b += le(0) * 5
    assert len(b) == 220, len(b)
    return bytes(b)

def make_screen(script_name, tile_fn, sprites=()):
    b = bytearray()
    b += b'\x00' * 20                       # name[20]
    for t in range(97):                     # 97 tile records x 80B
        idx = tile_fn(t) if t < TILES else 0
        b += le(idx)                        # square_full_idx0
        b += le(0)                          # property (unused)
        b += le(0)                          # althard
        b += b'\x00' * 6                    # more2/3/4
        b += b'\x00' * 2                    # alignment
        b += b'\x00' * 60                   # buff
    b += b'\x00' * 160                      # v
    b += b'\x00' * 80                       # s
    sl = list(sprites)[:100]
    for s in range(101):                    # sprite slots (1-based in engine)
        if 1 <= s <= len(sl):
            b += sl[s-1]
        else:
            b += b'\x00' * 220
    sc = script_name.encode('ascii')[:20]
    b += sc + b'\x00' * (21 - len(sc))      # script[21]
    b += b'\x00' * 1018
    b += b'\x00' * 1
    assert len(b) == SCREEN_SIZE, len(b)
    return bytes(b)

def grass(t):
    # چمن ساده (فقط فال‌بک)
    return 128 + (t % 4)

# ---- قالب‌های بیوم برداشتی از صفحه‌های دست‌ساز بازی پایه (tools/harvest.py) ----
import re as _re, glob as _glob

def _script_clear_points():
    """نقاط اشغال هر صفحه: create_sprite های اسکریپت صفحه + نقاط فرود warp (sp_x/sp_y بعد از player_map)."""
    sdir = os.path.join(os.path.dirname(__file__), '..', 'pigkeeper', 'Story')
    txt = {}
    for f in _glob.glob(os.path.join(sdir, '*.c')):
        n = os.path.splitext(os.path.basename(f))[0].lower()
        txt[n] = _re.sub(r'//.*', '', open(f, encoding='utf-8').read())
    pts = {}  # screen_script_name -> [(x,y)...]
    def add(scr_script, x, y): pts.setdefault(scr_script, []).append((int(x), int(y)))
    # یک سطح spawn را هم دنبال کن
    def props_of(name, depth=0):
        out = []
        tx = txt.get(name)
        if tx is None: return out
        out += [(int(a), int(b)) for a, b in _re.findall(r'create_sprite\s*\(\s*(-?\d+)\s*,\s*(-?\d+)', tx)]
        if depth < 1:
            for sp in _re.findall(r'spawn\s*\(\s*"([^"]+)"', tx):
                out += props_of(sp.lower(), depth + 1)
        return out
    for n in txt:
        pts[n] = props_of(n)
    # نقاط فرود: &player_map = N ... sp_x(1, X) ... sp_y(1, Y)
    land = {}  # screen_number -> [(x,y)]
    for n, tx in txt.items():
        for m_, x_, y_ in _re.findall(r'&player_map\s*=\s*(\d+)\s*;\s*sp_x\(1,\s*(-?\d+)\)\s*;\s*sp_y\(1,\s*(-?\d+)\)', tx):
            land.setdefault(int(m_), []).append((int(x_), int(y_)))
    return pts, land

def biome_tiles(name):
    arr = _TMPL['biomes'][name]['tiles']
    return lambda t: arr[t]

def biome_sprites(name, script_name, screen_no, pts, land, clearance=60):
    avoid = list(pts.get(script_name, [])) + list(land.get(screen_no, []))
    out = []
    for x, y, seq, frame, typ, size, hard in _TMPL['biomes'][name]['sprites']:
        ok = True
        for ax, ay in avoid:
            if abs(x - ax) < clearance and abs(y - ay) < clearance:
                ok = False; break
        if ok:
            out.append(sprite_record(x=x, y=y, seq=seq, frame=frame,
                                     stype=typ, size=size, active=1, hard=hard))
    return out

import json, os
_TMPL = json.load(open(os.path.join(os.path.dirname(__file__), 'templates.json')))

# الگوهای تایید‌شده از دیتای خانه پایه (Ts01=شیت۰): کف چوبی {6,7,18,19}، تیره=30
def wood(t):
    """کپی ۱:۱ کف/قاب خانه پایه — پارکت واقعی"""
    return _TMPL['house_tiles'][t]

def cave(t):
    """کپی ۱:۱ صفحه غار پایه (شیت 36) — سنگ واقعی"""
    return _TMPL['cave_tiles'][t]

def wall_sprites():
    """دیوارهای داخلی seq31 دقیقاً مطابق خانه پایه"""
    out = []
    for x, y, seq, frame, typ, size, hard in _TMPL['house_walls']:
        out.append(sprite_record(x=x, y=y, seq=seq, frame=frame,
                                 stype=typ, size=size, active=1, hard=hard))
    return out

def main(outdir):
    # چیدمان گرید 32xN: خانه (2) شمالِ مزرعه (34)؛ میدان (35) شرق مزرعه
    # نکته میدانی: اسپرایت‌های map-placed به preload سکانس نیاز دارند؛
    # عناصر بصری از اسکریپت ساخته می‌شوند (S1FARM) — اسلات‌ها برای warp آینده
    pts, land = _script_clear_points()
    def B(name, script, scr_no):
        return make_screen(script, biome_tiles(name),
                           biome_sprites(name, script, scr_no, pts, land))
    screens = [
        make_screen('s1house', wood, wall_sprites()),       # 1: خانه (indoor)
        B('farm',  's1farm',  34),                          # 2: مزرعه
        B('square','s1squar', 35),                          # 3: میدان
        B('fore1', 's2fore1', 33),                          # 4: جنگل غرب ۱
        B('fore2', 's2fore2', 32),                          # 5: جنگل غرب ۲ (مارا)
        B('road',  's5road',  31),                          # 6: جاده ساحلی
        B('port',  's5port',  30),                          # 7: بندر سالت‌مور
        make_screen('s5tav',   wood, wall_sprites()),       # 8: میخانه لنگر کج
        make_screen('s5shop',  wood, wall_sprites()),       # 9: ساعت‌سازی مایلو
        B('mine1', 's6mine1', 93),                          # 10: معدن نمک ۱
        B('mine2', 's6mine2', 94),                          # 11: معدن نمک ۲ (مایلو)
        B('dock',  's7dock',  29),                          # 12: اسکله شبانه (فصل ۷)
        B('marsh', 's8marsh', 28),                          # 13: مرداب خاموش
        B('grivn', 's8grivn', 27),                          # 14: دهکده گریون
        make_screen('s8chrch', wood, wall_sprites()),       # 15: کلیسای غرق (دفتر)
        B('camp',  's9camp',  26),                          # 16: کمپ شب آخر
        B('yard',  's10yard', 25),                          # 17: حیاط دژ خاکستر
        make_screen('s10kiln', cave),                       # 18: تالار کوره
        make_screen('s11top',  cave),                       # 19: سکوی کوره (فینال)
        B('epi',   's11epi',  90),                          # 20: اپیلوگ بهار
        B('south', 's1south', 66),                          # 21: جنوب روستا (کوئست‌ها)
    ]
    idx_house, idx_farm, idx_square = 2, 34, 35
    idx_f1, idx_f2 = 33, 32
    idx_road, idx_port, idx_tav = 31, 30, 62
    idx_shop, idx_m1, idx_m2 = 61, 93, 94
    idx_dock, idx_marsh, idx_grivn, idx_chrch = 29, 28, 27, 59
    idx_camp, idx_yard, idx_kiln = 26, 25, 57
    idx_top, idx_epi = 89, 90
    idx_south = 66
    with open(os.path.join(outdir, 'map.dat'), 'wb') as f:
        for s in screens:
            f.write(s)
    loc = [0] * 769
    music = [0] * 769
    indoor = [0] * 769
    # ۵-۸/Q7: موسیقی هر صفحه (N → Sound/N.mid دیتای پایه — همه آزاد/GPL)
    MUSIC = {
        2: 7,    # خانه — آرام
        34: 7,   35: 7,   66: 7,          # پاین‌هالو
        33: 12,  32: 12,                  # جنگل غرب — ماجرا
        31: 13,  30: 13,  62: 104, 61: 104,  # ساحل/بندر + میخانه/مغازه
        93: 18,  94: 18,                  # معدن — تاریک
        29: 105, 28: 105, 27: 105,        # اسکله شبانه/مرداب/گریون — غمگین
        59: 2,                            # کلیسای غرق
        26: 12,  25: 18,                  # کمپ / حیاط دژ
        57: 2,   89: 2,                   # کوره/سکو — سنگین
        90: 7,                            # اپیلوگ بهار
    }
    for scr, mu in MUSIC.items():
        music[scr] = mu
    loc[idx_house] = 1
    loc[idx_farm] = 2
    loc[idx_square] = 3
    loc[idx_f1] = 4
    loc[idx_f2] = 5
    loc[idx_road] = 6
    loc[idx_port] = 7
    loc[idx_tav] = 8
    loc[idx_shop] = 9
    loc[idx_m1] = 10
    loc[idx_m2] = 11
    indoor[idx_house] = 1
    indoor[idx_tav] = 1
    indoor[idx_shop] = 1
    loc[idx_dock] = 12
    loc[idx_marsh] = 13
    loc[idx_grivn] = 14
    loc[idx_chrch] = 15
    indoor[idx_m1] = 1
    indoor[idx_m2] = 1
    loc[idx_camp] = 16
    loc[idx_yard] = 17
    loc[idx_kiln] = 18
    indoor[idx_chrch] = 1
    loc[idx_top] = 19
    loc[idx_epi] = 20
    loc[idx_south] = 21
    indoor[idx_kiln] = 1
    indoor[idx_top] = 1
    with open(os.path.join(outdir, 'dink.dat'), 'wb') as f:
        name = b'Pigkeeper'
        f.write(name + b'\x00' * (20 - len(name)))
        for v in loc:    f.write(le(v))
        for v in music:  f.write(le(v))
        for v in indoor: f.write(le(v))
        f.write(b'\x00' * 2240)
    # verify round-trip sizes
    assert os.path.getsize(os.path.join(outdir, 'map.dat')) == SCREEN_SIZE * len(screens)
    assert os.path.getsize(os.path.join(outdir, 'dink.dat')) == 20 + 769*4*3 + 2240
    print(f"mapgen OK: {len(screens)} screens -> {outdir}")

if __name__ == '__main__':
    main(sys.argv[1] if len(sys.argv) > 1 else 'pigkeeper')
