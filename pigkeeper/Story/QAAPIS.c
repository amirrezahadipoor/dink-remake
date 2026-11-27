//QA فقط-CI: رفت‌وبرگشت تنظیمات + سختی + شمارش دستاورد
void main( void )
{
int &v;
setting_set("difficulty", 2);
&v = setting_get("difficulty");
if (&v == 2)
{
 debug("QA_SETTINGS_OK");
}
int &t;
&t = create_sprite(500, 220, 9, 531, 1);
sp_script(&t, "s2merc");
setting_set("difficulty", 1);
&v = achieve_count();
debug("QA_APIS_DONE");
kill_this_task();
}
