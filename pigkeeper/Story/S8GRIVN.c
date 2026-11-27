//دهکده گریون — همه «درمان‌شده»
void main( void )
{
int &t;
debug("CH8_GRIVN_OK");
int &st;
&st = flag_get("ch7_reveal");
if (&st == 0)
{
 kill_this_task();
}
scene_tint(185, 200, 200);
//اهالی آرام (مِیدن قهوه‌ای، بی‌حرکت، بی‌واکنش)
preload_seq(241);
&t = create_sprite(200, 190, 16, 241, 1);
sp_script(&t, "s8hollow");
&t = create_sprite(420, 260, 16, 241, 3);
sp_script(&t, "s8hollow");
debug("CH8_HOLLOW_FOLK");
kill_this_task();
}
