/**
 * Fonts

 * Copyright (C) 2007, 2008, 2009, 2014, 2015  Sylvain Beucler

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

#if defined _WIN32 || defined __WIN32__ || defined __CYGWIN__
#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0401
#include <windows.h>
#include <shlobj.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <algorithm>
#include <climits>
#include <sstream>
#include "SDL.h"
#include "SDL_ttf.h"
#include "gfx.h"
#include "resources.h"
#include "paths.h"
#include "gfx_fonts.h"
#include "gfx_palette.h"
#include "ImageLoader.h"
#include "vgasys_fon.h"
#include "log.h"

using namespace std;

/* Default fonts: dialog and system */
TTF_Font *dialog_font = NULL;
static TTF_Font *system_font = NULL;

/* Current font parameters */
static SDL_Color text_color;

static TTF_Font *load_default_font();


SDL_Color font_colors[16];

#ifdef HAVE_FONTCONFIG
#include <fontconfig/fontconfig.h>
/* Get filename for canonical font name 'fontname'. Return NULL if the
   font cannot be found (for correctness, no alternate font will be
   provided). */
char* get_fontconfig_path(char* fontname)
{
  char* filename = NULL;
  FcPattern* p = NULL;
  FcChar8* strval = NULL;
  FcObjectSet *attr = NULL;

  if (!FcInit())
    {
      log_error("get_fontconfig_path: cannot initialize fontconfig");
      return NULL;
    }

  p = FcNameParse((FcChar8*)fontname);
  if (p == NULL)
    {
      log_error("get_fontconfig_path: invalid font pattern: %s", fontname);
      return NULL;
    }
  /* Grab filename attribute */
  attr = FcObjectSetBuild (FC_FILE, (char *) 0);

  FcFontSet *fs = FcFontList (0, p, attr);
  if (fs->nfont == 0)
    {
      log_error("get_fontconfig_path: no matching font for %s", fontname);
      return NULL;
    }
  if (FcPatternGetString(fs->fonts[0], FC_FILE, 0, &strval) == FcResultTypeMismatch
      || strval == NULL)
    {
      log_error("get_fontconfig_path: cannot find font filename for %s", fontname);
      return NULL;
    }

  filename = strdup((char*)strval);

  FcFontSetDestroy(fs);
  FcObjectSetDestroy(attr);
  FcPatternDestroy(p);
  FcFini();

  return filename;
}
#endif


/**
 * Init font subsystem and load the default fonts
 */
int gfx_fonts_init()
{
  // FreeType subpixel hinting makes old fonts (Arial) ugly, use old renderer
  // Cf. https://sourceforge.net/projects/freetype/files/freetype2/2.7/
  // and include/freetype/ftttdrv.h
  putenv("FREETYPE_PROPERTIES=truetype:interpreter-version=35");

  if (TTF_Init() == -1) {
    log_error("%s", TTF_GetError());
    return -1;
  }

  /* Load system font from compiled data */
  system_font = TTF_OpenFontRW(SDL_RWFromConstMem(vgasys_fon, sizeof(vgasys_fon)),
			       1, FONT_SIZE);
  if (system_font == NULL)
    {
      log_error("Failed to load built-in 'vgasys.fon' font: %s", SDL_GetError());
      return -1;
    }
  setup_font(system_font);

  /* Load dialog font from built-in resources */
  dialog_font = load_default_font();
  if (dialog_font == NULL)
    return -1; /* error message set by load_default_font */
  setup_font(dialog_font);

  gfx_fonts_init_colors();

  return 0;
}

/**
 * Quit the font subsystem (and free loaded fonts from memory)
 */
void gfx_fonts_quit(void)
{
  if (dialog_font != NULL)
    {
      TTF_CloseFont(dialog_font);
      dialog_font = NULL;
    }
  if (system_font != NULL)
    {
      TTF_CloseFont(system_font);
      system_font = NULL;
    }

  TTF_Quit();
}


/**
 * Default font from resources and pkgdatadir
 */
