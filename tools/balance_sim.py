#!/usr/bin/env python3
"""B7/B2/B3/B4 — شبیه‌ساز بالانس: TTK + دقیقه/لول + اقتصاد پرده + باس با ۲ لول پایین‌تر.
هر قانون پذیرش که نقض شود CI قرمز می‌شود."""
import sys, re

HITS_PER_SEC = 1.2
KILLS_PER_MIN = 2.5          # نرخ کشتار بازی عادی (شامل جابه‌جایی)
TTK_MIN, TTK_MAX = 2, 14     # دشمن عادی در لول ۱
BOSS_MIN, BOSS_MAX = 10, 45  # باس در لول توصیه‌شده
BOSS_UNDERLVL_CAP = 90       # B4: باس با ۲ لول پایین‌تر: سخت ولی ممکن (≤ ۹۰ ثانیه)
MAX_MIN_PER_LEVEL = 25       # B2
START_LIFE, LIFE_PER_LVL = 10, 3
DODGE = 0.5                  # «با مهارت»: نصف ضربه‌های باس جاخالی
ELIXIR_LIFE, ELIXIRS = 10, 2 # B3↔B4: اقتصاد ۲ الیکسیر قبل هر باس را تأمین می‌کند (economy.md)

def pdmg(level):   # همه امتیازها روی حمله (سقف خوش‌بینانه مدل)
    return 4 + (level - 1)

def plife(level):
    return START_LIFE + LIFE_PER_LVL * (level - 1)

def load_enemies(path):
    rows = []
    for line in open(path, encoding='utf-8'):
        line = line.strip()
        if not line or line.startswith('#'): continue
        p = [x.strip() for x in line.split('|')]
        if len(p) < 7: continue
        rec = int(p[7]) if len(p) > 7 and p[7].isdigit() else None
        rows.append(dict(name=p[0], hp=int(p[1]), dmg=int(p[2]), df=int(p[3]),
                         exp=int(p[4]), rec=rec))
    return rows

def ttk(hp, armor, level):
    eff = max(1, pdmg(level) - armor)
    return hp / (eff * HITS_PER_SEC)

def main():
    fails = []
    enemies = load_enemies('pigkeeper/data/enemies.md')

    # --- B7/B1: TTK ---
    for e in enemies:
        if e['rec']:  # باس
            t = ttk(e['hp'], e['df'], e['rec'])
            ok = BOSS_MIN <= t <= BOSS_MAX
            print(f"{e['name']} [BOSS L{e['rec']}]: TTK={t:.1f}s {'OK' if ok else '*** OUT ***'}")
            if not ok: fails.append(f"TTK boss {e['name']}")
            # --- B4: دو لول پایین‌تر ---
            lu = max(1, e['rec'] - 2)
            tu = ttk(e['hp'], e['df'], lu)
            boss_dps_in = e['dmg'] * HITS_PER_SEC * 0.5 * DODGE   # باس کندتر می‌زند
            surv = (plife(lu) + ELIXIR_LIFE * ELIXIRS) / max(0.1, boss_dps_in)
            ok2 = tu <= BOSS_UNDERLVL_CAP and surv >= tu * 0.9
            print(f"  B4 under-level L{lu}: TTK={tu:.1f}s survive={surv:.1f}s {'OK' if ok2 else '*** FAIL ***'}")
            if not ok2: fails.append(f"B4 {e['name']}")
        else:
            t = ttk(e['hp'], e['df'], 1)
            ok = TTK_MIN <= t <= TTK_MAX
            print(f"{e['name']}: TTK(L1)={t:.1f}s {'OK' if ok else '*** OUT ***'}")
            if not ok: fails.append(f"TTK {e['name']}")

    # --- B2: دقیقه/لول از xp.md ---
    for line in open('pigkeeper/data/xp.md', encoding='utf-8'):
        line = line.strip()
        if not line or line.startswith('#'): continue
        p = [x.strip() for x in line.split('|')]
        if len(p) < 4: continue
        lvl, need, avg = int(p[0]), int(p[1]), int(p[3])
        engine = 100 * (lvl - 1) * (lvl - 1)   # next_raise در لول قبلی
        if need != engine:
            fails.append(f"B2 xp L{lvl}: جدول {need} ≠ موتور {engine}"); continue
        mins = need / (avg * KILLS_PER_MIN)
        ok = mins <= MAX_MIN_PER_LEVEL
        print(f"XP L{lvl-1}→{lvl}: {need}xp @ {avg}/kill = {mins:.1f}min {'OK' if ok else '*** SLOW ***'}")
        if not ok: fails.append(f"B2 pacing L{lvl}")

    # --- B3: اقتصاد پرده ---
    inc, need_c = {}, {}
    for line in open('pigkeeper/data/economy.md', encoding='utf-8'):
        line = line.strip()
        if not line or line.startswith('#'): continue
        p = [x.strip() for x in line.split('|')]
        if len(p) < 4: continue
        kind, act, cost = p[0], p[1], int(p[3])
        if kind == 'income': inc[act] = inc.get(act, 0) + cost
        elif kind == 'need': need_c[act] = need_c.get(act, 0) + cost
    for act in sorted(need_c):
        have = sum(v for k, v in inc.items() if k <= act)  # سکه از پرده‌های قبل می‌ماند
        ok = need_c[act] <= have or need_c[act] == 0
        print(f"ECON پرده {act}: درآمد تجمعی {have} / ضروری {need_c[act]} {'OK' if ok else '*** GRIND ***'}")
        if not ok: fails.append(f"B3 act {act}")

    if fails:
        print("BALANCE FAIL:", "; ".join(fails)); return 1
    print("balance_sim v2 OK"); return 0

if __name__ == '__main__':
    sys.exit(main())
