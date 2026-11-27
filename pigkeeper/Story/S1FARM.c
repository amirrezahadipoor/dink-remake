//فصل ۱ — صحنه ۱: مزرعه خوک‌ها (اسکریپت صفحه)
//«آخرین خوک‌چران» — اولین صفحه‌ی دنیای خودِ ریمیک

void main( void )
{
int &t;
int &st;
debug("CH1_FARM_OK");

&st = flag_get("ch1_fire");
if (&st == 1)
{
 kill_this_task();
}
//آتش‌سوزی: وقتی هم تیلی گرفته شده هم قول داده شده
&st = flag_get("ch1_tilly_caught");
if (&st == 1)
{
 int &pr;
 &pr = flag_get("ch1_promise");
 if (&pr == 1)
 {
  wait(900);
  debug("CH1_FIRE_START");
  screen_shake(900, 7);
  say_xy("`4بووووم! صدای مهیبی از سمت خانه آمد!", 80, 60);
  wait(1800);
  screen_shake(600, 4);
  say_xy("`4دینک: نه... خونه! مامان!!", 150, 60);
  wait(1800);
  flag_set("ch1_fire", 1);
  journal_done("قولِ جعبه به مادر");
  journal_add("بازمانده‌های خانه را بگرد");
  say_xy("`%(فصل ۱ — شبِ بوی دود)", 170, 220);
  kill_this_task();
 }
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

//درخت‌های صحنه (seq 32 — preload لازم برای نمایش)
preload_seq(32);
preload_seq(43);
//🐞 ایشوی باز NOTANIM: تا فیکس رندر، دکور درخت غیرفعال (نشست بعد)
debug("CH1_TREES_PENDING");

//تیلی — خوک فراری (brain 4 = خوک، seq 43)
&t = create_sprite(470, 240, 4, 43, 1);
sp_base_walk(&t, 40);
sp_speed(&t, 1);
sp_timing(&t, 33);
sp_script(&t, "s1tilly");
debug("CH1_TILLY_SPAWNED");
kill_this_task();
}
