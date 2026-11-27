void click( void )
{
int &ex;
&ex = save_game_exist(9);
if (&ex == 1)
{
 debug("BTN_CONTINUE_CLICKED");
 load_game(9);
}
if (&ex == 0)
{
 say_xy("`7هنوز سیوی وجود ندارد — اول «شروع بازی»!", 150, 380);
}
}
