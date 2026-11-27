void touch( void )
{
int &g;
&g = flag_get("q10_letters");
&g += 1;
flag_set("q10_letters", &g);
playsound(10, 22050, 0, 0, 0);
sp_active(&current_sprite, 0);
if (&g == 3)
{
 say_xy("`%سه نامه‌ی بیست‌ساله. ببرشان پیش پستچی.", 120, 60);
}
kill_this_task();
}
