//M8 — قلب کوچک: +۶ جان
void touch( void )
{
&life += 6;
if (&life > &lifemax)
{
 &life = &lifemax;
}
playsound(10, 22050, 0, 0, 0);
sp_active(&current_sprite, 0);
kill_this_task();
}
