//S9 «حفار سفید» — مینی‌باس (B1 v2: hp78 dmg4 def1 exp200)
void main( void )
{
sp_brain(&current_sprite, 9);
sp_speed(&current_sprite, 1);
sp_timing(&current_sprite, 33);
sp_base_walk(&current_sprite, 130);
sp_base_death(&current_sprite, 140);
sp_hitpoints(&current_sprite, 78);
sp_defense(&current_sprite, 1);
sp_strength(&current_sprite, 4);
sp_touch_damage(&current_sprite, 4);
sp_gold(&current_sprite, 15);
sp_exp(&current_sprite, 200);
sp_target(&current_sprite, 1);
}
void hit( void )
{
sp_target(&current_sprite, &enemy_sprite);
screen_shake(150, 2);
}
void die( void )
{
flag_set("q9_done", 1);
flag_set("q9_lightstone", 1);
journal_add("سنگ‌نمک درخشان");
journal_done("سنگ‌نمک درخشان");
achieve("حفار سفید");
debug("Q9_DONE");
say_xy("`%سنگ‌نمکِ درخشان! چراغی که روغن نمی‌خواهد.", 120, 60);
}
