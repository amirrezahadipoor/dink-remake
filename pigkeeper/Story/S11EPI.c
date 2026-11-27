//صفحه ۹۰ — اپیلوگ بهار (پایان طلایی به اینجا وارپ می‌کند؛ قبل از آن چمن خالی)
void main( void )
{
int &e;
&e = flag_get("ending3_epilogue");
if (&e == 0)
{
 kill_this_task();
}
scene_tint(255, 235, 200);
preload_seq(66);
preload_seq(221);
preload_seq(231);
preload_seq(441);
int &t;
&t = create_sprite(150, 270, 0, 66, 1);
&t = create_sprite(450, 290, 0, 66, 3);
&t = create_sprite(280, 255, 0, 66, 2);
&t = create_sprite(360, 255, 0, 221, 1);
sp_hard(&t, 1);
&t = create_sprite(230, 270, 0, 231, 1);
sp_hard(&t, 1);
&t = create_sprite(395, 300, 0, 441, 1);
debug("CH11_EPILOGUE_OK");
kill_this_task();
}
