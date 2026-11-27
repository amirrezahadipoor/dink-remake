//S8 «کیف بروم»
void talk( void )
{
int &st;
&st = flag_get("q8_done");
if (&st == 1)
{
 say("`%بروم: طبق فرم ۱۲-د، من به شما مدیونم. مکتوب.", &current_sprite);
 kill_this_task();
}
&st = flag_get("q8_bag");
if (&st == 1)
{
 flag_set("q8_done", 1);
 flag_set("ally_broom", 1);
 journal_add("کیفِ مأمور بروم");
 journal_done("کیفِ مأمور بروم");
 achieve("سه نسخه");
 debug("Q8_DONE");
 say("`%بروم: کیفم! فرم‌هام! شما... شما یک شهروند نمونه‌اید!", &current_sprite);
 wait(2600);
 say("`%بروم: این را مکتوب می‌کنم. سه نسخه. یکی برای بایگانی ابدی.", &current_sprite);
 kill_this_task();
}
say("`%بروم: جیب‌برِ بی‌فرهنگ! کیفم را زد! تمام فرم‌های ۴۴-الف داخلش بود!", &current_sprite);
}
