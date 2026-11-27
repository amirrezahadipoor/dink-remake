void main( void )
{
flag_set("ch8_defeat", 1);
&player_map = 26;
sp_x(1, 200);
sp_y(1, 330);
load_screen();
draw_screen();
debug("CI_TELEPORT_CAMP");
int &dstop;
&dstop = demo_stop();
if (&dstop == 9)
{
 kill_this_task();
}
spawn("citele10");
kill_this_task();
}
