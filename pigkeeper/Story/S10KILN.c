//تالار کوره — باس نهایی پرده: دودِ یادها
void main( void )
{
int &t;
debug("CH10_KILN_OK");
int &st;
&st = flag_get("ch10_infil");
if (&st == 0)
{
 kill_this_task();
}
&st = flag_get("ch10_smoke_down");
if (&st == 1)
{
 spawn("s11gate");
 kill_this_task();
}
scene_tint(235, 170, 140);
preload_seq(531);
preload_seq(533);
preload_seq(537);
preload_seq(539);
//دودِ یادها: بونکای تیره، بزرگ، سمج
&t = create_sprite(430, 230, 9, 531, 1);
sp_script(&t, "s10smoke");
sp_size(&t, 130);
debug("CH10_SMOKE_SPAWNED");
spawn("s10kc");
kill_this_task();
}
