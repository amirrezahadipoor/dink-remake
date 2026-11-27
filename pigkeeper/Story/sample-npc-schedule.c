//الگوی مرجع «برنامه روزانه NPC» (فاز ۲-۱۰) — نمونه: بانو وردا
//این الگو در فاز ۵ برای همه NPC های پاین‌هالو استفاده می‌شود.
//اتصال به اسپرایت: sp_script(&npc, "sample-npc-schedule");

void main( void )
{
int &h;
scheduleloop:
 &h = get_hour();
 //صبح (۵ تا ۱۲): کنار میدان روستا
 if (&h >= 5)
 {
  if (&h < 12)
  {
   sp_x(&current_sprite, 420);
   sp_y(&current_sprite, 200);
  }
 }
 //بعدازظهر (۱۲ تا ۲۰): جلوی خانه‌اش
 if (&h >= 12)
 {
  if (&h < 20)
  {
   sp_x(&current_sprite, 180);
   sp_y(&current_sprite, 310);
  }
 }
 //شب: داخل خانه (مخفی)
 &h = is_night();
 if (&h == 1)
 {
  sp_nodraw(&current_sprite, 1);
  sp_nohit(&current_sprite, 1);
 }
 if (&h == 0)
 {
  sp_nodraw(&current_sprite, 0);
  sp_nohit(&current_sprite, 0);
 }
 wait(2000);
 goto scheduleloop;
}

void talk( void )
{
int &h;
 &h = get_hour();
 if (&h < 12)
 {
  say_xy("`%صبح بخیر. کاری داری، بچه؟", 120, 140);
 }
 if (&h >= 12)
 {
  say_xy("`%بعدازظهرها حرف نمی‌زنم. مگه اینکه مهم باشه.", 90, 140);
 }
}
