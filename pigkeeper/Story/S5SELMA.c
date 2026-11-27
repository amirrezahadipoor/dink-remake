//ناخدا سلما — لحن دریایی، اعتماد سخت
void talk( void )
{
int &st;
//S5: تحویل چرخ‌دنده ساعت دیواری
&st = flag_get("q5_gear");
if (&st == 1)
{
 int &d5;
 &d5 = flag_get("q5_done");
 if (&d5 == 0)
 {
  flag_set("q5_done", 1);
  flag_set("ally_selma", 1);
  journal_add("ساعتِ بی‌عقربه لنگر کج");
  journal_done("ساعتِ بی‌عقربه لنگر کج");
  achieve("ساعت بی‌عقربه");
  debug("Q5_DONE");
  say("`%سلما: ...این چرخ‌دنده. بیست ساله این ساعت ساکته.", &current_sprite);
  wait(2600);
  say("`%(تیک... تاک...) سلما چیزی نمی‌گوید. فقط نوشیدنی‌ات مجانی می‌شود.", &current_sprite);
  kill_this_task();
 }
}
&st = flag_get("ch5_selma_met");
if (&st == 1)
{
 say("`%سلما: مایلو؟ معدن نمک قدیمی. و بچه— لنگرت رو سفت کن.", &current_sprite);
 kill_this_task();
}
flag_set("ch5_selma_met", 1);
journal_add("نشان گره دریایی را دنبال کن");
achieve("لنگر کج");
debug("CH5_SELMA_MET");
say("`%سلما: غریبه‌ها اول می‌گن کی‌ان، بعد می‌پرسن.", &current_sprite);
wait(2400);
say("`$دینک: دینک. پسر سیلوی.", 1);
wait(2200);
say("`%سلما: ...سیلوی. اون زن لنگرش از کوه سنگین‌تر بود. خب، پسرش.", &current_sprite);
wait(2800);
say("`%سلما: مایلو رو فرقه نگرفته، نترس. خودش رو کوک کرد رفت معدن نمک قدیمی.", &current_sprite);
wait(3000);
say("`%سلما: دو هفته پیش یکی اومده بود سراغ «دفترچه». حالا برو.", &current_sprite);
kill_this_task();
}
