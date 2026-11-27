#!/usr/bin/env python3
"""mapgen.py — generate pigkeeper's own dink.dat/map.dat (phase 5).

Byte layout transcribed 1:1 from engine/src/editor_screen.cpp
(load_screen_to) and EditorMap.cpp (save): screen record = 31280 bytes.
"""
import struct, sys, os

SCREEN_SIZE = 31280
TILES = 96  # engine reads 97 records; index 0..95 drawn (12x8)

def le(i): return struct.pack('<i', i)

def make_screen(script_name, tile_fn):
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
    for s in range(101):                    # sprites (all inactive)
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

def main(outdir):
    screens = [
        make_screen('s1farm',  grass),            # map index 1
        make_screen('s1squar', grass_with_path),  # map index 2 (east)
    ]
    with open(os.path.join(outdir, 'map.dat'), 'wb') as f:
        for s in screens:
            f.write(s)
    loc = [0] * 769
    music = [0] * 769
    indoor = [0] * 769
    loc[1] = 1
    loc[2] = 2
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
