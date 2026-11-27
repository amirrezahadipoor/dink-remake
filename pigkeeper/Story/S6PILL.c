//ساس معدن (B1: hp8 dmg2 exp6)
void main( void )
{
sp_brain(&current_sprite, 9);
sp_speed(&current_sprite, 1);
sp_timing(&current_sprite, 33);
sp_base_walk(&current_sprite, 130);
sp_base_death(&current_sprite, 140);
sp_hitpoints(&current_sprite, 10);
sp_defense(&current_sprite, 0);
sp_strength(&current_sprite, 2);
sp_touch_damage(&current_sprite, 2);
sp_gold(&current_sprite, 2);
sp_exp(&current_sprite, 6);
sp_target(&current_sprite, 1);
debug("CH6_PILL_READY");
}
void hit( void )
{
sp_target(&current_sprite, &enemy_sprite);
playsound(30, 22050, 4000, &current_sprite, 0);
}
void die( void )
{
int &k;
&k = flag_get("ch6_kills");
&k += 1;
flag_set("ch6_kills", &k);
if (&k == 2)
{
 flag_set("ch6_m1_clear", 1);
 say_xy("`%راه باز شد. صدای تق‌تق از عمق معدن می‌آید...", 110, 60);
}
}
