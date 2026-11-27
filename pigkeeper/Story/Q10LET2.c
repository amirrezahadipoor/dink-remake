//نامه 2 — برداشتنی یک‌باره (فیکس باگ ری‌اسپاون)
void touch( void )
{
int &g;
flag_set("q10_l2", 1);
&g = flag_get("q10_letters");
&g += 1;
flag_set("q10_letters", &g);
playsound(10, 22050, 0, 0, 0);
if (&g >= 3)
{
 say_xy("`%سه نامه‌ی بیست‌ساله. ببرشان پیش پستچی.", 120, 60);
}
sp_active(&current_sprite, 0);
kill_this_task();
}
