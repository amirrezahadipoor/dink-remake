void main( void )
{
flag_set("ch10_smoke_down", 1);
&player_map = 89;
sp_x(1, 320);
sp_y(1, 300);
load_screen();
draw_screen();
debug("CI_TELEPORT_TOP");
kill_this_task();
}
