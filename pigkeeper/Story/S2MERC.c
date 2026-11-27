//مزدور فرقه — آمار از pigkeeper/data/enemies.md (B1)
void main( void )
{
sp_brain(&current_sprite, 9);
sp_speed(&current_sprite, 1);
sp_timing(&current_sprite, 33);
sp_base_walk(&current_sprite, 530);
sp_base_attack(&current_sprite, 540);
sp_size(&current_sprite, 80);
sp_hitpoints(&current_sprite, 22);
sp_defense(&current_sprite, 1);
sp_strength(&current_sprite, 5);
sp_touch_damage(&current_sprite, 5);
sp_gold(&current_sprite, 7);
sp_exp(&current_sprite, 40);
preload_seq(531);
preload_seq(533);
preload_seq(537);
preload_seq(539);
preload_seq(542);
preload_seq(544);
preload_seq(546);
preload_seq(548);
sp_target(&current_sprite, 1);
debug("CH2_MERC_READY");
}

void hit( void )
{
sp_target(&current_sprite, &enemy_sprite);
playsound(30, 21050, 4000, &current_sprite, 0);
}

void die( void )
{
//M8: هر سومین مزدور یک قلب کوچک می‌اندازد (seq 54 دیتای پایه)
int &hd;
&hd = flag_get("drop_rot");
&hd += 1;
flag_set("drop_rot", &hd);
if (&hd >= 3)
{
 flag_set("drop_rot", 0);
 int &hp;
 &hp = create_sprite(&save_x, &save_y, 0, 54, 1);
 sp_script(&hp, "heart");
 sp_touch_damage(&hp, -1);
 sp_kill(&hp, 14000);
}
int &left;
&left = flag_get("ch2_kills");
&left += 1;
flag_set("ch2_kills", &left);
debug("CH2_MERC_DOWN");
if (&left == 2)
{
 flag_set("ch2_f1_clear", 1);
 journal_add("از جنگل غرب عبور کن");
 say_xy("`$دینک: این‌ها دیگه کی بودن؟! ...من نترسیدم. فقط دارم سریع فکر می‌کنم.", 100, 60);
 achieve("اولین نبرد");
}
}
