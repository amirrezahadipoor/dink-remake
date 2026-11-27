//S7 «فانوس خاموش» — نیازمند سنگ‌نمک درخشان (S9)
void talk( void )
{
int &st;
&st = flag_get("q7_done");
if (&st == 1)
{
 say("`%فانوس می‌درخشد. ماهیگیرها دیگر در مه گم نمی‌شوند.", &current_sprite);
 kill_this_task();
}
&st = flag_get("q9_lightstone");
if (&st == 1)
{
 flag_set("q7_done", 1);
 flag_set("ally_fishers", 1);
 journal_add("فانوس اسکله را روشن کن");
 journal_done("فانوس اسکله را روشن کن");
 achieve("فانوس خاموش");
 debug("Q7_DONE");
 say("`%سنگ‌نمکِ درخشان در جای چراغ می‌نشیند... نور سرد و پایدار.", 1);
 wait(2600);
 say("`%از دور، سوت قایق ماهیگیرها — انگار تشکر می‌کنند.", 1);
 kill_this_task();
}
say("`7فانوس سال‌هاست خاموش است. چراغش روغن نمی‌گیرد... چیز دیگری می‌خواهد.", 1);
}
