//فصل ۶ — معدن نمک ۱: تاریکی و ساس‌های معدن
void main( void )
{
int &t;
debug("CH6_MINE1_OK");
preload_seq(131);
preload_seq(133);
preload_seq(141);
preload_seq(143);
int &st;
&st = flag_get("ch6_m1_clear");
if (&st == 1)
{
 kill_this_task();
}
&t = create_sprite(430, 200, 9, 131, 1);
sp_script(&t, "s6pill");
&t = create_sprite(380, 330, 9, 131, 1);
sp_script(&t, "s6pill");
debug("CH6_PILLS_SPAWNED");
spawn("s6m1c");
kill_this_task();
}
