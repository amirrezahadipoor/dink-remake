#!/usr/bin/env python3
"""B7 — شبیه‌ساز TTK: اگر زمان-تا-کشتن از محدوده خارج شود CI قرمز می‌شود."""
import sys, re

PLAYER_DMG_L1 = 4          # مشت پایه منهای دفاع
TTK_MIN, TTK_MAX = 2, 14   # دشمن عادی (لول ۱)
BOSS_MIN, BOSS_MAX = 10, 45  # باس‌ها: نبرد طولانی مجاز (پیشوند نام: باس_ یا دود/گرگ)
HITS_PER_SEC = 1.2
BOSSES = ("دود_یادها", "گرگ_قراردادی")

def main(path='pigkeeper/data/enemies.md'):
    rows = []
    for line in open(path, encoding='utf-8'):
        line = line.strip()
        if not line or line.startswith('#'): continue
        parts = [p.strip() for p in line.split('|')]
        if len(parts) < 6: continue
        name, hp, dmg, armor = parts[0], int(parts[1]), int(parts[2]), int(parts[3])
        eff = max(1, PLAYER_DMG_L1 - armor)
        ttk = hp / (eff * HITS_PER_SEC)
        if name in BOSSES:
            ok = BOSS_MIN <= ttk <= BOSS_MAX
            tag = "BOSS"
        else:
            ok = TTK_MIN <= ttk <= TTK_MAX
            tag = ""

        rows.append((name, ttk, ok))
        print(f"{name}{(' ['+tag+']') if tag else ''}: TTK(L1)={ttk:.1f}s {'OK' if ok else '*** OUT OF RANGE ***'}")
    bad = [r for r in rows if not r[2]]
    if bad:
        print(f"BALANCE FAIL: {len(bad)} enemies out of range"); return 1
    print(f"balance_sim OK ({len(rows)} enemies)"); return 0

if __name__ == '__main__':
    sys.exit(main(*sys.argv[1:]))
