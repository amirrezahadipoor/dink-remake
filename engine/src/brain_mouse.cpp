#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "brain.h"
#include "live_sprites_manager.h"
#include "input.h"
#include "log.h"
#include "gfx_sprites.h"
#include "dinkc.h"
#include "sfx.h"

void run_through_mouse_list(int h, /*bool*/int special)
{
  rect box;
  int i;

	for (i = 1; i <= last_sprite_created; i++)
	{
		
		if (spr[i].active) if (i != h) if
			((spr[i].touch_damage != 0) )
		{
			
			if (spr[i].touch_damage != -1) if (spr[h].notouch) return;
			rect_copy(&box, &k[getpic(i)].hardbox);
			rect_offset(&box, spr[i].x, spr[i].y);
			
			
			if (inside_box(spr[h].x, spr[h].y, box))
			{	
				
				if ((spr[i].touch_damage == -1) && (spr[i].script != 0))
				{
					log_info("running %d's script..", spr[i].script);
					if (locate(spr[i].script, "CLICK")) run_script(spr[i].script);
				} 
				else
				{
					if (spr[i].touch_damage == -1)
					{
						log_error("Sprites touch damage is set to -1 but there is no script set!");
					} else
					{
						//lets hurt the guy
					}
					
				}
				
				if (special) return;	
				
			}
			
			
		}
		
	}
	
	/* Remake (M5/2-5): rectangular script buttons — no sprite needed.
	   Registered from DinkC via make_button(); hit = pointer inside rect. */
	{
		for (int bi = 0; bi < 8; bi++) {
			if (!remake_btns[bi].active) continue;
			if (spr[h].x >= remake_btns[bi].x1 && spr[h].x <= remake_btns[bi].x2 &&
			    spr[h].y >= remake_btns[bi].y1 && spr[h].y <= remake_btns[bi].y2) {
				int scr = load_script(remake_btns[bi].script, 1000);
				if (scr != 0 && locate(scr, "CLICK")) {
					log_info("BUTTON_CLICK %s", remake_btns[bi].script);
					run_script(scr);
				}
				special = 1;
			}
		}
	}

	if (special) 		SoundPlayEffect(19, 22050, 0, 0,0);
	
}

/* Remake (M5): registered in dinkc_bindings.cpp */
struct RemakeBtn { int x1,y1,x2,y2; char script[14]; int active; };
extern RemakeBtn remake_btns[8];

void mouse_brain(int h)
{
	
	if (spr[h].move_active) 
	{
		process_move(h);
		return;
	}
	
	
	int diag = 0;
	
	if (sjoy.right) diag++;
	if (sjoy.left) diag++;
	if (sjoy.down) diag++;
	if (sjoy.up) diag++;
	
	
    //*********************************PROCESS MOVEMENT							
	
	if (diag == 1)
	{
								
								if (sjoy.right)
								{
									move(h,spr[h].speed,'+','0');
									changedir(6,h,spr[h].base_walk);
								}
								
								
								if (sjoy.left) 
								{
									move(h,spr[h].speed,'-','0');
									changedir(4,h,spr[h].base_walk);
								}
								
								
								if (sjoy.down)
								{
									move(h,spr[h].speed,'0','+');
									changedir(2,h,spr[h].base_walk);
								}
								
								
								if (sjoy.up) 
								{
									move(h,spr[h].speed,'0','-');
									changedir(8,h,spr[h].base_walk);
								}
								
	}
	// ***************** DIAGONAL!!!!
	
	if (diag > 1)
	{
								
								if ( (sjoy.up) && (sjoy.left) ) 
								{
									changedir(7,h,spr[h].base_walk);
									move(h,spr[h].speed - (spr[h].speed / 3),'-','-');
									
								}
								
								if ( (sjoy.down) && (sjoy.left))
								{
									changedir(1,h,spr[h].base_walk);
									move(h,spr[h].speed - (spr[h].speed / 3),'-','+');
									
								}
								
								if ( (sjoy.down) && (sjoy.right))
								{
									changedir(3,h,spr[h].base_walk);
									move(h,spr[h].speed - (spr[h].speed / 3),'+','+');
								}
								
								
								if ( (sjoy.up) && (sjoy.right))
								{
									changedir(9,h,spr[h].base_walk);
									move(h,spr[h].speed - (spr[h].speed / 3),'+','-');
								}
								
	}
	
	
	
	
	if ( (sjoy.button[ACTION_ATTACK] == /*TRUE*/1) | (mouse1) )
	{
		
		log_info("running through mouse list..");
		run_through_mouse_list(h, /*true*/1);
		sjoy.button[ACTION_ATTACK] = /*false*/0;
		mouse1 = /*false*/0;
							 
	}
	
}
