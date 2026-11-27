#!/usr/bin/env python3
"""برداشت قالب صفحه (تایل ۹۶تایی + اسپرایت‌های دکور) از Map.dat بازی پایه → templates.json
فقط دکور امن: active، بدون اسکریپت، بدون warp، brain=0، داخل محدوده بازی."""
import struct, json, sys, os

SS = 31280
def harvest(scr, locs, m):
    l = locs[scr]; assert l > 0, f"screen {scr} empty"
    off = (l-1)*SS
    tiles = []
    for t in range(96):
        idx, = struct.unpack('<i', m[off+20+t*80: off+20+t*80+4])
        tiles.append(idx)
    sprites = []
    soff = off + 20 + 97*80 + 160 + 80
    for i in range(1, 101):
        r = m[soff+i*220: soff+(i+1)*220]
        x, y, seq, frame, typ, size = struct.unpack('<6i', r[:24])
        active = r[24]
        brain, = struct.unpack('<i', r[36:40])
        script = r[40:54].split(b'\0')[0].decode('latin1')
        is_warp, = struct.unpack('<i', r[140:144])
        hard, = struct.unpack('<i', r[120:124])
        if not active or seq <= 0: continue
        if script or is_warp or brain != 0: continue
        if not (-20 <= x <= 680 and -20 <= y <= 440): continue
        sprites.append([x, y, seq, frame, typ, size, hard])
    return {"src": scr, "tiles": tiles, "sprites": sprites}

if __name__ == '__main__':
    d = open('gamedata/dink/Dink.dat','rb').read()
    locs = struct.unpack('<769i', d[20:20+769*4])
    m = open('gamedata/dink/Map.dat','rb').read()
    biomes = {
        "farm": 622, "square": 536, "south": 535,
        "fore1": 766, "fore2": 667,
        "road": 449, "port": 352, "dock": 340,
        "marsh": 70, "grivn": 74,
        "camp": 380, "yard": 94, "epi": 381,
        "mine1": 27, "mine2": 58,
    }
    tpath = 'tools/templates.json'
    T = json.load(open(tpath))
    T['biomes'] = {k: harvest(v, locs, m) for k, v in biomes.items()}
    # لبه دنیای پایه (تایل‌های سیاه ردیف بالا) → کپی ردیف دوم
    for k in ('epi', 'mine1'):
        tl = T['biomes'][k]['tiles']
        tl[0:12] = tl[12:24]
    json.dump(T, open(tpath, 'w'))
    for k, v in T['biomes'].items():
        print(f"{k}: src={v['src']} sprites={len(v['sprites'])}")
