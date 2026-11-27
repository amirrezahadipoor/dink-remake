//صفحه مرگ فارسی — جایگزین DINFO انگلیسی دیتای پایه (نشتی Q3)
void die( void )
{
freeze(1);
&update_status = 0;
sp_seq(1, 436);
sp_base_idle(1, -1);
wait(3000);
sp_nohit(1, 1);
sp_brain(1, 0);
again:
        choice_start();
        set_y 240
        title_start();
خوک‌چران افتاد... ولی قصه هنوز تمام نشده.
        title_end();
"بارگذاری سیو"
"شروع دوباره از اول"
"خروج از بازی"
        choice_end();

if (&result == 1)
{
load();
kill_this_task();
}
if (&result == 2)
{
 sp_nohit(1, 0);
 restart_game();
}
if (&result == 3)
{
 kill_game();
}
}

void load( void )
{
Playsound(18,22050,0,0,0);
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
    goto again;
int &mycrap = game_exist(&result);
if (&mycrap == 0)
    goto again;
    sp_brain(1, 1);
    sp_nohit(1, 0);

    stopmidi();
    script_attach(1000);
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

   load_game(&result);
//loading a game kills ALL tasks
}