static TTF_Font *load_default_font() {
  TTF_Font *font_object = NULL;
  SDL_RWops* rwops = NULL;

  /* Remake: allow overriding the default font (e.g. a Persian-capable one)
     without touching fontconfig — also the mechanism used on Android. */
  if (rwops == NULL)
    {
      const char* envfont = getenv("FREEDINK_FONT");
      if (envfont != NULL && envfont[0] != '\0')
        {
          rwops = SDL_RWFromFile(envfont, "rb");
          if (rwops != NULL)
            log_info("Loaded default font from FREEDINK_FONT=%s", envfont);
          else
            log_error("FREEDINK_FONT set but unreadable: %s", envfont);
        }
    }

  /* Try from resources */
  if (rwops == NULL)
    {
      rwops = find_resource_as_rwops("LiberationSans-Regular.ttf");
      if (rwops != NULL)
          log_info("Loaded LiberationSans-Regular.ttf from %s", paths_getpkgdatadir());
    }
#ifdef HAVE_FONTCONFIG
  if (rwops == NULL)
    {
      char *path = get_fontconfig_path("Liberation Sans:style=Regular");
      rwops = SDL_RWFromFile(path, "rb");
      if (rwops != NULL)
          log_info("Loaded LiberationSans-Regular:style=Regular from fontconfig at %s",
                   path);
      free(path);
    }
#endif
  if (rwops == NULL)
    {
      log_error("Could not open font 'LiberationSans-Regular.ttf'. I tried:\n"
							 "- loading from '%s' [build prefix]\n"
#ifdef HAVE_FONTCONFIG
			 "- querying fontconfig"
#endif
			 ,
			 paths_getpkgdatadir());
      return NULL;
    }

  font_object = TTF_OpenFontRW(rwops, 1, FONT_SIZE);
  if (font_object == NULL)
    {
      log_error("Could not open font 'LiberationSans-Regular.ttf': %s", TTF_GetError());
      return NULL;
    }

  return font_object;
}

/**
 * Change the current dialog font (DinkC initfont() command)
 */
int initfont(char* fontname) {
  TTF_Font *new_font = NULL;
  char* ext = ".ttf";
  char* filename = (char*)malloc(strlen(fontname) + strlen(ext) + 1);
  strcpy(filename, fontname);
  strcat(filename, ext);

  if (new_font == NULL)
    {
      char *path = NULL;
#if defined _WIN32 || defined __WIN32__ || defined __CYGWIN__
      /* Look in system fonts dir */
      path = (char*)malloc(MAX_PATH + 1 + strlen(filename) + 1);
      /* C:\WINNT\Fonts */
      SHGetSpecialFolderPath(NULL, path, CSIDL_FONTS, 0);
      strcat(path, "\\");
      strcat(path, filename);
#else
#  ifdef HAVE_FONTCONFIG
      path = get_fontconfig_path(fontname);
#  else
      path = NULL;
#  endif
#endif
      if (path == NULL)
	{
	  log_error("initfont: cannot find '%s'", fontname);
	}
      else
	{
	  new_font = TTF_OpenFont(path, FONT_SIZE);
	  if (new_font == NULL)
	    log_error("TTF_OpenFont: %s", TTF_GetError());
	  free(path);
	}
    }

  if (new_font == NULL)
    return -1;


  /* new_font could be loaded - we can free the previous one */
  TTF_CloseFont(dialog_font);
  dialog_font = new_font;

  setup_font(dialog_font);

  return 0;
}

/**
 * Change a font color (DinkC set_font_color() command)
 */
void set_font_color(int no, int r, int g, int b)
{
  if (no >= 1 && no <= 15
      && r   >= 0 && r   <= 255
      && g >= 0 && g <= 255
      && b  >= 0 && b  <= 255)
    {
      font_colors[no].r = r;
      font_colors[no].g = g;
      font_colors[no].b = b;
    }
}


/**
 * Apply default style to the font
 * Plus some informative output
 */
