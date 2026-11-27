void main( void )
{
flag_set("ch6_milo_truth", 1);
&player_map = 29;
sp_x(1, 320);
sp_y(1, 300);
load_screen();
draw_screen();
debug("CI_TELEPORT_DOCK");
int &dstop;
&dstop = demo_stop();
if (&dstop == 6)
{
 kill_this_task();
}
spawn("citele7");
kill_this_task();
}
