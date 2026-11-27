//پرده ۲ — بندر سالت‌مور (فصل ۵)
void main( void )
{
int &t;
debug("CH5_PORT_OK");
preload_seq(63);
preload_seq(96);
//خانه‌های بندر
&t = create_sprite(140, 150, 0, 63, 2);
sp_hard(&t, 1);
&t = create_sprite(380, 140, 0, 96, 3);
sp_hard(&t, 1);
&t = create_sprite(320, 120, 0, 63, 5);
sp_hard(&t, 1);
//S6: زن جوان کنار خیمه فرقه
preload_seq(241);
&t = create_sprite(480, 265, 16, 241, 1);
sp_speed(&t, 0);
sp_script(&t, "q6widow");
debug("Q6_WIDOW_HERE");
//S5: چرخ‌دنده دوم، گمشده کنار جرثقیل بندر
int &q5;
&q5 = flag_get("q5_gear");
if (&q5 == 0)
{
 preload_seq(66);
 &t = create_sprite(520, 250, 0, 66, 10);
 sp_script(&t, "q5gear");
 sp_touch_damage(&t, -1);
}
//S8: بروم در بندر (پیگیری کیف)
preload_seq(231);
&t = create_sprite(240, 320, 16, 231, 9);
sp_speed(&t, 0);
sp_script(&t, "q8broom");
//S12: پیرزنِ ساعت شکسته
preload_seq(221);
&t = create_sprite(90, 250, 16, 221, 5);
sp_speed(&t, 0);
sp_script(&t, "q12old");
int &st;
&st = flag_get("ch5_port_seen");
if (&st == 1)
{
 kill_this_task();
}
&st = flag_get("ch2_mara_met");
if (&st == 0)
{
 kill_this_task();
}
flag_set("ch5_port_seen", 1);
journal_done("همراه مارا به سالت‌مور برو");
journal_add("مغازه مایلو را پیدا کن");
achieve("بندر بی‌لنگر");
debug("CH5_PORT_ARRIVED");
spawn("s5portc");
kill_this_task();
}
