//ساعت‌سازی مایلو — بسته (فصل ۵→۶)
void main( void )
{
debug("CH5_SHOP_OK");
int &st;
&st = flag_get("ch5_selma_met");
if (&st == 0)
{
 spawn("s5shopa");
 kill_this_task();
}
spawn("s5shopb");
kill_this_task();
}
