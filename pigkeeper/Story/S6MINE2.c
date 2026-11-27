//فصل ۶ — معدن نمک ۲: مایلو پشت آوار
void main( void )
{
int &t;
debug("CH6_MINE2_OK");
preload_seq(231);
preload_seq(233);
preload_seq(237);
preload_seq(239);
//مایلو (پیرمرد) کنار دیوار شرقی
&t = create_sprite(470, 240, 16, 231, 1);
sp_base_walk(&t, 230);
sp_speed(&t, 0);
sp_script(&t, "s6milo");
debug("CH6_MILO_HERE");
kill_this_task();
}
