//S4 «خوک همسایه»
void talk( void )
{
int &st;
&st = flag_get("q4_done");
if (&st == 1)
{
 say("`%خرخر! (خوک همسایه حالا خونه‌ست)", &current_sprite);
 kill_this_task();
}
flag_set("q4_done", 1);
journal_add("خوک همسایه را برگردان");
journal_done("خوک همسایه را برگردان");
achieve("بوکشیدن تیلی");
debug("Q4_DONE");
say("`%(تیلی بو می‌کشد... خرخرِ آشنا! خوک همسایه پیدا شد)", &current_sprite);
wait(2400);
say("`$دینک: بریم خونه، رفیق گِلی.", 1);
}
