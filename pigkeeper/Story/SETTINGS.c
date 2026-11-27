//M12 — تنظیمات کامل فارسی (ماندگار در settings موتور، گیت‌ها در نقاط مشترک)
void main( void )
{
int &v;
freeze(1);
menu:
        choice_start();
        set_y 240
        title_start();
ـــ تنظیمات ـــ
        title_end();
        "صدا: روشن/خاموش"
        "موسیقی: روشن/خاموش"
        "ویبره: روشن/خاموش"
        "لرزش صفحه: روشن/خاموش"
        "اعداد آسیب: روشن/خاموش"
        "تصویر: نرم/تیز"
        "درجه سختی"
        "بازگشت"
        choice_end();

if (&result == 1)
{
 &v = setting_get("sound");
 if (&v == 1)
 {
  setting_set("sound", 0);
  say_xy("`7صدا: خاموش", 10, 200);
 }
 if (&v == 0)
 {
  setting_set("sound", 1);
  say_xy("`%صدا: روشن", 10, 200);
 }
 wait(700);
 goto menu;
}
if (&result == 2)
{
 &v = setting_get("music");
 if (&v == 1)
 {
  setting_set("music", 0);
  stopmidi();
  say_xy("`7موسیقی: خاموش", 10, 200);
 }
 if (&v == 0)
 {
  setting_set("music", 1);
  say_xy("`%موسیقی: روشن", 10, 200);
 }
 wait(700);
 goto menu;
}
if (&result == 3)
{
 &v = setting_get("vibrate");
 if (&v == 1)
 {
  setting_set("vibrate", 0);
  say_xy("`7ویبره: خاموش", 10, 200);
 }
 if (&v == 0)
 {
  setting_set("vibrate", 1);
  say_xy("`%ویبره: روشن", 10, 200);
 }
 wait(700);
 goto menu;
}
if (&result == 4)
{
 &v = setting_get("shake");
 if (&v == 1)
 {
  setting_set("shake", 0);
  say_xy("`7لرزش صفحه: خاموش", 10, 200);
 }
 if (&v == 0)
 {
  setting_set("shake", 1);
  say_xy("`%لرزش صفحه: روشن", 10, 200);
 }
 wait(700);
 goto menu;
}
if (&result == 5)
{
 &v = setting_get("dmgnum");
 if (&v == 1)
 {
  setting_set("dmgnum", 0);
  say_xy("`7اعداد آسیب: خاموش", 10, 200);
 }
 if (&v == 0)
 {
  setting_set("dmgnum", 1);
  say_xy("`%اعداد آسیب: روشن", 10, 200);
 }
 wait(700);
 goto menu;
}
if (&result == 6)
{
 &v = setting_get("scaler");
 if (&v == 0)
 {
  setting_set("scaler", 1);
  say_xy("`%تصویر: تیز (پیکسل‌آرت)", 10, 200);
 }
 if (&v == 1)
 {
  setting_set("scaler", 0);
  say_xy("`%تصویر: نرم", 10, 200);
 }
 wait(700);
 goto menu;
}
if (&result == 7)
{
        choice_start();
        set_y 240
        title_start();
سختی: دشمن‌ها چقدر جان و زور داشته باشند؟
        title_end();
        "آسان — برای قصه"
        "عادی — تجربه اصلی"
        "سخت — خوک‌چرانِ آهنین"
        "بازگشت"
        choice_end();
 if (&result == 1)
 {
  setting_set("difficulty", 0);
  say_xy("`%سختی: آسان (دشمن‌های جدید)", 10, 200);
  wait(900);
 }
 if (&result == 2)
 {
  setting_set("difficulty", 1);
  say_xy("`%سختی: عادی", 10, 200);
  wait(900);
 }
 if (&result == 3)
 {
  setting_set("difficulty", 2);
  say_xy("`4سختی: سخت!", 10, 200);
  wait(900);
 }
 goto menu;
}
unfreeze(1);
kill_this_task();
}
