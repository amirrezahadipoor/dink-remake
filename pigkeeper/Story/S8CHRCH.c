//کلیسای غرق — دفتر سیلوی و تله وَیل
void main( void )
{
debug("CH8_CHURCH_OK");
int &st;
&st = flag_get("ch7_reveal");
if (&st == 0)
{
 kill_this_task();
}
&st = flag_get("ch8_defeat");
if (&st == 1)
{
 spawn("s8after");
 kill_this_task();
}
spawn("s8book");
kill_this_task();
}
