void touch( void )
{
flag_set("q5_gear", 1);
playsound(10, 22050, 0, 0, 0);
say_xy("`%چرخ‌دنده‌ی برنجی بزرگ — اندازه‌ی ساعت دیواری!", 120, 60);
sp_active(&current_sprite, 0);
kill_this_task();
}
