/**
 * FreeDink (not FreeDinkEdit) screen update

 * Copyright (C) 1997, 1998, 1999, 2002, 2003  Seth A. Robinson
 * Copyright (C) 2005, 2006  Dan Walma
 * Copyright (C) 2005, 2007, 2008, 2009, 2010, 2011, 2012, 2014, 2015  Sylvain Beucler

 * This file is part of GNU FreeDink

 * GNU FreeDink is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.

 * GNU FreeDink is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "update_frame.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/* #include <windows.h> */
/* #include <ddraw.h> */
#include "SDL.h"

#include "game_engine.h"
#include "live_sprites_manager.h"
#include "DMod.h"
#include "editor_screen.h"
#include "live_screen.h"
#include <stdlib.h> /* remake: getenv/atoi for autosave */
#include "live_sprite.h"
#include "freedink.h"
#include "brains.h"
#include "gfx.h"
#include "gfx_sprites.h"
#include "gfx_tiles.h"
#include "dinkini.h"
#include "bgm.h"
#include "log.h"
#include "rect.h"
#include "input.h"
#include "sfx.h"
#include "text.h"
#include "game_choice.h"
#include "game_choice_renderer.h"
#include "status.h"

/* For printing strings in process_talk */
#include "gfx_fonts.h"
#include <vector> /* remake: touch overlay TextCommands */

static unsigned long mold;

/* Fills 'struct seth_joy sjoy' with the current keyboard and/or
   joystick state */
/* TODO INPUT: group all input checks here, and switch to events
   processing rather than keystate parsing for 'justpressed' events
   (to avoid having to maintain a fake keyboard state in input.c) */
static void remake_touch_poll();  /* remake (3-7), defined below */
static void remake_touch_apply();

