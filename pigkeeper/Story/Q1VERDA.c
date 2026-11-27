//S1 «گل‌های وردا»
void talk( void )
{
int &st;
&st = flag_get("q1_done");
if (&st == 1)
{
 say("`%وردا: گل‌ها قشنگ شدن. تخفیفت سر جاشه، بچه.", &current_sprite);
 kill_this_task();
}
&st = flag_get("q1_active");
if (&st == 1)
{
 &st = flag_get("q1_flowers");
 if (&st >= 3)
 {
  flag_set("q1_done", 1);
  journal_done("سه گل برای باغ وردا");
  achieve("گل‌های وردا");
  debug("Q1_DONE");
  say("`%وردا: ...سه تا گل سالم. خب. اعتمادم رو پس گرفتی.", &current_sprite);
  wait(2600);
  say("`%وردا: از این به بعد تو مغازه‌ی ده، نصف‌قیمت می‌خری.", &current_sprite);
  kill_this_task();
 }
 say("`%وردا: سه گل. از جنوب روستا. تیلی رو هم نبر که بخوردشون!", &current_sprite);
 kill_this_task();
}
flag_set("q1_active", 1);
journal_add("سه گل برای باغ وردا");
debug("Q1_START");
say("`%وردا: بچه! خوکِ تو باغ منو شخم زد!", &current_sprite);
wait(2400);
say("`$دینک: تیلی فقط... باغبونی مدرن بلده؟", 1);
wait(2200);
say("`%وردا: سه گل تازه از جنوب روستا بیار تا ببخشمت.", &current_sprite);
}
