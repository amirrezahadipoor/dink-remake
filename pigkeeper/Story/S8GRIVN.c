//دهکده گریون — همه «درمان‌شده»
void main( void )
{
int &t;
debug("CH8_GRIVN_OK");
int &st;
&st = flag_get("ch7_reveal");
if (&st == 0)
{
 kill_this_task();
}
scene_tint(185, 200, 200);
//اهالی آرام (مِیدن قهوه‌ای، بی‌حرکت، بی‌واکنش)
preload_seq(241);
&t = create_sprite(200, 190, 16, 241, 1);
sp_script(&t, "s8hollow");
&t = create_sprite(420, 260, 16, 241, 3);
sp_script(&t, "s8hollow");
debug("CH8_HOLLOW_FOLK");
//S10: پستچی خاکسترگرفته + سه نامه برداشتنی
preload_seq(231);
&t = create_sprite(120, 300, 16, 231, 1);
sp_speed(&t, 0);
sp_script(&t, "q10post");
int &q;
&q = flag_get("q10_letters");
if (&q < 3)
{
 preload_seq(66);
 &t = create_sprite(250, 350, 0, 66, 8);
 sp_script(&t, "q10letr");
 sp_touch_damage(&t, -1);
 &t = create_sprite(480, 200, 0, 66, 8);
 sp_script(&t, "q10letr");
 sp_touch_damage(&t, -1);
 &t = create_sprite(360, 310, 0, 66, 8);
 sp_script(&t, "q10letr");
 sp_touch_damage(&t, -1);
}
debug("Q10_SET");
kill_this_task();
}
