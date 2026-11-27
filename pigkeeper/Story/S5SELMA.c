//ناخدا سلما — لحن دریایی، اعتماد سخت
void talk( void )
{
int &st;
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
