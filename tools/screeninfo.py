#!/usr/bin/env python3
"""کالبدشکافی صفحات دیتای پایه: تایل‌ها + اسپرایت‌های placed (برای قالب‌برداری)"""
import struct, sys
dd=open('gamedata/dink/Dink.dat','rb').read()
loc=[struct.unpack_from('<i',dd,20+4*i)[0] for i in range(769)]
indoor=[struct.unpack_from('<i',dd,20+769*8+4*i)[0] for i in range(769)]
md=open('gamedata/dink/Map.dat','rb').read()

def tiles(scr):
    b=(scr-1)*31280+20
    return [struct.unpack_from('<i',md,b+t*80)[0] for t in range(96)]

def sprites(scr):
    b=(scr-1)*31280+20+97*80+160+80
    out=[]
    for i in range(100):
        o=b+(i+0)*220
        x,y,seq,frame,typ,size=struct.unpack_from('<6i',md,o)
        active=md[o+24]
        hard=struct.unpack_from('<i',md,o+120)[0]
        if active:
            out.append((x,y,seq,frame,typ,size,hard))
    return out

cmd=sys.argv[1]
if cmd=='dump':
    scr=loc[int(sys.argv[2])]
    print("tiles grid:")
    g=tiles(scr)
    for y in range(8): print(' '.join(f"{g[y*12+x]:3d}" for x in range(12)))
    print("placed sprites (x,y,seq,frame,type,size,hard):")
    for s in sprites(scr): print(" ", s)
elif cmd=='find-indoor':
    from collections import Counter
    for mi in range(1,769):
        if loc[mi]>0 and indoor[mi]==1:
            g=tiles(loc[mi]); c=Counter(v//128 for v in g)
            top=c.most_common(1)[0]
            if top[0]!=0:
                print(mi, loc[mi], c.most_common(3))
