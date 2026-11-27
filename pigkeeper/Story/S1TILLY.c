//تیلی، خوک دینک — فصل ۱

void main( void )
{
}

void talk( void )
{
int &st;
&st = flag_get("ch1_tilly_caught");
if (&st == 1)
{
 say("`%خرخر... (تیلی حالا آرومه)", &current_sprite);
 kill_this_task();
}
flag_set("ch1_tilly_caught", 1);
journal_done("پیدا کردن تیلی");
achieve("اولین دوست");
debug("CH1_TILLY_OK");
say("`%دینک: گرفتمت تیلی! دیگه فرار نکن.", 1);
wait(1200);
say("`$گوبر: ایول! بریم میدون روستا، کارت دارم.", 1);
sp_speed(&current_sprite, 0);
sp_brain(&current_sprite, 6);
kill_this_task();
}

void hit( void )
{
say("`4خرررر! (تیلی ناراحت شد)", &current_sprite);
}
