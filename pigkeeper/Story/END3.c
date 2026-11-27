void click( void )
{
int &e;
&e = flag_get("ending");
if (&e > 0)
{
 kill_this_task();
}
flag_set("ending", 3);
clear_buttons();
achieve("یادنگه‌دار");
debug("ENDING_3");
spawn("end3c");
}
