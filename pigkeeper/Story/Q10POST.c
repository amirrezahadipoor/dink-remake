//S10 «نامه‌های نرسیده»
void talk( void )
{
int &st;
&st = flag_get("q10_done");
if (&st == 1)
{
 say("`7پستچی: ...رساندن. رساندن خوب است.", &current_sprite);
 kill_this_task();
}
&st = flag_get("q10_letters");
if (&st >= 3)
{
 flag_set("q10_done", 1);
 flag_set("ally_postman", 1);
 journal_add("نامه‌های نرسیده");
 journal_done("نامه‌های نرسیده");
 achieve("نامه‌های نرسیده");
 debug("Q10_DONE");
 say("`7پستچی (آرام): سه نامه... گیرنده: خانم آبر. آقای تِم. دخترِ ماهیگیر.", &current_sprite);
 wait(3000);
 say("`%«مادر، نون اینجا مزه خاک می‌ده. برگشتم اول می‌رم نونوایی.»", &current_sprite);
 wait(3200);
 say("`%چیزی در چشم‌های خالی پستچی تکان می‌خورد. «...می‌رسانم.»", &current_sprite);
 kill_this_task();
}
say("`7پستچی: نامه... نامه‌ها گم شدند. بیست سال. هر روز می‌گردم.", &current_sprite);
}
