//فصل ۱۰ — حیاط دژ خاکستر: شب یلدا
void main( void )
{
int &t;
debug("CH10_YARD_OK");
int &st;
&st = flag_get("ch9_plan");
if (&st == 0)
{
 kill_this_task();
}
scene_tint(160, 160, 200);
//صف داوطلب‌ها (مِیدن‌ها، بی‌حرکت رو به شمال)
preload_seq(241);
preload_seq(227);
&t = create_sprite(260, 180, 16, 247, 1);
&t = create_sprite(300, 210, 16, 247, 1);
&t = create_sprite(340, 240, 16, 247, 1);
debug("CH10_QUEUE");
&st = flag_get("ch10_infil");
if (&st == 1)
{
 kill_this_task();
}
flag_set("ch10_infil", 1);
achieve("کوره خاطرات");
spawn("s10c");
kill_this_task();
}
