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

def wood(t):
    # indoor-ish: از واریانت‌های تیره‌تر همان شیت (برش عمودی)
    return 128 + 32 + (t % 4)

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
    ]
    idx_house, idx_farm, idx_square = 2, 34, 35
    idx_f1, idx_f2 = 33, 32
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
    indoor[idx_house] = 1
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
