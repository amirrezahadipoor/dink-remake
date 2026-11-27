void main( void )
{
flag_set("ch9_plan", 1);
&player_map = 25;
sp_x(1, 150);
sp_y(1, 330);
load_screen();
draw_screen();
debug("CI_TELEPORT_YARD");
spawn("citele11");
kill_this_task();
}
