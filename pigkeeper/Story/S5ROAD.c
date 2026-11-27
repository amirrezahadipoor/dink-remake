//پرده ۲ — جاده ساحلی به سالت‌مور
void main( void )
{
int &t;
debug("CH5_ROAD_OK");
preload_seq(32);
&t = create_sprite(100, 140, 0, 32, 4);
&t = create_sprite(540, 360, 0, 32, 2);
int &st;
&st = flag_get("ch2_mara_met");
if (&st == 0)
{
 kill_this_task();
}
&st = flag_get("ch5_road_seen");
if (&st == 1)
{
 kill_this_task();
}
flag_set("ch5_road_seen", 1);
spawn("s5roadc");
kill_this_task();
}
