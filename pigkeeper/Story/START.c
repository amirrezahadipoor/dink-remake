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

//عنوان و منوی نمایشی فارسی
int &i;
&i = 0;
titleloop:
say_xy("`%آخرین خوک‌چران", 240, 60);
say_xy("`$بازسازی کامل دینک اسمال‌وود — نسخه آزمایشی", 130, 100);
say_xy("`%شروع بازی", 270, 300);
say_xy("`7ادامه از سیو", 262, 330);
say_xy("`4خروج", 290, 360);
wait(1400);
&i += 1;
if (&i < 9)
{
 goto titleloop;
}

//ورود خودکار به بازی (تست CI — در نسخه نهایی با لمس/کلیک)
debug("PIGKEEPER_AUTOSTART");
sp_x(1, 334);
sp_y(1, 161);
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
if (&i < 12)
{
 goto sayloop;
}
kill_this_task();
}
