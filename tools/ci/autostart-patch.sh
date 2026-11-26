#!/bin/sh
# CI-only patch: auto-start a new game ~15s after boot, bypassing the
# mouse-driven title menu (SDL relative-warp mouse is unreliable under Xvfb).
# Applied to the *fetched* gamedata copy only - base data stays pristine.
set -e
S=gamedata/dink/Story

cat > "$S/START-AU.c" <<'EOF'
//CI autostart: replicates START-1.c click() (new game) after a delay
void main( void )
{
 wait(15000);
 debug("CI_AUTOSTART_FIRING");
 sp_x(1, 334);
 sp_y(1, 161);
 sp_base_walk(1, 70);
 sp_base_attack(1, 100);
 set_mode(2);
 reset_timer();
 sp_dir(1, 4);
 sp_brain(1, 1);
 sp_que(1, 0);
 sp_noclip(1, 0);
 add_item("item-fst",438, 1);
 &cur_weapon = 1;
 arm_weapon();
 debug("CI_AUTOSTART_OK");
 kill_this_task();
}
EOF

# hook: spawn the autostart script when the title screen is built
sed -i 's/playmidi("1003.mid");/spawn("start-au");\nplaymidi("1003.mid");/' "$S/START.c"
grep -q 'start-au' "$S/START.c"
echo "autostart patch applied."
