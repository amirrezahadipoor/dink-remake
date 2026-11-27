//معرفی و اسپاون باس (جدا از اسکریپت صفحه — الگوی امن)
void main( void )
{
int &t;
preload_seq(641);
preload_seq(643);
preload_seq(647);
preload_seq(649);
preload_seq(632);
preload_seq(634);
preload_seq(636);
preload_seq(638);
wait(400);
cinema(1);
say_xy("`4هیکل غول‌پیکری از مه بیرون می‌آید...", 140, 60);
wait(2200);
say_xy("`4گرگ قراردادی: قرارداد می‌گه جعبه. درباره‌ی جنازه‌ها چیزی ننوشته.", 40, 60);
wait(2600);
cinema(0);
&t = create_sprite(480, 250, 9, 641, 1);
sp_script(&t, "s2wolf");
debug("CH2_WOLF_SPAWNED");
kill_this_task();
}
