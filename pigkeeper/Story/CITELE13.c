void main( void )
{
&player_map = 66;
sp_x(1, 320);
sp_y(1, 260);
load_screen();
draw_screen();
debug("CI_TELEPORT_SOUTH");
int &dstop;
&dstop = demo_stop();
if (&dstop == 99)
{
 spawn("qaapis");
}
kill_this_task();
}