void setup_font(TTF_Font *font)
{
  const char *familyname = TTF_FontFaceFamilyName(font);
  if(familyname)
    log_info("The family name of the face in the font is: %s", familyname);
  const char *stylename = TTF_FontFaceStyleName(font);
  if(stylename)
    log_info("The name of the face in the font is: %s", stylename);
  log_info("The font max height is: %d", TTF_FontHeight(font));
  log_info("The font ascent is: %d", TTF_FontAscent(font));
  log_info("The font descent is: %d", TTF_FontDescent(font));
  log_info("The font line skip is: %d", TTF_FontLineSkip(font));
  if(TTF_FontFaceIsFixedWidth(font))
    log_info("The font is fixed width.");
  else
    log_info("The font is not fixed width.");

  TTF_SetFontStyle(font, TTF_STYLE_BOLD);
}




void FONTS_SetTextColor(Uint8 r, Uint8 g, Uint8 b) {
  text_color.r = r;
  text_color.g = g;
  text_color.b = b;
}


/* ------------------------------------------------------------------ */
/* Remake: dependency-free Persian/Arabic text shaping (phase 2-2).    */
/* Maps letters to Unicode Presentation Forms (A/B) with correct      */
/* initial/medial/final/isolated joining, LAM-ALEF ligatures, ZWNJ,   */
/* then applies a light bidi pass (RTL with LTR digit/latin runs).    */
/* Requires a font with FB50/FE70 blocks in cmap (e.g. Vazirmatn).    */
/* ------------------------------------------------------------------ */
struct ArabForms { Uint32 cp, iso, fin, ini, med; };
/* ini==0 means right-joining only (never connects to the next letter) */
static const ArabForms arab_forms[] = {
  {0x0621,0xFE80,0xFE80,0,0},      {0x0622,0xFE81,0xFE82,0,0},
  {0x0623,0xFE83,0xFE84,0,0},      {0x0624,0xFE85,0xFE86,0,0},
  {0x0625,0xFE87,0xFE88,0,0},      {0x0626,0xFE89,0xFE8A,0xFE8B,0xFE8C},
  {0x0627,0xFE8D,0xFE8E,0,0},      {0x0628,0xFE8F,0xFE90,0xFE91,0xFE92},
  {0x0629,0xFE93,0xFE94,0,0},      {0x062A,0xFE95,0xFE96,0xFE97,0xFE98},
  {0x062B,0xFE99,0xFE9A,0xFE9B,0xFE9C}, {0x062C,0xFE9D,0xFE9E,0xFE9F,0xFEA0},
  {0x062D,0xFEA1,0xFEA2,0xFEA3,0xFEA4}, {0x062E,0xFEA5,0xFEA6,0xFEA7,0xFEA8},
  {0x062F,0xFEA9,0xFEAA,0,0},      {0x0630,0xFEAB,0xFEAC,0,0},
  {0x0631,0xFEAD,0xFEAE,0,0},      {0x0632,0xFEAF,0xFEB0,0,0},
  {0x0633,0xFEB1,0xFEB2,0xFEB3,0xFEB4}, {0x0634,0xFEB5,0xFEB6,0xFEB7,0xFEB8},
  {0x0635,0xFEB9,0xFEBA,0xFEBB,0xFEBC}, {0x0636,0xFEBD,0xFEBE,0xFEBF,0xFEC0},
  {0x0637,0xFEC1,0xFEC2,0xFEC3,0xFEC4}, {0x0638,0xFEC5,0xFEC6,0xFEC7,0xFEC8},
  {0x0639,0xFEC9,0xFECA,0xFECB,0xFECC}, {0x063A,0xFECD,0xFECE,0xFECF,0xFED0},
  {0x0641,0xFED1,0xFED2,0xFED3,0xFED4}, {0x0642,0xFED5,0xFED6,0xFED7,0xFED8},
  {0x0643,0xFED9,0xFEDA,0xFEDB,0xFEDC}, {0x0644,0xFEDD,0xFEDE,0xFEDF,0xFEE0},
  {0x0645,0xFEE1,0xFEE2,0xFEE3,0xFEE4}, {0x0646,0xFEE5,0xFEE6,0xFEE7,0xFEE8},
  {0x0647,0xFEE9,0xFEEA,0xFEEB,0xFEEC}, {0x0648,0xFEED,0xFEEE,0,0},
  {0x0649,0xFEEF,0xFEF0,0,0},      {0x064A,0xFEF1,0xFEF2,0xFEF3,0xFEF4},
  /* Persian letters (Presentation Forms-A) */
  {0x067E,0xFB56,0xFB57,0xFB58,0xFB59}, /* پ */
  {0x0686,0xFB7A,0xFB7B,0xFB7C,0xFB7D}, /* چ */
  {0x0698,0xFB8A,0xFB8B,0,0},           /* ژ */
  {0x06A9,0xFB8E,0xFB8F,0xFB90,0xFB91}, /* ک */
  {0x06AF,0xFB92,0xFB93,0xFB94,0xFB95}, /* گ */
  {0x06CC,0xFBFC,0xFBFD,0xFBFE,0xFBFF}, /* ی */
  {0x06C0,0xFBA4,0xFBA5,0,0},           /* ۀ */
};
#define ZWNJ 0x200C

