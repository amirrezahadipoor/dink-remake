//M4 — سفر سریع محدود: فقط مقصدهای بازشده با پیشروی داستان (نقشه ناحیه = پاداش S3)
void main( void )
{
int &m;
int &q3;
int &c5;
int &c8;
int &c9;
&q3 = flag_get("q3_done");
if (&q3 == 0)
{
 say_xy("`7نقشه ناحیه را نداری. نامه‌رسانِ جنگل غرب کمک لازم دارد...", 10, 200);
 wait(2200);
 kill_this_task();
}
&c5 = flag_get("ch5_selma_met");
&c8 = flag_get("ch8_defeat");
&c9 = flag_get("ch9_plan");
freeze(1);
        choice_start();
        set_y 240
        title_start();
نقشه ناحیه — کجا برویم؟
        title_end();
        "پاین‌هالو (میدان ده)"
        "جنگل غرب"
        "بندر سالت‌مور"
        "دهکده گریون"
        "کمپ شب آخر"
        "بی‌خیال"
        choice_end();
&m = 0;
if (&result == 1)
{
 &m = 35;
}
if (&result == 2)
{
 &m = 33;
}
if (&result == 3)
{
 if (&c5 == 1)
 {
  &m = 30;
 }
}
if (&result == 4)
{
 if (&c8 == 1)
 {
  &m = 27;
 }
}
if (&result == 5)
{
 if (&c9 == 1)
 {
  &m = 26;
 }
}
unfreeze(1);
if (&m == 0)
{
 say_xy("`7هنوز راهش را بلد نیستی.", 10, 200);
 wait(1500);
 kill_this_task();
}
&player_map = &m;
sp_x(1, 320);
sp_y(1, 330);
load_screen();
draw_screen();
playsound(10, 22050, 0, 0, 0);
debug("FASTTRAVEL_OK");
kill_this_task();
}
