//لول‌آپ فارسی — جایگزین LRAISE انگلیسی دیتای پایه (منحنی موتور: 100×L×L)
void raise( void )
{
playsound(33, 22000, 0, 0,0);
script_attach(1000);
Playsound(10,22050,0,0,0);
wait(1000);
if (&level < 16)
  {
stop_entire_game(1);
   &level += 1;
        choice_start();
        set_y 240
        title_start();
به سطح &level رسیدی!

یکی را قوی‌تر کن:
        title_end();
        "قدرت ضربه (+۱)"
        "دفاع (+۱)"
        "جادو (+۱)"
        choice_end();

  if (&result == 1)
  {
   &strength += 1;
  }
  if (&result == 2)
  {
   &defense += 1;
  }
  if (&result == 3)
  {
   &magic += 1;
  }

   &lifemax += 3;
   &life = &lifemax;
  }
else
{
	&exp = 0;
	say("`$دینک: دیگه از این قوی‌تر؟ خوک‌ها ازم می‌ترسن.", 1);
}
draw_status();
kill_this_task();
}
