//S12 «آخرین مشتری»
void talk( void )
{
int &st;
&st = flag_get("q12_done");
if (&st == 1)
{
 say("`%پیرزن: تیک‌تاک... این صدا رو یادمه. ممنون پسرم.", &current_sprite);
 kill_this_task();
}
&st = flag_get("q12_gear");
if (&st == 1)
{
 flag_set("q12_done", 1);
 journal_add("آخرین مشتری");
 journal_done("آخرین مشتری");
 achieve("آخرین مشتری");
 debug("Q12_DONE");
 say("`%پیرزن: چرخ‌دنده‌ش! هدیه‌ی شوهرم بود... قبل از اینکه «آروم» بشه.", &current_sprite);
 wait(3000);
 say("`%(ساعت راه می‌افتد) «این صدا رو یادمه.»", &current_sprite);
 kill_this_task();
}
say("`%پیرزن: ساعتم خوابیده. مایلو هم نیست. چرخ‌دنده‌ش گم شده...", &current_sprite);
}
