//معرفی و اسپاون باس — نسخه مقاوم: اول اسپاون، بعد سینماتیک
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
&t = create_sprite(500, 240, 9, 641, 1);
sp_script(&t, "s2wolf");
debug("CH2_WOLF_SPAWNED");
cinema(1);
say_xy("`4گرگ قراردادی: قرارداد می‌گه جعبه. درباره‌ی جنازه‌ها چیزی ننوشته.", 40, 60);
wait(2600);
cinema(0);
kill_this_task();
}