static const ArabForms* arab_lookup(Uint32 cp) {
	for (unsigned i = 0; i < sizeof(arab_forms)/sizeof(arab_forms[0]); i++)
		if (arab_forms[i].cp == cp) return &arab_forms[i];
	return NULL;
}
static int is_ltr_keep(Uint32 c) { /* chars whose run stays left-to-right */
	return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
	    || (c >= 0x0660 && c <= 0x0669) || (c >= 0x06F0 && c <= 0x06F9);
}
static Uint32 mirror_bracket(Uint32 c) {
	switch (c) { case '(': return ')'; case ')': return '(';
	case '[': return ']'; case ']': return '[';
	case '{': return '}'; case '}': return '{';
	case '<': return '>'; case '>': return '<'; }
	return c;
}

/* Returns a freshly malloc'ed shaped UTF-8 string, or NULL if `str`
   contains no Arabic-script character (caller then uses `str` as-is). */
static char* shape_rtl_utf8(const char* str) {
	size_t len = strlen(str);
	Uint32* cps = (Uint32*)malloc((len + 1) * sizeof(Uint32));
	size_t n = 0; int has_arabic = 0;
	/* --- UTF-8 decode (tolerant) --- */
	for (size_t i = 0; i < len;) {
		unsigned char c = str[i]; Uint32 cp = 0xFFFD; size_t adv = 1;
		if (c < 0x80) { cp = c; }
		else if ((c & 0xE0) == 0xC0 && i+1 < len) { cp = ((c&0x1F)<<6)|(str[i+1]&0x3F); adv = 2; }
		else if ((c & 0xF0) == 0xE0 && i+2 < len) { cp = ((c&0x0F)<<12)|((str[i+1]&0x3F)<<6)|(str[i+2]&0x3F); adv = 3; }
		else if ((c & 0xF8) == 0xF0 && i+3 < len) { cp = ((c&0x07)<<18)|((str[i+1]&0x3F)<<12)|((str[i+2]&0x3F)<<6)|(str[i+3]&0x3F); adv = 4; }
		if ((cp >= 0x0600 && cp <= 0x06FF) || cp == ZWNJ) has_arabic = 1;
		cps[n++] = cp; i += adv;
	}
	if (!has_arabic) { free(cps); return NULL; }

	/* --- joining pass -> presentation forms (in logical order) --- */
	Uint32* out = (Uint32*)malloc((n + 1) * sizeof(Uint32));
	size_t m = 0;
	for (size_t i = 0; i < n; i++) {
		const ArabForms* cur = arab_lookup(cps[i]);
		if (!cur) { if (cps[i] != ZWNJ) out[m++] = cps[i]; continue; }
		/* previous connects to us? (it must be dual-joining, no ZWNJ gap) */
		int prev_links = 0;
		if (i > 0 && cps[i-1] != ZWNJ) {
			const ArabForms* p = arab_lookup(cps[i-1]);
			if (p && p->ini) prev_links = 1;
		}
		/* LAM + ALEF ligature */
		if (cps[i] == 0x0644 && i+1 < n) {
			Uint32 lig = 0;
			switch (cps[i+1]) { case 0x0622: lig = 0xFEF5; break;
			case 0x0623: lig = 0xFEF7; break; case 0x0625: lig = 0xFEF9; break;
			case 0x0627: lig = 0xFEFB; break; }
			if (lig) { out[m++] = lig + (prev_links ? 1 : 0); i++; continue; }
		}
		/* we connect to next? (we must be dual-joining, next joinable, no ZWNJ) */
		int next_links = 0;
		if (cur->ini && i+1 < n && cps[i+1] != ZWNJ && arab_lookup(cps[i+1]))
			next_links = 1;
		if (prev_links && next_links)      out[m++] = cur->med ? cur->med : cur->fin;
		else if (prev_links)               out[m++] = cur->fin;
		else if (next_links)               out[m++] = cur->ini ? cur->ini : cur->iso;
		else                               out[m++] = cur->iso;
	}
	free(cps);

	/* --- light bidi: reverse everything, then un-reverse LTR runs --- */
	for (size_t i = 0; i < m / 2; i++) {
		Uint32 t = out[i]; out[i] = out[m-1-i]; out[m-1-i] = t;
	}
	for (size_t i = 0; i < m; i++) out[i] = mirror_bracket(out[i]);
	for (size_t i = 0; i < m;) {
		if (is_ltr_keep(out[i])) {
			size_t j = i; while (j < m && is_ltr_keep(out[j])) j++;
			for (size_t a = i, b = j - 1; a < b; a++, b--) {
				Uint32 t = out[a]; out[a] = out[b]; out[b] = t;
			}
			i = j;
		} else i++;
	}

	/* --- UTF-8 encode --- */
	char* res = (char*)malloc(m * 4 + 1);
	size_t w = 0;
	for (size_t i = 0; i < m; i++) {
		Uint32 cp = out[i];
		if (cp < 0x80) res[w++] = (char)cp;
		else if (cp < 0x800) { res[w++] = 0xC0|(cp>>6); res[w++] = 0x80|(cp&0x3F); }
		else if (cp < 0x10000) { res[w++] = 0xE0|(cp>>12); res[w++] = 0x80|((cp>>6)&0x3F); res[w++] = 0x80|(cp&0x3F); }
		else { res[w++] = 0xF0|(cp>>18); res[w++] = 0x80|((cp>>12)&0x3F); res[w++] = 0x80|((cp>>6)&0x3F); res[w++] = 0x80|(cp&0x3F); }
	}
	res[w] = '\0';
	free(out);
	return res;
}
/* ------------------------------------------------------------------ */

