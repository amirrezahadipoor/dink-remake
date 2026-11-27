//M11 — فهرست دستاوردهای این اسلات
void main( void )
{
int &n;
int &i;
int &y;
int &t;
freeze(1);
say_frame(1);
&n = achieve_count();
say_xy("`$ـــ دستاوردها (&n) ـــ", 10, 90);
if (&n == 0)
{
 say_xy("`7هنوز دستاوردی باز نشده. قصه تازه اول راهه.", 10, 200);
}
&i = 0;
&y = 130;
listloop:
if (&i < &n)
{
 if (&i < 9)
 {
  &t = achieve_title(&i, 40, &y);
  &y += 30;
 }
 &i += 1;
 goto listloop;
}
wait(4200);
say_frame(0);
unfreeze(1);
kill_this_task();
}
