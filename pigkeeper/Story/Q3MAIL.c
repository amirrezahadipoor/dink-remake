//S3 «نامه‌رسان ترسو» — اسکورت = پاک‌سازی راه
void talk( void )
{
int &st;
&st = flag_get("q3_done");
if (&st == 1)
{
 say("`7نامه‌رسان: راه امنه... ممنونِ تو. نامه‌ها می‌رسن.", &current_sprite);
 kill_this_task();
}
&st = flag_get("ch2_f1_clear");
if (&st == 1)
{
 flag_set("q3_done", 1);
 journal_add("راه نامه‌رسان را باز کن");
 journal_done("راه نامه‌رسان را باز کن");
 achieve("نامه‌رسان ترسو");
 debug("Q3_DONE");
 say("`7نامه‌رسان: اون... اون چیزها رو تو زدی؟ راه بازه؟", &current_sprite);
 wait(2600);
 say("`$دینک: بازه. بدو تا دوباره نیومدن.", 1);
 wait(2200);
 say("`7نامه‌رسان: این نقشه‌ی ناحیه مال تو. من حفظمش کردم!", &current_sprite);
 kill_this_task();
}
say("`7نامه‌رسان (از پشت درخت): ن-نرو اون‌طرف! صداشون رو می‌شنوی؟!", &current_sprite);
}
