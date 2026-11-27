//منوی توقف فارسی — جایگزین ESCAPE انگلیسی دیتای پایه (نشتی Q3)
void main(void)
{
Playsound(18, 22050, 0,0,0);
freeze(1);
menu:
        choice_start();
        set_y 240
        title_start();
ـــ توقف ـــ
        title_end();
        "ادامه بازی"
        "دفترچه مأموریت‌ها"
        "ذخیره بازی"
        "بارگذاری سیو"
        "سفر سریع (نقشه ناحیه)"
        "دستاوردها"
        "تنظیمات"
        "شروع دوباره از اول"
        "خروج از بازی"
        choice_end();

if (&result == 1)
{
 unfreeze(1);
 kill_this_task();
}

if (&result == 2)
{
 unfreeze(1);
 spawn("journalm");
 kill_this_task();
}
if (&result == 3)
{
 unfreeze(1);
 spawn("savemenu");
 kill_this_task();
}
if (&result == 5)
{
 unfreeze(1);
 spawn("fasttrav");
 kill_this_task();
}
if (&result == 6)
{
 unfreeze(1);
 spawn("achlist");
 kill_this_task();
}
if (&result == 7)
{
 unfreeze(1);
 spawn("settings");
 kill_this_task();
}
if (&result == 4)
{
        choice_start();
        "&savegameinfo"
        "&savegameinfo"
        "&savegameinfo"
        "&savegameinfo"
        "&savegameinfo"
        "&savegameinfo"
        "&savegameinfo"
        "&savegameinfo"
        "&savegameinfo"
        "&savegameinfo"
        "بی‌خیال، برگرد"
        choice_end();
 if (&result == 11)
 {
  goto menu;
 }
 if (game_exist(&result) == 0)
 {
  goto menu;
 }
init("load_sequence_now graphics\dink\walk\ds-w1- 71 43 38 72 -14 -9 14 9");
init("load_sequence_now graphics\dink\walk\ds-w2- 72 43 37 69 -13 -9 13 9");
init("load_sequence_now graphics\dink\walk\ds-w3- 73 43 38 72 -14 -9 14 9");
init("load_sequence_now graphics\dink\walk\ds-w4- 74 43 38 72 -12 -9 12 9");
init("load_sequence_now graphics\dink\walk\ds-w6- 76 43 38 72 -13 -9 13 9");
init("load_sequence_now graphics\dink\walk\ds-w7- 77 43 38 72 -12 -10 12 10");
init("load_sequence_now graphics\dink\walk\ds-w8- 78 43 37 69 -13 -9 13 9");
init("load_sequence_now graphics\dink\walk\ds-w9- 79 43 38 72 -14 -9 14 9");
init("load_sequence_now graphics\dink\idle\ds-i2- 12 250 33 70 -12 -9 12 9");
init("load_sequence_now graphics\dink\idle\ds-i4- 14 250 30 71 -11 -9 11 9");
init("load_sequence_now graphics\dink\idle\ds-i6- 16 250 36 70 -11 -9 11 9");
init("load_sequence_now graphics\dink\idle\ds-i8- 18 250 32 68 -12 -9 12 9");
init("load_sequence_now graphics\dink\hit\normal\ds-h2- 102 75 60 72 -19 -9 19 9");
init("load_sequence_now graphics\dink\hit\normal\ds-h4- 104 75 61 73 -19 -10 19 10");
init("load_sequence_now graphics\dink\hit\normal\ds-h6- 106 75 58 71 -18 -10 18 10");
init("load_sequence_now graphics\dink\hit\normal\ds-h8- 108 75 61 71 -19 -10 19 10");

 unfreeze(1);
 load_game(&result);
 &update_status = 1;
 draw_status();
 kill_this_task();
}

if (&result == 8)
{
        choice_start();
        "آره، از اول شروع کن"
        "نه، برگرد به بازی"
        choice_end();
 if (&result == 1)
 {
  restart_game();
 }
 unfreeze(1);
 kill_this_task();
}

if (&result == 9)
{
        choice_start();
        "آره، خارج شو"
        "نه، برگرد به بازی"
        choice_end();
 if (&result == 1)
 {
  kill_game();
 }
 unfreeze(1);
 kill_this_task();
}

unfreeze(1);
kill_this_task();
}
