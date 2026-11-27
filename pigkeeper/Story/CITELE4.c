void main( void )
{
flag_set("ch5_selma_met", 1);
&player_map = 93;
sp_x(1, 160);
sp_y(1, 240);
load_screen();
draw_screen();
debug("CI_TELEPORT_MINE");
spawn("citele5");
kill_this_task();
}
