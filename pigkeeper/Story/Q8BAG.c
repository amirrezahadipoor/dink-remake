void touch( void )
{
flag_set("q8_bag", 1);
playsound(10, 22050, 0, 0, 0);
say_xy("`%کیف چرمی پر از فرم‌های مهرشده — «مأمور بروم»!", 110, 60);
sp_active(&current_sprite, 0);
kill_this_task();
}
