//زنجیره دموی CI — ایستگاه بندر (بدون wait: مصون از باگ callback)
void main( void )
{
flag_set("ch2_mara_met", 1);
&player_map = 30;
sp_x(1, 320);
sp_y(1, 300);
load_screen();
draw_screen();
debug("CI_TELEPORT_PORT");
int &dstop;
&dstop = demo_stop();
if (&dstop == 2)
{
 kill_this_task();
}
spawn("citele3");
kill_this_task();
}
