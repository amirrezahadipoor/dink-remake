//«آخرین خوک‌چران» — صفحه عنوان نسخه نمونه (فاز ۲)
//این فایل نسخه پایه START.c را در D-Mod بازنویسی می‌کند.
//فعلاً نمایشی است: عنوان فارسی + ورود خودکار به بازی برای تست CI.
//منوی لمسی واقعی در فاز ۳ (M9) و گرافیک منو در فاز ۵ ساخته می‌شود.

void main( void )
{
int &crap;
fill_screen(0);
sp_seq(1, 0);
sp_brain(1, 13);
sp_pseq(1,10);
sp_pframe(1,8);
sp_que(1,20000);
sp_noclip(1, 1);

&crap = create_sprite(320,240, 0, 196, 1);

debug("PIGKEEPER_BOOT_OK");
playmidi("1003.mid");

//۲-۵: منوی لمسی واقعی — نواحی کلیک روی لیبل‌های فارسی
clear_buttons();
button_script("btnstart");
make_button(205, 225, 435, 262, 0);
button_script("btncont");
make_button(205, 262, 435, 294, 0);
button_script("btnquit");
make_button(205, 325, 435, 358, 0);

//صفحه عنوان — تایپوگرافی v2 (x=10 یعنی وسط‌چین کامل صفحه)
int &i;
&i = 0;
titleloop:
say_xy("`$ـــــــــــــــــــــــــــ", 10, 56);
say_xy("`%آخرین خوک‌چران", 10, 86);
say_xy("`$بازسازی فارسی دینک اسمال‌وود", 10, 116);
say_xy("`$ـــــــــــــــــــــــــــ", 10, 142);
say_xy("`%« شروع بازی »", 10, 236);
say_xy("`7ادامه از سیو", 10, 270);
say_xy("`7تنظیمات", 10, 302);
say_xy("`4خروج", 10, 334);
say_xy("`9نسخه آزمایشی ۰٫۱٫۰ — موتور آزاد FreeDink", 10, 420);
wait(1400);
&i += 1;
int &go;
&go = flag_get("title_go");
if (&go == 1)
{
 goto startgame;
}
if (&i < 9)
{
 goto titleloop;
}
startgame:
flag_set("title_go", 0);
clear_buttons();

//ورود خودکار به بازی (تست CI — در نسخه نهایی با لمس/کلیک)
debug("PIGKEEPER_AUTOSTART");
&player_map = 34;
sp_x(1, 320);
sp_y(1, 230);
sp_base_walk(1, 70);
sp_base_attack(1, 100);
set_mode(2);
reset_timer();
sp_dir(1, 4);
sp_brain(1, 1);
sp_que(1, 0);
sp_noclip(1, 0);
add_item("item-fst",438, 1);
&cur_weapon = 1;
arm_weapon();
wait(1500);
debug("PIGKEEPER_INGAME");
&i = 0;
sayloop:
say_xy("`%اینجا پاین‌هالو است، خانه‌ی دینک", 170, 150);
wait(1500);
&i += 1;
if (&i < 6)
{
 goto sayloop;
}
//دموی CI: پرش به جنگل برای تست مبارزه فصل ۲ (بازیکن واقعی پیاده می‌رود)
flag_set("ch1_letter", 1);
&player_map = 33;
sp_x(1, 320);
sp_y(1, 240);
load_screen();
draw_screen();
debug("CI_TELEPORT_FOREST");
kill_this_task();
}
