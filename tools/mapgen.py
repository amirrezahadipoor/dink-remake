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
    # tileset 1 (Ts01, grass sheet) -> full idx = 128 + square
    return 128 + (t % 4)

def grass_with_path(t):
    x, y = t % 12, t // 12
    if y in (3, 4):          # a horizontal dirt-ish path strip (variant tiles)
        return 128 + 8 + (x % 4)
    return 128 + (t % 4)

# الگوهای تایید‌شده از دیتای خانه پایه (Ts01=شیت۰): کف چوبی {6,7,18,19}، تیره=30
_WOOD = (6, 7, 18, 19)
def wood(t):
    x, y = t % 12, t // 12
    if x < 2 or x > 9:
        return 30
    return _WOOD[((x - 2) + (y % 2) * 2) % 4]

def cave(t):
    x, y = t % 12, t // 12
    if y == 0 or x == 0 or x == 11:
        return 30
    return _WOOD[(x + y) % 4] if (x + y) % 7 == 0 else 30

def main(outdir):
    # چیدمان گرید 32xN: خانه (2) شمالِ مزرعه (34)؛ میدان (35) شرق مزرعه
    # نکته میدانی: اسپرایت‌های map-placed به preload سکانس نیاز دارند؛
    # عناصر بصری از اسکریپت ساخته می‌شوند (S1FARM) — اسلات‌ها برای warp آینده
    farm_sprites = []
    screens = [
        make_screen('s1house', wood),                       # 1: خانه (indoor)
        make_screen('s1farm',  grass, farm_sprites),        # 2: مزرعه
        make_screen('s1squar', grass_with_path),            # 3: میدان
        make_screen('s2fore1', grass),                      # 4: جنگل غرب ۱
        make_screen('s2fore2', grass),                      # 5: جنگل غرب ۲ (مارا)
        make_screen('s5road',  grass_with_path),            # 6: جاده ساحلی
        make_screen('s5port',  grass_with_path),            # 7: بندر سالت‌مور
        make_screen('s5tav',   wood),                       # 8: میخانه لنگر کج
        make_screen('s5shop',  wood),                       # 9: ساعت‌سازی مایلو
        make_screen('s6mine1', cave),                       # 10: معدن نمک ۱
        make_screen('s6mine2', cave),                       # 11: معدن نمک ۲ (مایلو)
        make_screen('s7dock',  grass_with_path),            # 12: اسکله شبانه (فصل ۷)
        make_screen('s8marsh', grass),                      # 13: مرداب خاموش
        make_screen('s8grivn', grass_with_path),            # 14: دهکده گریون
        make_screen('s8chrch', wood),                       # 15: کلیسای غرق (دفتر)
        make_screen('s9camp',  grass),                      # 16: کمپ شب آخر
        make_screen('s10yard', grass_with_path),            # 17: حیاط دژ خاکستر
        make_screen('s10kiln', cave),                       # 18: تالار کوره
    ]
    idx_house, idx_farm, idx_square = 2, 34, 35
    idx_f1, idx_f2 = 33, 32
    idx_road, idx_port, idx_tav = 31, 30, 62
    idx_shop, idx_m1, idx_m2 = 61, 93, 94
    idx_dock, idx_marsh, idx_grivn, idx_chrch = 29, 28, 27, 59
    idx_camp, idx_yard, idx_kiln = 26, 25, 57
    with open(os.path.join(outdir, 'map.dat'), 'wb') as f:
        for s in screens:
            f.write(s)
    loc = [0] * 769
    music = [0] * 769
    indoor = [0] * 769
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
    indoor[idx_kiln] = 1
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
