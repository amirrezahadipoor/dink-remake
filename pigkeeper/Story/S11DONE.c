void main( void )
{
scene_tint(255, 255, 255);
say_xy("`%«یادت باشه پسرم — همیشه یادت باشه. حتی چیزهای بد.»", 85, 200);
wait(3000);
say_xy("`$— آخرین خوک‌چران — پایان —", 170, 240);
debug("GAME_COMPLETE");
kill_this_task();
}