void check_joystick()
{
  remake_touch_poll(); /* remake (3-7): sample finger state once per frame */

  /* Clean-up */
  /* Actions */
  {
    int a = ACTION_FIRST;
    for (a = ACTION_FIRST; a < ACTION_LAST; a++)
      sjoy.joybit[a] = 0;
  }
  
  /* Arrows */
  sjoy.right = 0;
  sjoy.left = 0;
  sjoy.up = 0;
  sjoy.down = 0;

  /* Arrows triggered (not maintained pressed) */
  sjoy.rightd = 0;
  sjoy.leftd = 0;
  sjoy.upd = 0;
  sjoy.downd = 0;
	
  if (joystick)
    {
      SDL_JoystickUpdate(); // required even with joystick events enabled
      Sint16 x_pos = 0, y_pos = 0;
      /* SDL counts buttons from 0, not from 1 */
      int i = 0;
      for (i = 0; i < NB_BUTTONS; i++)
	if (SDL_JoystickGetButton(jinfo, i))
	  sjoy.joybit[input_get_button_action(i)] = 1;

      x_pos = SDL_JoystickGetAxis(jinfo, 0);
      y_pos = SDL_JoystickGetAxis(jinfo, 1);
      /* Using thresold=10% (original game) is just enough to get rid
	 of the noise. Let's use 30% instead, otherwise Dink will go
	 diags too easily. */
      {
	Sint16 threshold = 32767 * 30/100;
	if (x_pos < -threshold) sjoy.left  = 1;
	if (x_pos > +threshold) sjoy.right = 1;
	if (y_pos < -threshold) sjoy.up    = 1;
	if (y_pos > +threshold) sjoy.down  = 1;
      }
    }
  
  if (input_getscancodestate(SDL_SCANCODE_LCTRL) || input_getscancodestate(SDL_SCANCODE_RCTRL)) sjoy.joybit[ACTION_ATTACK] = 1;
  if (input_getscancodestate(SDL_SCANCODE_SPACE)) sjoy.joybit[ACTION_TALK] = 1;
  if (input_getscancodestate(SDL_SCANCODE_LSHIFT) || input_getscancodestate(SDL_SCANCODE_RSHIFT)) sjoy.joybit[ACTION_MAGIC] = 1;
  if (input_getscancodestate(SDL_SCANCODE_RETURN)) sjoy.joybit[ACTION_INVENTORY] = 1;
  if (input_getscancodestate(SDL_SCANCODE_ESCAPE)) sjoy.joybit[ACTION_MENU] = 1;
  if (input_getscancodestate(SDL_SCANCODE_6)) sjoy.joybit[ACTION_MAP] = 1;
  if (input_getcharstate(SDLK_m)) sjoy.joybit[ACTION_MAP] = 1;
  if (input_getscancodestate(SDL_SCANCODE_7)) sjoy.joybit[ACTION_BUTTON7] = 1;

  /* Remake (3-7): merge touch controls into the same action bits */
  remake_touch_apply();
  
  {
    int a = ACTION_FIRST;
    for (a = ACTION_FIRST; a < ACTION_LAST; a++)
      {
	sjoy.button[a] = 0;
	if (sjoy.joybit[a] && sjoy.joybitold[a] == 0)
	  /* Button was just pressed */
	  sjoy.button[a] = 1;
	sjoy.joybitold[a] = sjoy.joybit[a];
      }
  }
  
  if (input_getscancodestate(SDL_SCANCODE_RIGHT) || sjoy.joybit[ACTION_RIGHT]) sjoy.right = 1;
  if (input_getscancodestate(SDL_SCANCODE_LEFT)  || sjoy.joybit[ACTION_LEFT])  sjoy.left  = 1;
  if (input_getscancodestate(SDL_SCANCODE_DOWN)  || sjoy.joybit[ACTION_DOWN])  sjoy.down  = 1;
  if (input_getscancodestate(SDL_SCANCODE_UP)    || sjoy.joybit[ACTION_UP])    sjoy.up    = 1;
  
  if (sjoy.right && sjoy.rightold == 0)
    sjoy.rightd = 1;
  sjoy.rightold = sjoy.right;
	
  if (sjoy.left && sjoy.leftold == 0)
    sjoy.leftd = 1;
  sjoy.leftold = sjoy.left;
  
  if (sjoy.up && sjoy.upold == 0)
    sjoy.upd = 1;
  sjoy.upold = sjoy.up;
	
  if (sjoy.down && sjoy.downold == 0)
    sjoy.downd = 1;
  sjoy.downold = sjoy.down;

  
  /* High speed */
  if (input_getscancodestate(SDL_SCANCODE_TAB) == 1)
    {
      game_set_high_speed();
    }
  else if (input_getscancodestate(SDL_SCANCODE_TAB) == 0)
    {
      game_set_normal_speed();
    }
  

  if (wait4b.active)
    {
      //check for dirs
      
      if (sjoy.rightd) wait4b.button = 16;
      if (sjoy.leftd)  wait4b.button = 14;
      if (sjoy.upd)    wait4b.button = 18;
      if (sjoy.downd)  wait4b.button = 12;
      
      sjoy.rightd = 0;
      sjoy.downd = 0;
      sjoy.upd = 0;
      sjoy.leftd = 0;
      
      //check buttons
      {
	int a = ACTION_FIRST;
	for (a = ACTION_FIRST; a < ACTION_LAST; a++)
	  {
	    if (sjoy.button[a])
	      //button was pressed
	      wait4b.button = a;
	    sjoy.button[a] = /*false*/0;
	  }
      }
      
      if (wait4b.button != 0)
	{
	  *presult = wait4b.button;
	  wait4b.active = /*false*/0;
	  run_script(wait4b.script);
	}
    }
}

/* Remake (2-6): periodic autosave to slot 9. Interval in seconds via
   FREEDINK_AUTOSAVE (default 180, 0 disables). Only while playing. */
extern void save_game(int num);
static void remake_autosave() {
	static int interval = -2;
	static Uint32 last = 0;
	if (interval == -2) {
		const char* e = getenv("FREEDINK_AUTOSAVE");
		interval = (e != NULL) ? atoi(e) : 180;
		last = SDL_GetTicks();
	}
	if (interval <= 0 || mode < 2)
		return;
	/* remake audit #1: never autosave mid-cutscene/dialog (player frozen)
	   — reloading such a save could trap the player */
	if (spr[1].freeze)
		return;
	Uint32 now = SDL_GetTicks();
	if (now - last >= (Uint32)interval * 1000) {
		last = now;
		save_game(9);
		log_info("AUTOSAVE_OK: slot 9");
	}
}

/* Remake (2-9): advance the game clock and apply a simple night
   dimming (needs truecolor; the Android build runs truecolor/GL).
   Day length via FREEDINK_DAYLEN in real seconds (default 1200 = 20min,
   0 disables the whole system). Fades keep priority over night dim. */
