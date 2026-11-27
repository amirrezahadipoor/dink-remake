//گل 1 — برداشتنی یک‌باره (فیکس باگ ری‌اسپاون)
void touch( void )
{
int &g;
flag_set("q1_f1", 1);
&g = flag_get("q1_flowers");
&g += 1;
flag_set("q1_flowers", &g);
playsound(10, 22050, 0, 0, 0);
if (&g >= 3)
{
 say("`%سه گل! برگرد پیش وردا.", 1);
}
sp_active(&current_sprite, 0);
kill_this_task();
}
