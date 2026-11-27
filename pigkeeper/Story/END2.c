void click( void )
{
int &e;
&e = flag_get("ending");
if (&e > 0)
{
 kill_this_task();
}
flag_set("ending", 2);
clear_buttons();
achieve("فراموشی داوطلب");
debug("ENDING_2");
spawn("end2c");
}