static void remake_clock_and_night() {
	static int daylen = -2;
	static Uint32 lastms = 0;
	static int night_applied = 0;
	const double NIGHT_BRIGHT = 170;
	if (daylen == -2) {
		const char* e = getenv("FREEDINK_DAYLEN");
		daylen = (e != NULL) ? atoi(e) : 1200;
		lastms = SDL_GetTicks();
	}
	if (daylen <= 0)
		return;
	Uint32 now = SDL_GetTicks();
	if (mode >= 2) { /* clock only runs while playing */
		remake_clock_min += (now - lastms) * (1440.0 / (daylen * 1000.0));
		while (remake_clock_min >= 1440) remake_clock_min -= 1440;
	}
	lastms = now;
	/* night = 20:00..05:00 */
	int hour = ((int)(remake_clock_min / 60)) % 24;
	int night = (hour >= 20 || hour < 5);
	/* don't fight running fades */
	if (process_downcycle || process_upcycle)
		return;
	if (mode >= 2 && night && g_display->brightness == 256) {
		g_display->brightness = NIGHT_BRIGHT;
		if (!night_applied) log_info("NIGHT_DIM_ON (hour %d)", hour);
		night_applied = 1;
	} else if ((!night || mode < 2) && night_applied && g_display->brightness == NIGHT_BRIGHT) {
		g_display->brightness = 256;
		log_info("NIGHT_DIM_OFF (hour %d)", hour);
		night_applied = 0;
	}
}

/* ------------------------------------------------------------------ */
/* Remake (3-7): touch controls v1 — virtual joystick (left zone) +   */
/* action buttons (right cluster). Polled from SDL finger state, so   */
/* no event plumbing; auto-enabled when a touch device exists.        */
/* ------------------------------------------------------------------ */
static struct RTouch {
	int checked, on;
	int r, l, u, d, atk, talk, mag, inv, menu;
	int jdown; float ox, oy, jx, jy;
	int lastmask;
} g_touch;

static int touch_hit(float x, float y, float cx, float cy, float rad) {
	float dx = (x - cx), dy = (y - cy) * 0.75f; /* 4:3 aspect comp */
	return dx*dx + dy*dy <= rad*rad;
}

static void remake_touch_poll() {
	if (!g_touch.checked) {
		const char* e = getenv("FREEDINK_TOUCH");
		g_touch.on = (SDL_GetNumTouchDevices() > 0) ||
		             (e != NULL && strcmp(e, "1") == 0);
		g_touch.checked = 1;
		if (g_touch.on) log_info("TOUCH_CONTROLS_ON");
	}
	if (!g_touch.on) return;
	g_touch.r = g_touch.l = g_touch.u = g_touch.d = 0;
	g_touch.atk = g_touch.talk = g_touch.mag = g_touch.inv = g_touch.menu = 0;
	if (mode < 2) { g_touch.jdown = 0; return; } /* title: taps stay mouse */
	if (SDL_GetNumTouchDevices() <= 0) return;
	SDL_TouchID dev = SDL_GetTouchDevice(0);
	int n = SDL_GetNumTouchFingers(dev);
	int saw_joy = 0;
	for (int i = 0; i < n; i++) {
		SDL_Finger* f = SDL_GetTouchFinger(dev, i);
		if (f == NULL) continue;
		float x = f->x, y = f->y;
		if (x < 0.45f) { /* movement zone */
			if (!g_touch.jdown) { g_touch.jdown = 1; g_touch.ox = x; g_touch.oy = y; }
			saw_joy = 1; g_touch.jx = x; g_touch.jy = y;
			float dx = x - g_touch.ox, dy = y - g_touch.oy;
			const float dead = 0.025f;
			if (dx >  dead) g_touch.r = 1;
			if (dx < -dead) g_touch.l = 1;
			if (dy >  dead) g_touch.d = 1;
			if (dy < -dead) g_touch.u = 1;
		} else {
			if (touch_hit(x, y, 0.875f, 0.78f, 0.100f)) g_touch.atk  = 1;
			if (touch_hit(x, y, 0.720f, 0.88f, 0.080f)) g_touch.mag  = 1;
			if (touch_hit(x, y, 0.720f, 0.60f, 0.080f)) g_touch.talk = 1;
			if (touch_hit(x, y, 0.575f, 0.74f, 0.070f)) g_touch.inv  = 1;
			if (touch_hit(x, y, 0.955f, 0.06f, 0.060f)) g_touch.menu = 1;
		}
	}
	if (!saw_joy) g_touch.jdown = 0;
	int mask = g_touch.r | g_touch.l<<1 | g_touch.u<<2 | g_touch.d<<3 |
	           g_touch.atk<<4 | g_touch.talk<<5 | g_touch.mag<<6 |
	           g_touch.inv<<7 | g_touch.menu<<8;
	if (mask != g_touch.lastmask) {
		log_info("TOUCH_STATE mv[r%d l%d u%d d%d] atk%d talk%d mag%d inv%d menu%d",
		         g_touch.r, g_touch.l, g_touch.u, g_touch.d,
		         g_touch.atk, g_touch.talk, g_touch.mag, g_touch.inv, g_touch.menu);
		g_touch.lastmask = mask;
	}
}

