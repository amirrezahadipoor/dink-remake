void touch( void )
{
flag_set("q12_gear", 1);
playsound(10, 22050, 0, 0, 0);
say_xy("`%چرخ‌دنده‌ی برنجی ساعت — هنوز سالم!", 150, 60);
sp_active(&current_sprite, 0);
kill_this_task();
}