/*static*/ SDL_Surface* print_text(TTF_Font * font, char *str, SDL_Color /*&*/color) {
	if (strlen (str) == 0)
		return NULL;

	if (!truecolor) {
		// Text color is using the physical palette (no palette-switching effect)
		SDL_PixelFormat* fmt = gfx_palette_get_phys_format();
		Uint32 phys_index = SDL_MapRGB(fmt, color.r, color.g, color.b);
		SDL_GetRGB(phys_index, ImageLoader::blitFormat->format, &(color.r), &(color.g), &(color.b));
		SDL_FreeFormat(fmt);
	}

	/* Remake: shape Persian/Arabic text before rendering (phase 2-2) */
	char* shaped = shape_rtl_utf8(str);

	/* Transparent, low quality - closest to the original engine. */
	/* Also we do need a monochrome rendering for palette support above */
	SDL_Surface* tmp = TTF_RenderUTF8_Solid(font, shaped ? shaped : str, color);
	if (shaped)
		free(shaped);
	if (tmp == NULL) {
		log_error("Error rendering text: %s; font is %p", TTF_GetError(), font);
		return NULL;
	}

	return tmp;
}

/**
 * Get the size in pixel of 'len' chars starting at 'str'
 */
static int
font_len (TTF_Font *font, char *str, int len)
{
  int text_w;
  char *tmp;

  /* Get the specified string portion and terminate it by \0 */
  tmp = (char *) malloc ((len + 1) * sizeof (char));
  strncpy (tmp, str, len);
  tmp[len] = 0;

  TTF_SizeUTF8 (font, tmp, &text_w, NULL);
  free (tmp);

  return text_w;
}

/**
 * Add newlines in the text so that it fit in 'box'
 * (a.k.a. word-wrapping)
 */
