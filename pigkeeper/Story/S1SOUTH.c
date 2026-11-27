//جنوب روستا — صحنه کوئست‌های S1/S2/S4
void main( void )
{
int &t;
debug("Q_SOUTH_OK");
preload_seq(32);
&t = create_sprite(90, 150, 0, 32, 4);
//سه گل (آیتم‌های برداشتنی — seq 423: گل قرمز از دیتای پایه)
preload_seq(66);
int &g;
&g = flag_get("q1_f1");
if (&g == 0)
{
 &t = create_sprite(180, 200, 0, 66, 2);
 sp_script(&t, "q1flo1");
 sp_touch_damage(&t, -1);
}
&g = flag_get("q1_f2");
if (&g == 0)
{
 &t = create_sprite(320, 300, 0, 66, 4);
 sp_script(&t, "q1flo2");
 sp_touch_damage(&t, -1);
}
&g = flag_get("q1_f3");
if (&g == 0)
{
 &t = create_sprite(390, 270, 0, 66, 6);
 sp_script(&t, "q1flo3");
 sp_touch_damage(&t, -1);
}
debug("Q1_FLOWERS_SET");
//S2: پیر هیزم‌شکن و پسرِ «آرام‌شده»
preload_seq(231);
&t = create_sprite(120, 350, 16, 231, 1);
sp_speed(&t, 0);
sp_script(&t, "q2wood");
//S4: خوک گم‌شده همسایه
preload_seq(43);
&t = create_sprite(560, 380, 4, 43, 1);
sp_base_walk(&t, 40);
sp_script(&t, "q4pig");
debug("Q_SOUTH_NPCS");
kill_this_task();
}
