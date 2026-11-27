void main( void )
{
&player_map = 59;
sp_x(1, 320);
sp_y(1, 330);
load_screen();
draw_screen();
debug("CI_TELEPORT_CHURCH");
int &dstop;
&dstop = demo_stop();
if (&dstop == 8)
{
 kill_this_task();
}
spawn("citele9");
kill_this_task();
}
