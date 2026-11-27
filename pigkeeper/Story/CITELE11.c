void main( void )
{
flag_set("ch10_infil", 1);
&player_map = 57;
sp_x(1, 180);
sp_y(1, 280);
load_screen();
draw_screen();
debug("CI_TELEPORT_KILN");
kill_this_task();
}
