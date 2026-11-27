//باس: دودِ یادها (B1: hp45 dmg7 def2 exp60)
void main( void )
{
sp_brain(&current_sprite, 9);
sp_speed(&current_sprite, 1);
sp_timing(&current_sprite, 33);
sp_base_walk(&current_sprite, 530);
sp_hitpoints(&current_sprite, 45);
sp_defense(&current_sprite, 2);
sp_strength(&current_sprite, 7);
sp_touch_damage(&current_sprite, 7);
sp_gold(&current_sprite, 40);
sp_exp(&current_sprite, 60);
sp_target(&current_sprite, 1);
debug("CH10_SMOKE_READY");
}
void hit( void )
{
sp_target(&current_sprite, &enemy_sprite);
screen_shake(200, 4);
playsound(31, 15050, 4000, &current_sprite, 0);
}
void die( void )
{
flag_set("ch10_smoke_down", 1);
journal_add("بالای سکو: وَیل و دفتر");
achieve("دودِ یادها");
debug("CH10_SMOKE_DOWN");
screen_shake(700, 6);
scene_tint(255, 255, 255);
say_xy("`%راف وسط دود فریاد زد: «منو یادته؟ من دستور دادم! بیا سراغ خودم!»", 50, 60);
wait(3000);
say_xy("`%و از آن‌سو کامل بیرون آمد. با کل خاطره‌اش. «حالا می‌تونیم حرف بزنیم.»", 50, 60);
}