static int
process_text_for_wrapping (TTF_Font *font, char *str, int max_len)
{
  //printf("process_text_for_wrapping: %s on %dx%d\n", str, box->right - box->left, box->bottom - box->top);
  int i, start, line, last_fit;

  start = 0;
  i = 0;
  last_fit = -1;
  line = 0;
  while (str[i] != '\0')
    {
      int len;

      /* Skip forward to the end of the word */
      while (str[i] != '\0' && str[i] != ' ' && str[i] != '\n')
	i++;

      /* If the length of the text from start to i is bigger than the
	 box, then draw the text up to the last fitting portion -
	 unless that was the beginning of the string. */
      len = font_len (font, &str[start], i - start);

      if (len > max_len)
	{
	  /* String is bigger than the textbox */

	  if (last_fit == -1)
	    {
	      /* Current word is too long by itself already, let's
		 keep it on a single line */
	      if (str[i] != '\0')
		str[i] = '\n';
	      /* continue on a new line */
	      line++;
	      start = i + 1;
	      if (str[i] != '\0')
		i++;
	    }
	  else
	    {
	      /* All those words is bigger than the textbox, linebreak
		 at previous space */
	      str[last_fit] = '\n';
	      /* continue on a new line */
	      line++;
	      start = last_fit + 1;
	      i = last_fit + 1;
	    }
	  last_fit = -1;
	}
      else if (str[i] == '\0')
	{
	  line++;
	}
      else if (str[i] == '\n')
	{
	  line++;
	  i++;
	  start = i;
	  last_fit = -1;
	}
      else
	{
	  last_fit = i;
	  i++;
	}
    }

  return line;
}

/**
 * Print text 'str' in 'box', adding newlines if necessary
 * (word-wrapping). Return the text height in pixels.
 * 
 * calc_only: don't actually draw text on screen, but still compute
 * the text height
 */
int print_text_wrap_getcmds(char *str, rect* box,
		/*bool*/int hcenter, int calc_only, FONT_TYPE font_type, std::vector<TextCommand>* cmds) {
	int x, y, res_height;
	char *tmp, *pline, *pc;
	int this_is_last_line = 0;
	SDL_Color color = text_color;
	TTF_Font *font;
	int lineskip = 0;

	if (font_type == FONT_DIALOG)
		font = dialog_font;
	else if (font_type == FONT_SYSTEM)
		font = system_font;
	else {
		log_error("Error: unknown font type %d", font_type);
		exit(1);
	}

	/* Workaround: with vgasys.fon, lineskip is always 1. We'll use it's
	   height instead. */
	lineskip = TTF_FontLineSkip(font);
	if (lineskip == 1)
		lineskip = TTF_FontHeight(font);

	tmp = strdup(str);
	process_text_for_wrapping(font, tmp, box->right - box->left);

	x = box->left;
	y = box->top;

	res_height = 0;
	pline = pc = tmp;
	this_is_last_line = 0;
	while (!this_is_last_line) {
		while (*pc != '\n' && *pc != '\0')
			pc++;

		if (*pc == '\0')
			this_is_last_line = 1;
		else
			/* Terminate the current line to feed it to print_text */
			*pc= '\0';

		if (!calc_only) {
			SDL_Surface* img = print_text(font, pline, color);
			if (img != NULL) {
				int w = box->right - box->left;

				SDL_Rect src;
				src.x = src.y = 0;
				src.w = min(w, img->w); // truncate text if outside the box
				src.h = img->h;

				SDL_Rect dst;
				dst.x = x;
				if (hcenter) {
					dst.x += w / 2;
					dst.x -= img->w / 2;
				}
				dst.y = y + res_height;
				TextCommand cmd = {img, src, dst};
				cmds->push_back(cmd);
			}
		}

		res_height += lineskip;

		/* advance to next line*/
		pc++;
		pline = pc;
	}
	free(tmp);
	return res_height;
}

void print_text_cmds(std::vector<TextCommand>* cmds) {
	for (std::vector<TextCommand>::iterator it = cmds->begin(); it != cmds->end(); ++it) {
		if (!truecolor)
			it->img = ImageLoader::convertToPaletteFormat(it->img);
		IOGfxSurface* surf = g_display->upload(it->img);
		IOGFX_backbuffer->blit(surf, &it->src, &it->dst);
		delete surf;
	}
}

