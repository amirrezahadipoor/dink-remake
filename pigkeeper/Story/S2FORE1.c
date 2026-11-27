//فصل ۲ — جنگل غرب ۱: اولین مبارزه واقعی
void main( void )
{
int &t;
int &st;
debug("CH2_FOREST_OK");
preload_seq(32);
//قاب جنگل
&t = create_sprite(90, 140, 0, 32, 2);
&t = create_sprite(540, 130, 0, 32, 4);
&t = create_sprite(150, 390, 0, 32, 6);
&t = create_sprite(500, 380, 0, 32, 1);
&t = create_sprite(320, 110, 0, 32, 5);

&st = flag_get("ch2_f1_clear");
if (&st == 1)
{
 kill_this_task();
}
&st = flag_get("ch1_letter");
if (&st == 0)
{
 //داستان هنوز اینجا نرسیده — فقط جنگل خلوت
 kill_this_task();
}
int &wd;
&wd = flag_get("ch2_wolf_down");
if (&wd == 0)
{
 //کمین فعال تا وقتی سرکرده (گرگ) زنده است — بعدش جنگل خلوت می‌شود
 preload_seq(531);
 preload_seq(533);
 preload_seq(537);
 preload_seq(539);
 &t = create_sprite(520, 200, 9, 531, 1);
 sp_script(&t, "s2merc");
 &t = create_sprite(480, 330, 9, 531, 1);
 sp_script(&t, "s2merc");
 debug("CH2_MERCS_SPAWNED");
 int &seen;
 &seen = flag_get("ch2_ambush_seen");
 if (&seen == 0)
 {
  flag_set("ch2_ambush_seen", 1);
  wait(400);
  say_xy("`4صدایی از پشت درخت‌ها: اون بچه‌هه‌ست! جعبه دستشه!", 70, 40);
 }
}
//S3: نامه‌رسان ترسو، پشت درخت قایم شده
preload_seq(231);
&t = create_sprite(90, 250, 16, 231, 7);
sp_speed(&t, 0);
sp_script(&t, "q3mail");
kill_this_task();
}
