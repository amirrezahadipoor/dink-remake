//فصل ۱ — صحنه ۱-۲: خانه دینک و سیلوی (قبل از آتش)

void main( void )
{
int &st;
debug("CH1_HOUSE_OK");
&st = flag_get("ch1_fire");
if (&st == 1)
{
 say_xy("`4خانه... چیزی جز خاکستر نمانده.", 120, 60);
 kill_this_task();
}
&st = flag_get("ch1_promise");
if (&st == 1)
{
 kill_this_task();
}
wait(600);
say_xy("`%سیلوی: دینک، بیا اینجا پسرم.", 150, 50);
wait(2400);
say_xy("`%سیلوی: این جعبه رو می‌بینی؟ اگه یه روز من نبودم...", 90, 50);
wait(2800);
say_xy("`%سیلوی: ...ببرش پیش عموت مایلو تو سالت‌مور. فقط مایلو. قول؟", 60, 50);
wait(3000);
say_xy("`$دینک: خب... قول. ولی تو که جایی نمی‌ری، نه؟", 100, 50);
wait(2400);
flag_set("ch1_promise", 1);
journal_add("قولِ جعبه به مادر");
debug("CH1_PROMISE_OK");
kill_this_task();
}
