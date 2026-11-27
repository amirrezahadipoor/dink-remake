//باس پرده ۱: «گرگ قراردادی» — سرکرده مزدورها (B1: hp30 dmg6 def2 exp25)
void main( void )
{
sp_brain(&current_sprite, 9);
sp_speed(&current_sprite, 1);
sp_timing(&current_sprite, 33);
sp_base_walk(&current_sprite, 640);
sp_base_attack(&current_sprite, 630);
sp_hitpoints(&current_sprite, 30);
sp_defense(&current_sprite, 2);
sp_strength(&current_sprite, 6);
sp_touch_damage(&current_sprite, 6);
sp_exp(&current_sprite, 25);
sp_target(&current_sprite, 1);
debug("CH2_WOLF_READY");
}

void hit( void )
{
sp_target(&current_sprite, &enemy_sprite);
screen_shake(180, 3);
playsound(30, 18050, 4000, &current_sprite, 0);
}

void die( void )
{
flag_set("ch2_wolf_down", 1);
debug("CH2_WOLF_DOWN");
screen_shake(500, 5);
say_xy("`$گوبر (نفس‌نفس): دیدی؟! ماهیتابه‌م صاف خورد تو سرش!", 70, 60);
wait(2200);
say_xy("`%دینک: ...تو کِی رسیدی اینجا؟!", 150, 60);
wait(2000);
say_xy("`$گوبر: از همون اول دنبالت بودم. گفتم شاید نون لازم بشه.", 80, 60);
achieve("گرگ قراردادی");
journal_add("با مارا حرف بزن");
}
