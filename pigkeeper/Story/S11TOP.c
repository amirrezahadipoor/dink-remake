//فصل ۱۱ — سکوی کوره: وَیل، دفتر، انتخاب
void main( void )
{
debug("CH11_TOP_OK");
int &st;
&st = flag_get("ch10_smoke_down");
if (&st == 0)
{
 kill_this_task();
}
&st = flag_get("ending");
if (&st > 0)
{
 spawn("s11done");
 kill_this_task();
}
scene_tint(235, 180, 150);
spawn("s11vail");
kill_this_task();
}