static void remake_touch_apply() {
	if (!g_touch.on || mode < 2) return;
	if (g_touch.r)    sjoy.joybit[ACTION_RIGHT] = 1;
	if (g_touch.l)    sjoy.joybit[ACTION_LEFT] = 1;
	if (g_touch.u)    sjoy.joybit[ACTION_UP] = 1;
	if (g_touch.d)    sjoy.joybit[ACTION_DOWN] = 1;
	if (g_touch.atk)  sjoy.joybit[ACTION_ATTACK] = 1;
	if (g_touch.talk) sjoy.joybit[ACTION_TALK] = 1;
	if (g_touch.mag)  sjoy.joybit[ACTION_MAGIC] = 1;
	if (g_touch.inv)  sjoy.joybit[ACTION_INVENTORY] = 1;
	if (g_touch.menu) sjoy.joybit[ACTION_MENU] = 1;
}

/* HUD labels for the touch zones, drawn right before flip (Persian,
   dialog font => shaped + anti-aliased). Called from AppFreeDink. */
void remake_touch_overlay_render() {
	if (!g_touch.on || mode < 2)
		return;
	struct { const char* txt; int x; int y; } L[] = {
		{"حمله", 560, 362}, {"جادو", 461, 410}, {"گفتگو", 461, 276},
		{"آیتم", 368, 343}, {"منو", 611, 17},
	};
	FONTS_SetTextColor(235, 235, 235);
	for (unsigned i = 0; i < sizeof(L)/sizeof(L[0]); i++) {
		std::vector<TextCommand> cmds;
		rect r = {L[i].x - 70, L[i].y, L[i].x + 70, L[i].y + 30};
		print_text_wrap_getcmds((char*)L[i].txt, &r, 1, 0, FONT_DIALOG, &cmds);
		print_text_cmds(&cmds);
	}
	if (g_touch.jdown) {
		std::vector<TextCommand> cmds;
		int jx = (int)(g_touch.jx * 640), jy = (int)(g_touch.jy * 480);
		rect r = {jx - 20, jy - 10, jx + 20, jy + 20};
		print_text_wrap_getcmds((char*)"+", &r, 1, 0, FONT_DIALOG, &cmds);
		print_text_cmds(&cmds);
	}
}

