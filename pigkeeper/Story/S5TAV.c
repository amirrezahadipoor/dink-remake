//میخانه لنگر کج — سلما (فصل ۵)
void main( void )
{
int &t;
debug("CH5_TAV_OK");
preload_seq(221);
preload_seq(223);
preload_seq(227);
preload_seq(229);
//سلما پشت پیشخون (مِیدن قرمز، ایستاده)
&t = create_sprite(320, 180, 16, 221, 1);
sp_base_walk(&t, 220);
sp_script(&t, "s5selma");
sp_speed(&t, 0);
int &st;
&st = flag_get("ch5_port_seen");
if (&st == 0)
{
 kill_this_task();
}
kill_this_task();
}
