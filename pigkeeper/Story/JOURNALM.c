//M3 — صفحه مأموریت‌ها (ژورنال RTL روی قاب)
void main( void )
{
freeze(1);
say_frame(1);
journal_say();
wait(4500);
say_frame(0);
unfreeze(1);
kill_this_task();
}
