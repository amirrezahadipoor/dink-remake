//فصل ۱ — صحنه ۲: میدان روستای پاین‌هالو (اسکریپت صفحه)

void main( void )
{
int &st;
debug("CH1_SQUARE_OK");
&st = flag_get("ch1_square_seen");
if (&st == 0)
{
 flag_set("ch1_square_seen", 1);
 wait(500);
 say_xy("`%میدان پاین‌هالو — اینجا همه‌چیز شروع می‌شود", 90, 40);
 wait(2200);
 int &h;
 &h = get_hour();
 if (&h < 12)
 {
  say_xy("`7وردا (از دور): باز که خوک‌هات ول شدن، بچه!", 110, 70);
 }
 if (&h >= 12)
 {
  say_xy("`7وردا (از دور): بعدازظهرها مزاحمم نشو، بچه.", 110, 70);
 }
}
kill_this_task();
}
