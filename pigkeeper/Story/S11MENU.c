//نمایش گزینه‌ها (تکرارشونده تا انتخاب)
void main( void )
{
int &e;
menuloop:
&e = flag_get("ending");
if (&e > 0)
{
 kill_this_task();
}
//قاب گفتار باکس say_xy را خراب می‌کند — برای منو خاموش
say_frame(0);
say_xy("`$ـــ انتخاب دینک ـــ", 10, 112);
say_xy("`4آتش‌به‌آتش", 10, 160);
say_xy("`%فراموشی داوطلب", 10, 218);
say_xy("`$یادنگه‌دار", 10, 276);
wait(1600);
goto menuloop;
}
