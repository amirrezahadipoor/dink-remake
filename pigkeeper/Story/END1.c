void click( void )
{
int &e;
&e = flag_get("ending");
if (&e > 0)
{
 kill_this_task();
}
flag_set("ending", 1);
clear_buttons();
achieve("آتش‌به‌آتش");
debug("ENDING_1");
spawn("end1c");
}
