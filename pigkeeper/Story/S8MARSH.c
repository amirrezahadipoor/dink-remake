//فصل ۸ — مرداب خاموش: نه پرنده، نه باد
void main( void )
{
debug("CH8_MARSH_OK");
int &st;
&st = flag_get("ch7_reveal");
if (&st == 0)
{
 kill_this_task();
}
scene_tint(185, 200, 200);
&st = flag_get("ch8_marsh_seen");
if (&st == 1)
{
 kill_this_task();
}
flag_set("ch8_marsh_seen", 1);
spawn("s8mc");
kill_this_task();
}
