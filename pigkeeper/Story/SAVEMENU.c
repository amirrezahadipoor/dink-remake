//M6 — ذخیره دستی: ۸ اسلات (۹=اتوسیو موتور، ۷۷=تنظیمات)
void main( void )
{
freeze(1);
        choice_start();
        set_y 240
        title_start();
کدام اسلات؟ (اسلات ۹ = ذخیره خودکار)
        title_end();
        "&savegameinfo"
        "&savegameinfo"
        "&savegameinfo"
        "&savegameinfo"
        "&savegameinfo"
        "&savegameinfo"
        "&savegameinfo"
        "&savegameinfo"
        "بی‌خیال"
        choice_end();
if (&result > 8)
{
 unfreeze(1);
 kill_this_task();
}
save_game(&result);
playsound(10, 22050, 0, 0, 0);
say_xy("`%ذخیره شد — اسلات &result", 10, 200);
debug("MANUAL_SAVE_OK");
wait(1100);
unfreeze(1);
kill_this_task();
}
