//فصل ۲ — جنگل غرب ۲: ورود مارا
void main( void )
{
int &t;
int &st;
debug("CH2_MARA_SCREEN");
preload_seq(32);
&t = create_sprite(110, 150, 0, 32, 3);
&t = create_sprite(530, 360, 0, 32, 2);

&st = flag_get("ch2_mara_met");
if (&st == 1)
{
 kill_this_task();
}
&st = flag_get("ch2_f1_clear");
if (&st == 0)
{
 kill_this_task();
}
//مرحله باس: تا گرگ نیفتاده، مارا پیدایش نمی‌شود
&st = flag_get("ch2_wolf_down");
if (&st == 0)
{
 spawn("s2boss");
 kill_this_task();
}
spawn("s2mara");
kill_this_task();
}
