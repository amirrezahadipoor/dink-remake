#!/usr/bin/env python3
"""رندر تایل‌لایه صفحه‌ها از map.dat — برای QA بصری و برداشت قالب از بازی پایه."""
import struct, sys, os
from PIL import Image

SS = 31280
SQ = 50

class MapSet:
    def __init__(self, ddat, mdat, tiles_dir):
        d = open(ddat,'rb').read()
        self.locs = struct.unpack('<769i', d[20:20+769*4])
        self.m = open(mdat,'rb').read()
        self.tiles_dir = tiles_dir
        self.sheets = {}
    def sheet(self, n):
        if n not in self.sheets:
            want = f'ts{n+1:02d}.bmp'
            path = None
            for fn in os.listdir(self.tiles_dir):
                if fn.lower() == want:
                    path = os.path.join(self.tiles_dir, fn); break
            self.sheets[n] = Image.open(path).convert('RGB') if path else None
        return self.sheets[n]
    def screen_tiles(self, scr):
        l = self.locs[scr]
        if l <= 0: return None
        off = (l-1)*SS + 20
        out = []
        for t in range(96):
            idx, = struct.unpack('<i', self.m[off+t*80: off+t*80+4])
            out.append(idx)
        return out
    def base_script(self, scr):
        l = self.locs[scr]
        off = (l-1)*SS
        return self.m[off+SS-21-1018-1:off+SS-1018-1].split(b'\0')[0].decode('latin1')
    def sprites(self, scr):
        l = self.locs[scr]
        off = (l-1)*SS + 20 + 97*80 + 160 + 80
        out = []
        for i in range(1, 101):
            r = self.m[off+i*220: off+(i+1)*220]
            x, y, seq, frame, typ, size = struct.unpack('<6i', r[:24])
            active = r[24]
            if active and seq > 0:
                out.append(dict(slot=i, x=x, y=y, seq=seq, frame=frame, type=typ, size=size))
        return out
    def render(self, scr):
        tiles = self.screen_tiles(scr)
        if tiles is None: return None
        im = Image.new('RGB', (600, 400), (255, 0, 255))
        for t, idx in enumerate(tiles):
            sh, sq = idx // 128, idx % 128
            S = self.sheet(sh)
            if S is None: continue
            sx, sy = (sq % 12) * SQ, (sq // 12) * SQ
            if sx + SQ <= S.width and sy + SQ <= S.height:
                im.paste(S.crop((sx, sy, sx+SQ, sy+SQ)), ((t%12)*SQ, (t//12)*SQ))
        return im

if __name__ == '__main__':
    # mapview.py <dink.dat> <map.dat> <tiles_dir> <out_dir> [scr1 scr2 ...|all]
    dd, md, td, out = sys.argv[1:5]
    ms = MapSet(dd, md, td)
    os.makedirs(out, exist_ok=True)
    want = sys.argv[5:]
    scrs = [i for i in range(1,769) if ms.locs[i] > 0] if (not want or want[0]=='all') else [int(x) for x in want]
    for s in scrs:
        im = ms.render(s)
        if im:
            im.save(f'{out}/scr{s:03d}.webp', quality=70)
    print(f"rendered {len(scrs)} screens -> {out}")
