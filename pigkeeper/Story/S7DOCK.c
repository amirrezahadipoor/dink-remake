//فصل ۷ — اسکله: افشای راف/رالتون + راز مارا
void main( void )
{
int &t;
debug("CH7_DOCK_OK");
int &st;
&st = flag_get("ch6_milo_truth");
if (&st == 0)
{
 kill_this_task();
}
&st = flag_get("ch7_reveal");
if (&st == 1)
{
 kill_this_task();
}
//راف (پیرمرد دوم) کنار آب
preload_seq(231);
&t = create_sprite(180, 200, 16, 231, 3);
sp_speed(&t, 0);
sp_script(&t, "s7rauf");
debug("CH7_RAUF_HERE");
//S7: فانوس خاموش اسکله (تابلوی تعامل، seq 458)
preload_seq(458);
&t = create_sprite(540, 150, 0, 458, 2);
sp_script(&t, "q7light");
debug("Q7_LIGHT_HERE");
kill_this_task();
}
