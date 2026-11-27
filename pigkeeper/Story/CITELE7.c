void main( void )
{
flag_set("ch7_reveal", 1);
&player_map = 27;
sp_x(1, 320);
sp_y(1, 300);
load_screen();
draw_screen();
debug("CI_TELEPORT_GRIVN");
spawn("citele8");
kill_this_task();
}