void print_text_cache(IOGfxSurface* surf, SDL_Rect dst, int x, int y) {
	dst.x += x;
	dst.y += y;
	IOGFX_backbuffer->blit(surf, NULL, &dst);
}

IOGfxSurface* print_text_flatten_cmds(std::vector<TextCommand>* cmds) {
	// Determine bounding box
	int min_x = INT_MAX, max_x = 0, min_y = INT_MAX, max_y = 0;
	for (std::vector<TextCommand>::iterator it = cmds->begin(); it != cmds->end(); ++it) {
		min_x = min(min_x, it->dst.x);
		min_y = min(min_y, it->dst.y);
		max_x = max(max_x, it->dst.x+it->src.w);
		max_y = max(max_y, it->dst.y+it->src.h);
	}
	SDL_Surface* flat = ImageLoader::newWithBlitFormat(max_x-min_x, max_y-min_y);

	// Find unused color for transparency
	Uint32 key = 0;
	bool found;
	do {
		found = false;
		SDL_Color cur;
		if (flat->format->format == SDL_PIXELFORMAT_INDEX8)
			cur = flat->format->palette->colors[key];
		else
			cur = {(Uint8)key, 0, 0, 255};
		for (std::vector<TextCommand>::iterator it = cmds->begin(); it != cmds->end(); ++it) {
			if (memcmp(&it->img->format->palette->colors[1],
					&cur,
					sizeof(SDL_Color)) == 0) {
				key++;
				found = true;
				break;
			}
		}
	} while (found);
	SDL_FillRect(flat, NULL, key);
	SDL_SetColorKey(flat, SDL_TRUE, key);

	// Blit all texts
	for (std::vector<TextCommand>::iterator it = cmds->begin(); it != cmds->end(); ++it) {
		// Normalize coordinates
		it->dst.x -= min_x;
		it->dst.y -= min_y;
		if (SDL_BlitSurface(it->img, &it->src, flat, &it->dst) != 0)
			log_error("print_text_flatten_cmds: %s", SDL_GetError());
		SDL_FreeSurface(it->img);
	}
	cmds->clear();
	TextCommand cmd = {flat, {0,0,flat->w,flat->h}, {min_x,min_y,0,0}};
	cmds->push_back(cmd);

	return g_display->upload((*cmds)[0].img);
}

int print_text_wrap(char *str, rect* box,
		/*bool*/int hcenter, int calc_only, FONT_TYPE font_type) {
	  std::vector<TextCommand> cmds;
	  int res = print_text_wrap_getcmds(str, box, hcenter, calc_only, font_type, &cmds);
	  print_text_cmds(&cmds);
	  return res;
}

/**
 * Display text for debug mode (with a white background)
 */
void
print_text_wrap_debug(const char *text, int x, int y)
{
  if (text == NULL || text[0] == '\0')
    return;

  char *tmp, *pline, *pc;
  int this_is_last_line = 0;
  int res_height = 0;
  SDL_Color bgcolor = {255, 255, 255};
  int max_len = 640;

  /* Workaround: with vgasys.fon, lineskip is always 1. We'll use it's
     height instead. */
  int lineskip = TTF_FontHeight(system_font);

  int textlen = strlen(text);
  tmp = (char*)malloc(strlen(text) + 1);
  /* drop '\r' */
  pc = tmp;
  int i;
  for (i = 0; i < textlen; i++)
    if (text[i] == '\r' && text[i+1] == '\n')
      continue;
    else
      *(pc++) = text[i];
  *pc = '\0';
  
  process_text_for_wrapping(system_font, tmp, max_len);

  pline = pc = tmp;
  this_is_last_line = 0;
  while (!this_is_last_line)
    {
      while (*pc != '\n' && *pc != '\0')
	pc++;

      if (*pc == '\0')
	this_is_last_line = 1;
      else
	/* Terminate the current line to feed it to print_text */
	*pc= '\0';

      SDL_Rect dst = {x, y + res_height, -1, -1};
      /* Remake: shape Persian/Arabic in console/system text too */
      char* shaped_line = shape_rtl_utf8(pline);
      SDL_Surface* rendered_text = TTF_RenderUTF8_Shaded(system_font, shaped_line ? shaped_line : pline, text_color, bgcolor);
      if (shaped_line)
        free(shaped_line);
      if (!truecolor)
    	  rendered_text = ImageLoader::convertToPaletteFormat(rendered_text);
      IOGfxSurface* rendered_text2 = g_display->upload(rendered_text);
      IOGFX_backbuffer->blit(rendered_text2, NULL, &dst);
      delete rendered_text2;

      res_height += lineskip;

      /* advance to next line*/
      pc++;
      pline = pc;
    }
  free(tmp);
}



