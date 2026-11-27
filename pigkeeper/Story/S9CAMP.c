//فصل ۹ — سه روز: کمپ، نقشه، گفتگوهای شب آخر
void main( void )
{
debug("CH9_CAMP_OK");
int &st;
&st = flag_get("ch8_defeat");
if (&st == 0)
{
 kill_this_task();
}
scene_tint(150, 160, 215);
&st = flag_get("ch9_plan");
if (&st == 1)
{
 kill_this_task();
}
flag_set("ch9_plan", 1);
journal_done("سه روز تا یلدا — متحد جمع کن");
journal_add("شب یلدا: نفوذ به دژ خاکستر");
achieve("سه روز");
debug("CH9_PLAN_SET");
spawn("s9c");
kill_this_task();
}
