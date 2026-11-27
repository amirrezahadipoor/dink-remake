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
&t = create_sprite(470, 140, 0, 96, 3);
sp_hard(&t, 1);
&t = create_sprite(320, 120, 0, 63, 5);
sp_hard(&t, 1);
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