/* Say, SaySmall: only used by freedinkedit.c */
/**
 * SaySmall: print text in a 40x40 small square; without font border
 * (sprite info boxes when typing 'I', plus something in tile
 * hardness)
 */
void SaySmall(char thing[500], int px, int py, int r, int g, int b) {
	rect rcRect;
	rect_set(&rcRect,px,py,px+40,py+40);
	FONTS_SetTextColor(r, g, b);
	print_text_wrap(thing, &rcRect, 0, 0, FONT_SYSTEM);
}
/**
 * Say: print text until it reaches the border of the screen, with a
 * font border (input dialog boxes)
 */
void Say(char thing[500], int px, int py) {
	rect rcRect;
	rect_set(&rcRect,px,py,620,480);
	SDL_Color fg = {255,255,0};
	SDL_Color bg = {8,14,21};

	std::vector<TextCommand> cmds;
	FONTS_SetTextColor(bg.r, bg.g, bg.b);
	print_text_wrap_getcmds(thing, &rcRect, 0, 0, FONT_DIALOG, &cmds);
	rect_offset(&rcRect,-2,-2);
	print_text_wrap_getcmds(thing, &rcRect, 0, 0, FONT_DIALOG, &cmds);

	FONTS_SetTextColor(fg.r, fg.g, fg.b);
	rect_offset(&rcRect,1,1);
	print_text_wrap_getcmds(thing, &rcRect, 0, 0, FONT_DIALOG, &cmds);

	print_text_cmds(&cmds);
}


void gfx_fonts_init_colors()
{
  //Light Magenta
  font_colors[1].r = 255;
  font_colors[1].g = 198;
  font_colors[1].b = 255;

  //Dark Green
  font_colors[2].r = 131;
  font_colors[2].g = 181;
  font_colors[2].b = 74;

  //Bold Cyan
  font_colors[3].r = 99;
  font_colors[3].g = 242;
  font_colors[3].b = 247;

  //Orange
  font_colors[4].r = 255;
  font_colors[4].g = 156;
  font_colors[4].b = 74;

  //Magenta
  font_colors[5].r = 222;
  font_colors[5].g = 173;
  font_colors[5].b = 255;

  //Brown Orange
  font_colors[6].r = 244;
  font_colors[6].g = 188;
  font_colors[6].b = 73;

  //Light Gray
  font_colors[7].r = 173;
  font_colors[7].g = 173;
  font_colors[7].b = 173;

  //Dark Gray
  font_colors[8].r = 85;
  font_colors[8].g = 85;
  font_colors[8].b = 85;

  //Sky Blue
  font_colors[9].r = 148;
  font_colors[9].g = 198;
  font_colors[9].b = 255;

  //Bright Green
  font_colors[10].r = 0;
  font_colors[10].g = 255;
  font_colors[10].b = 0;

  //Yellow
  font_colors[11].r = 255;
  font_colors[11].g = 255;
  font_colors[11].b = 2;

  //Yellow
  font_colors[12].r = 255;
  font_colors[12].g = 255;
  font_colors[12].b = 2;

  //Hot Pink
  font_colors[13].r = 255;
  font_colors[13].g = 132;
  font_colors[13].b = 132;

  //Yellow
  font_colors[14].r = 255;
  font_colors[14].g = 255;
  font_colors[14].b = 2;

  //White
  font_colors[15].r = 255;
  font_colors[15].g = 255;
  font_colors[15].b = 255;
}
