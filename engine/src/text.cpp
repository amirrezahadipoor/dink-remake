/**
 * Display game texts

 * Copyright (C) 1997, 1998, 1999, 2002, 2003  Seth A. Robinson
 * Copyright (C) 2014  Sylvain Beucler

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

#include "text.h"

#include <stdlib.h>
#include <string.h>
#include "SDL.h"

#include "game_engine.h"
#include "live_sprites_manager.h"
#include "dinkini.h" /* check_seq_status */
#include "gfx.h"
#include "gfx_fonts.h"
#include "gfx_sprites.h"
#include "text.h"
#include "log.h"

#define TEXT_MIN 2700
#define TEXT_TIMER 77

/* say_text, say_text_xy */
int add_text_sprite(char* text, int script, int sprite_owner, int mx, int my)
{
  int tsprite = add_sprite(mx, my, 8, 0, 0);
  if (tsprite == 0)
    {
      log_error("Couldn't say something, out of sprites.");
      return 0;
    }

  /* remake: 512-byte buffer + UTF-8-boundary-safe truncation
     (never leave a dangling multi-byte sequence at the cut point) */
  strncpy(spr[tsprite].text, text, 512-1);
  spr[tsprite].text[512-1] = '\0';
  {
    int e = strlen(spr[tsprite].text);
    while (e > 0 && ((unsigned char)spr[tsprite].text[e-1] & 0xC0) == 0x80
           && e >= 512-4)
      spr[tsprite].text[--e] = '\0';
    if (e > 0 && (unsigned char)spr[tsprite].text[e-1] >= 0xC0 && e >= 512-4)
      spr[tsprite].text[e-1] = '\0';
  }
  spr[tsprite].text_born = SDL_GetTicks(); /* remake: typewriter start */

  *plast_text = tsprite;
  spr[tsprite].kill_ttl = strlen(text) * TEXT_TIMER;
  if (spr[tsprite].kill_ttl < TEXT_MIN)
    spr[tsprite].kill_ttl = TEXT_MIN;
  spr[tsprite].damage = -1;
  spr[tsprite].text_owner = sprite_owner;
  spr[tsprite].hard = 1;
  spr[tsprite].script = script;
  spr[tsprite].nohit = 1;

  return tsprite;
}

int say_text(char* text, int sprite_owner, int script) {
	int tsprite;
	if (sprite_owner == 1000)
		tsprite = add_text_sprite(text, script, 1000, 100, 100);
	else if (lsm_isValidSprite(sprite_owner))
		tsprite = add_text_sprite(text, script, sprite_owner,
								  spr[sprite_owner].x, spr[sprite_owner].y);
	else
		tsprite = add_text_sprite(text, script, sprite_owner, 100, 100);
	
	if (tsprite == 0)
		return 0;
	
	//set X offset for text, using strength var since it's unused
	spr[tsprite].strength = 75;
	if (lsm_isValidSprite(sprite_owner)) {
		check_seq_status(spr[sprite_owner].seq);
		int gfx_id = getpic(sprite_owner);
		if (gfx_id >= 0 && gfx_id < MAX_SPRITES)
			spr[tsprite].defense = (k[gfx_id].box.bottom - k[gfx_id].yoffset) + 100;
		else
			spr[tsprite].defense = 100;
		
		spr[tsprite].x = spr[sprite_owner].x - spr[tsprite].strength;
		spr[tsprite].y = spr[sprite_owner].y - spr[tsprite].defense;
	}
	return tsprite;
}

int say_text_xy(char* text, int mx, int my, int script)
{
  int sprite_owner = 1000;
  return add_text_sprite(text, script, sprite_owner, mx, my);
}