void updateFrame() {
	check_joystick();
	remake_autosave();
	remake_clock_and_night();
	
	int move_result ;
    
	int max_s;
	int rank[MAX_SPRITES_AT_ONCE];
	
	abort_this_flip = /*false*/0;
	
	
	/* This run prepares a screen transition (when Dink runs to the border) */
	bool get_frame = false;
	
	/* Screen transition preparation start point */
 trigger_start:
	
	game_compute_speed();

	if (showb.active) {
		process_show_bmp();
		return;
	}

	// Things to do every 1/10th second
	if (thisTickCount > mold+100) {
		mold = thisTickCount;
		
		if (bow.active) bow.hitme = /*true*/1;
		
		if (*pupdate_status == 1) update_status_all();
		
		update_sound();
		
		process_animated_tiles(g_dmod.bgTilesets.slots, cur_ed_screen.t, thisTickCount);
	}
	
	if (show_inventory) {
		process_item();
		return;
	}
	
	if (transition_in_progress) {
		transition(fps_final);
		return;
	}
	
	
	/* Fade to black, etc. */
	if (process_upcycle) up_cycle();
	if (process_warp > 0) process_warp_man();
	if (process_downcycle) CyclePalette();
	
	
	max_s = last_sprite_created;
	screen_rank_game_sprites(rank);
	
	//Blit from background, which holds the base scene.
	IOGFX_backbuffer->blit(IOGFX_background, NULL, NULL);
	
	
	if (stop_entire_game == 1) {
		if (game_choice.active) {
			game_choice_logic();
			game_choice_renderer_render();
		} else {
			stop_entire_game = 0;
			
			draw_screen_game_background();
			draw_status_all();
		}
		return;
	}
	

	/* Update all active sprites */
	for (int j = 0; j <= max_s; j++) {
		int h = rank[j];
		
		if (!(h > 0 && spr[h].active && spr[h].disabled == 0))
			continue;
		
		spr[h].moveman = 0; //init thing that keeps track of moving path	
		spr[h].lpx[0] = spr[h].x;
		spr[h].lpy[0] = spr[h].y; //last known legal cords
		
		//inc delay, used by "skip" by all sprites
		spr[h].skiptimer++;
		
		live_sprite_set_kill_start(h, thisTickCount);
		if (live_sprite_is_expired(h, thisTickCount)) {
			lsm_remove_sprite(h);
			get_last_sprite();
			if (spr[h].say_stop_callback > 0)
				run_script(spr[h].say_stop_callback);
		}
		
		if (spr[h].timing > 0) {
			if (thisTickCount > spr[h].wait)
				spr[h].wait = thisTickCount + spr[h].timing;
			else
				goto animate;
		}
		
		if (spr[h].notouch && thisTickCount > spr[h].notouch_timer)
			spr[h].notouch = /*false*/0;

		if (get_frame)
			goto past;

		//brains - predefined bahavior patterns available to any sprite
		if (spr[h].brain == 1) {
			run_through_touch_damage_list(h);
			if (process_warp == 0)
				human_brain(h);
		}
		if (spr[h].brain == 2) bounce_brain(h);
		if (spr[h].brain == 0) no_brain(h);
		if (spr[h].brain == 3) duck_brain(h);
		if (spr[h].brain == 4) pig_brain(h);
		if (spr[h].brain == 5) one_time_brain(h);
		if (spr[h].brain == 6) repeat_brain(h);
		if (spr[h].brain == 7) one_time_brain_for_real(h);
		if (spr[h].brain == 8) text_brain(h);
		if (spr[h].brain == 9) pill_brain(h);
		if (spr[h].brain == 10) dragon_brain(h);
		if (spr[h].brain == 11) missile_brain(h, /*true*/1);
		if (spr[h].brain == 12) scale_brain(h);
		if (spr[h].brain == 13) mouse_brain(h);
		if (spr[h].brain == 14) button_brain(h);
		if (spr[h].brain == 15) shadow_brain(h);
		if (spr[h].brain == 16) people_brain(h);
		if (spr[h].brain == 17) missile_brain_expire(h);

	animate:
		move_result = check_if_move_is_legal(h);
		
		if (warp_editor_sprite != -500) {
			log_debug("move result is %d", warp_editor_sprite);
			move_result = warp_editor_sprite;
			warp_editor_sprite = -500;
		}
		
		if (spr[h].brain == 1
			&& move_result > 100
			&& cur_ed_screen.sprite[move_result-100].is_warp == 1)
			special_block(move_result - 100);

		{
			bool isHitting = false;
			live_sprite_animate(h, thisTickCount, &isHitting);
			if (isHitting) {
				//this sprite can damage others right now!
				//lets run through the list and tag sprites who were hit with their damage
				run_through_tag_list(h, spr[h].strength);
			}
		}


	past:
		check_seq_status(spr[h].seq);
		draw_sprite_game(IOGFX_backbuffer, h);
	} /* for 0->max_s */
	
	apply_mode();

	/* Screen transition? */
	if (spr[1].active && spr[1].brain == 1) {
	  if (did_player_cross_screen()) {
		  /* let's restart and draw the next screen,
	       did_player_cross_screen->grab_trick() screenshot'd the current one
		   and drew base background, just need to place the active sprites */
	    get_frame = true;
	    goto trigger_start;
	  }
	}
	
	/* Screen transition */
	if (get_frame) {
	  get_frame = false;
	  transition_in_progress = 1;
	  SDL_Rect src = { playl, 0, 620 - playl, 400 };
	  IOGFX_tmp2->blit(IOGFX_backbuffer, &src, NULL);
	  abort_this_flip = 1;
	  return;
	}
	
	if (screenlock == 1) {
		//Msg("Drawing screenlock.");
		drawscreenlock();
	}
	
	for (int j = 0; j <= max_s; j++) {
	    int sprite = rank[j];
	    if (sprite > 0 && spr[sprite].active && spr[sprite].brain == 8)
			text_draw(sprite, g_display->brightness);
	}
    
    
	game_choice_logic(); // after brain_keyboard(), otherwise choice triggers Attack
	game_choice_renderer_render();
	
	kill_scripts_with_inactive_sprites();
	process_callbacks(thisTickCount);
}
