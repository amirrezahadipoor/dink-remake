void main( void )
{
&player_map = 94;
sp_x(1, 200);
sp_y(1, 250);
load_screen();
draw_screen();
debug("CI_TELEPORT_MILO");
spawn("citele6");
kill_this_task();
}