/* Get sprite #h, grab its text and display it */
void text_draw(int h, double brightness) {
	char crap[512]; /* remake: match live_sprite.text */
	char *cr;
	rect rcRect;
	int color = 0;
	
	if (spr[h].damage == -1) {
		snprintf(crap, sizeof(crap), "%s", spr[h].text); /* remake: bounded */
		cr = &crap[0];
		color = 14;
		while( cr[0] == '`') {
			//color code at top
			if (cr[1] == '#') color = 13;
			if (cr[1] == '1') color = 1;
			if (cr[1] == '2') color = 2;
			if (cr[1] == '3') color = 3;
			if (cr[1] == '5') color = 5;
			if (cr[1] == '6') color = 6;
			if (cr[1] == '7') color = 7;
			if (cr[1] == '8') color = 8;
			if (cr[1] == '9') color = 9;
			if (cr[1] == '0') color = 10;
			if (cr[1] == '$') color = 14;
			if (cr[1] == '%') color = 15;
			
			if (dversion >= 108) {
			    //support for additional colors
			    if (cr[1] == '@')
					color = 12;
			    if (cr[1] == '!')
					color = 11;
			}
			
			if (cr[1] == '4') color = 4;
			cr = &cr[2];
		}
		
		// Set size
		if (spr[h].text_owner == 1000) {
			rect_set(&rcRect, spr[h].x, spr[h].y,
					 spr[h].x + 620, spr[h].y + 400);
		} else {
			rect_set(&rcRect, spr[h].x, spr[h].y,
					 spr[h].x + 150, spr[h].y + 150);
			if ((spr[h].x + 150) > 620)
				rect_offset(&rcRect, ((spr[h].x+150)-620) - (((spr[h].x+150)-620) * 2), 0);
		}
	} else {
		// Display damage/experience points
		// Remake (M2): Persian digits for floating combat numbers
		{
			char tmp[32];
			sprintf(tmp, "%d", spr[h].damage);
			int w = 0;
			for (int ci = 0; tmp[ci] != '\0' && w < 500; ci++) {
				if (tmp[ci] >= '0' && tmp[ci] <= '9') {
					/* U+06F0..U+06F9 => 0xDB 0xB0.. */
					crap[w++] = (char)0xDB;
					crap[w++] = (char)(0xB0 + (tmp[ci] - '0'));
				} else
					crap[w++] = tmp[ci];
			}
			crap[w] = '\0';
		}
		cr = &crap[0];
		if (spr[h].brain_parm == 5000) // experience
			color = 14;
		
		if (spr[h].y < 0)
			spr[h].y = 0;
		rect_set(&rcRect, spr[h].x, spr[h].y,
				 spr[h].x + 50, spr[h].y + 50);
	}       
	
	
	/* During a fadedown/fadeup, use white text to mimic v1.07 */
	if (brightness < 256)
		color = 15;
	
	
	SDL_Color fg;
	// support for custom colors
	if (color >= 1 && color <= 15)
		fg = font_colors[color];
	else
		fg = {255,255,255};

	std::vector<TextCommand> cmds;

	/* Remake: typewriter effect (phase 2-4) — reveal dialog text
	   progressively (UTF-8 aware). Damage numbers are not affected.
	   Disable with FREEDINK_TYPEWRITER=0. */
	int fully_revealed = 1;
	static int tw_enabled = -1;
	if (tw_enabled == -1) {
		const char* e = getenv("FREEDINK_TYPEWRITER");
		tw_enabled = (e == NULL || strcmp(e, "0") != 0);
	}
	if (tw_enabled && spr[h].damage == -1) {
		const Uint32 MS_PER_CHAR = 28;
		Uint32 age = SDL_GetTicks() - spr[h].text_born;
		unsigned long want = age / MS_PER_CHAR + 1; /* chars to show */
		/* find byte offset of the want-th UTF-8 character */
		unsigned long chars = 0; char* p = cr;
		while (*p != '\0') {
			if (((unsigned char)*p & 0xC0) != 0x80) { /* not a continuation byte */
				if (chars >= want) break;
				chars++;
			}
			p++;
		}
		if (*p != '\0') {
			*p = '\0'; /* cr points into local 'crap' buffer: safe to cut */
			fully_revealed = 0;
		}
	}

	// Clear cache if color changed (i.e. during truecolor fade_down())
	// Remake perf/fix: original logic was inverted (cleared on EQUAL
	// colors) and never stored the color — now invalidate on change.
	if (spr[h].text_cache != NULL && memcmp(&spr[h].text_cache_color, &fg, sizeof(SDL_Color)) != 0) {
		delete spr[h].text_cache;
		spr[h].text_cache = NULL;
	}

	/* While revealing, render directly (no cache) so the next frame
	   can show more characters. */
	if (!fully_revealed) {
		if (spr[h].text_cache != NULL) {
			delete spr[h].text_cache;
			spr[h].text_cache = NULL;
		}
		if (strlen(cr) > 0) {
			SDL_Color bg = {8,14,21};
			rect abs_box = rcRect;
			FONTS_SetTextColor(bg.r, bg.g, bg.b);
			rect o = abs_box;
			rect_offset(&o, 1, 1);  print_text_wrap_getcmds(cr, &o, 1, 0, FONT_DIALOG, &cmds);
			rect_offset(&o, -2, 0); print_text_wrap_getcmds(cr, &o, 1, 0, FONT_DIALOG, &cmds);
			rect_offset(&o, 1, 1);  print_text_wrap_getcmds(cr, &o, 1, 0, FONT_DIALOG, &cmds);
			rect_offset(&o, 0, -2); print_text_wrap_getcmds(cr, &o, 1, 0, FONT_DIALOG, &cmds);
			FONTS_SetTextColor(fg.r, fg.g, fg.b);
			rect_offset(&o, 0, 1);  print_text_wrap_getcmds(cr, &o, 1, 0, FONT_DIALOG, &cmds);
			print_text_cmds(&cmds);
		}
		return;
	}

	/* Remake (M5): optional translucent backdrop behind fullscreen says */
	{
		extern int remake_say_frame;
		if (remake_say_frame && spr[h].damage == -1 && spr[h].text_owner == 1000) {
			static IOGfxSurface* barsurf = NULL;
			if (barsurf == NULL) {
				SDL_Surface* bs = SDL_CreateRGBSurfaceWithFormat(0, 640, 34, 32, SDL_PIXELFORMAT_ARGB8888);
				SDL_FillRect(bs, NULL, SDL_MapRGBA(bs->format, 6, 10, 18, 185));
				barsurf = g_display->upload(bs);
			}
			SDL_Rect bar = {0, rcRect.top - 6, 640, 34};
			IOGFX_backbuffer->blit(barsurf, NULL, &bar);
		}
	}

	if (spr[h].text_cache == NULL) {
		SDL_Color bg = {8,14,21};
		rect rel = rcRect;
		rect_offset(&rel, -rcRect.left, -rcRect.top);
		FONTS_SetTextColor(bg.r, bg.g, bg.b);
		print_text_wrap_getcmds(cr, &rel, 1, 0, FONT_DIALOG, &cmds);
		rect_offset(&rel,-2,0);
		print_text_wrap_getcmds(cr, &rel, 1, 0, FONT_DIALOG, &cmds);
		rect_offset(&rel,1,1);
		print_text_wrap_getcmds(cr, &rel, 1, 0, FONT_DIALOG, &cmds);
		rect_offset(&rel,0,-2);
		print_text_wrap_getcmds(cr, &rel, 1, 0, FONT_DIALOG, &cmds);

		FONTS_SetTextColor(fg.r, fg.g, fg.b);
		rect_offset(&rel,0,1);
		print_text_wrap_getcmds(cr, &rel, 1, 0, FONT_DIALOG, &cmds);

		if (cmds.size() > 0) {
			IOGfxSurface* surf = print_text_flatten_cmds(&cmds);
			spr[h].text_cache = surf;
			spr[h].text_cache_color = fg; /* remake: enable change detection */
			spr[h].text_cache_reldst = cmds[0].dst;
			print_text_cache(spr[h].text_cache, spr[h].text_cache_reldst, rcRect.left, rcRect.top);
		}
	} else {
		print_text_cache(spr[h].text_cache, spr[h].text_cache_reldst, rcRect.left, rcRect.top);
	}
}
