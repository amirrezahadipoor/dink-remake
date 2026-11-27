//فصل ۱ — صحنه ۱: مزرعه خوک‌ها (اسکریپت صفحه)
//«آخرین خوک‌چران» — اولین صفحه‌ی دنیای خودِ ریمیک

void main( void )
{
int &t;
int &st;
debug("CH1_FARM_OK");

&st = flag_get("ch1_tilly_caught");
if (&st == 1)
{
 //تیلی قبلاً گرفته شده
 kill_this_task();
}

&st = flag_get("ch1_started");
if (&st == 0)
{
 flag_set("ch1_started", 1);
 journal_add("پیدا کردن تیلی");
 wait(700);
 say_xy("`$گوبر: دینک! تیلی باز فرار کرد! تو مزرعه‌ست!", 20, 40);
}

//تیلی — خوک فراری (brain 4 = خوک، seq 43)
&t = create_sprite(470, 240, 4, 43, 1);
sp_base_walk(&t, 40);
sp_speed(&t, 1);
sp_timing(&t, 33);
sp_script(&t, "s1tilly");
kill_this_task();
}
