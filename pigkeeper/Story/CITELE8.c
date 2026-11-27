void main( void )
{
&player_map = 59;
sp_x(1, 320);
sp_y(1, 330);
load_screen();
draw_screen();
debug("CI_TELEPORT_CHURCH");
spawn("citele9");
kill_this_task();
}
