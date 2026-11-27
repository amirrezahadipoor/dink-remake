//ایستگاه میخانه — سلما در تصویر پایانی
void main( void )
{
&player_map = 62;
sp_x(1, 320);
sp_y(1, 330);
load_screen();
draw_screen();
debug("CI_TELEPORT_TAVERN");
int &dstop;
&dstop = demo_stop();
if (&dstop == 3)
{
 kill_this_task();
}
spawn("citele4");
kill_this_task();
}
