//فصل ۲ — جنگل غرب ۲: ورود مارا
void main( void )
{
int &t;
int &st;
debug("CH2_MARA_SCREEN");
preload_seq(32);
&t = create_sprite(110, 150, 0, 32, 3);
&t = create_sprite(530, 360, 0, 32, 2);

&st = flag_get("ch2_mara_met");
if (&st == 1)
{
 kill_this_task();
}
&st = flag_get("ch2_f1_clear");
if (&st == 0)
{
 kill_this_task();
}
//مرحله باس: تا گرگ نیفتاده، مارا پیدایش نمی‌شود
&st = flag_get("ch2_wolf_down");
if (&st == 0)
{
 preload_seq(640);
 preload_seq(632);
 preload_seq(634);
 preload_seq(636);
 preload_seq(638);
 wait(400);
 cinema(1);
 say_xy("`4هیکل غول‌پیکری از مه بیرون می‌آید...", 140, 50);
 wait(2200);
 say_xy("`4گرگ قراردادی: قرارداد می‌گه جعبه. درباره‌ی جنازه‌ها چیزی ننوشته.", 40, 50);
 wait(2600);
 cinema(0);
 &t = create_sprite(480, 250, 9, 640, 1);
 sp_script(&t, "s2wolf");
 debug("CH2_WOLF_SPAWNED");
 kill_this_task();
}
wait(500);
cinema(1);
say_xy("`%زنی با کوله گیاه، خم شده روی رد پاها...", 110, 50);
wait(2400);
say_xy("`%مارا: وایستا همون‌جا. این رد مزدورهاست. تو کی‌ای؟", 80, 50);
wait(2800);
say_xy("`$دینک: دینک. خوک‌چران. اون‌ها دنبال جعبه‌ی مادرم بودن.", 70, 50);
wait(2800);
say_xy("`%مارا: جعبه؟ ...نشونم بده. (مکث) این مُهر رو من می‌شناسم، خوک‌چران.", 50, 50);
wait(3000);
say_xy("`%مارا: اسمش فرقه‌ی خاکستره. و اگه دنبالت باشن، تنها نمی‌رسی سالت‌مور.", 45, 50);
wait(3000);
flag_set("ch2_mara_met", 1);
journal_done("از جنگل غرب عبور کن");
journal_add("همراه مارا به سالت‌مور برو");
achieve("شفاگر فراری");
debug("CH2_MARA_MET");
cinema(0);
say_xy("`%— پایان دموی فصل ۲ —", 200, 230);
kill_this_task();
}
