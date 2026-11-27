#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "IOGfxDisplay.h"

#include "SDL_image.h"

#include "log.h"
#include "freedink_xpm.h"

IOGfxDisplay::IOGfxDisplay(int w, int h, bool truecolor, Uint32 flags)
	: w(w), h(h), truecolor(truecolor), flags(flags),
	  initializedVideo(false), window(NULL),
	  brightness(256) {
}

IOGfxDisplay::~IOGfxDisplay() {
	close();  // non-virtual call
}

bool IOGfxDisplay::open() {
	if (!SDL_WasInit(SDL_INIT_VIDEO)) {
		if (SDL_InitSubSystem(SDL_INIT_VIDEO) == -1) {
			log_error("Video initialization error: %s", SDL_GetError());
			return false;
		}
		initializedVideo = true;
	}


	if (!createWindow()) return false;

	return true;
}

void IOGfxDisplay::close() {
	if (window) SDL_DestroyWindow(window);
	window = NULL;

	if (initializedVideo) {
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		initializedVideo = false;
	}
}

bool IOGfxDisplay::createWindow() {
	window = SDL_CreateWindow(PACKAGE_STRING,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		w, h, flags);
	if (window == NULL) {
		log_error("Unable to create %dx%d window: %s\n",
		          w, h, SDL_GetError());
		return false;
	}

	SDL_GetWindowSize(window, &w, &h);
	// There's a delay between theoretical and actual full-screen resize, impacting e.g. in OpenGL targets
	// Also (not worked-around here): starting in full-screen mode make the window keep the full size even when toggleFullScreen
	// TODO: fix me in SDL2 [master 2015-08]
	if (flags & SDL_WINDOW_FULLSCREEN)
		SDL_Delay(100);

	/* Window configuration */
	{
		SDL_Surface *icon = NULL;
		if ((icon = IMG_ReadXPMFromArray(freedink_xpm)) == NULL) {
			log_error("Error loading icon: %s", IMG_GetError());
		} else {
			SDL_SetWindowIcon(window, icon);
			SDL_FreeSurface(icon);
		}
	}

	return true;
}



void IOGfxDisplay::logDisplayInfo() {
	log_info("Truecolor mode: %s", truecolor ? "on" : "off");

	Uint32 Rmask=0, Gmask=0, Bmask=0, Amask=0; int bpp=0;
	SDL_PixelFormatEnumToMasks(getFormat(), &bpp,
		&Rmask, &Gmask, &Bmask, &Amask);
	log_info("SW buffer format: %s %d-bit R=0x%08x G=0x%08x B=0x%08x A=0x%08x",
			SDL_GetPixelFormatName(getFormat()),
			bpp, Rmask, Gmask, Bmask, Amask);

	logWindowInfo();
}

void IOGfxDisplay::logWindowInfo() {
	log_info("Video driver: %s", SDL_GetCurrentVideoDriver());
	log_info("Video fall-back surface (unused): %s",
	         SDL_GetPixelFormatName(SDL_GetWindowPixelFormat(window)));
	if (0) {
		// Segfaults on quit:
		//SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "opengl");
		SDL_Surface* window_surface = SDL_GetWindowSurface(window);
		log_info("Video fall-back surface (unused): %s",
				SDL_GetPixelFormatName(window_surface->format->format));
	}
}



Uint32 IOGfxDisplay::getFormat() {
    if (!truecolor)
    	return SDL_PIXELFORMAT_INDEX8;
    else
    	return SDL_PIXELFORMAT_RGB888;
}

void IOGfxDisplay::toggleFullScreen() {
	SDL_SetWindowFullscreen(window,
			(SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN)
			? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
	// Note: software renderer is buggy in SDL 2.0.2: it doesn't resize the surface
}

void IOGfxDisplay::toggleScreenKeyboard() {
	if (SDL_HasScreenKeyboardSupport()) {
		if (!SDL_IsScreenKeyboardShown(window))
			SDL_StartTextInput();
		else
			SDL_StopTextInput();
	}
}


/* Remake (M14): scene color tint (lighting mood) — night/cave/fire */
int remake_tint_r = 255, remake_tint_g = 255, remake_tint_b = 255;
void remake_scene_tint(int r, int g, int b) {
	if (r < 30) r = 30; if (r > 255) r = 255;
	if (g < 30) g = 30; if (g > 255) g = 255;
	if (b < 30) b = 30; if (b > 255) b = 255;
	remake_tint_r = r; remake_tint_g = g; remake_tint_b = b;
}

int remake_tint_locked = 0; /* M14: scripted scenes own the tint */
int remake_cinema_on = 0;   /* M13: letterbox bars */

/* Remake (M1): screen shake — applied here so BOTH renderers get it */
int remake_shake_mag = 0;
Uint32 remake_shake_until = 0;
void remake_screen_shake(int ms, int mag) {
	remake_shake_until = SDL_GetTicks() + (Uint32)ms;
	remake_shake_mag = mag;
}

void IOGfxDisplay::flipStretch(IOGfxSurface* backbuffer) {
	if (backbuffer == NULL)
		SDL_SetError("IOGfxDisplay::flipStretch: passed a NULL surface");
	SDL_Rect dstrect;
	centerScaledSurface(backbuffer, &dstrect);
	if (remake_shake_mag > 0 && SDL_GetTicks() < remake_shake_until) {
		dstrect.x += (rand() % (remake_shake_mag * 2 + 1)) - remake_shake_mag;
		dstrect.y += (rand() % (remake_shake_mag * 2 + 1)) - remake_shake_mag;
	}
	flip(backbuffer, &dstrect, true, true);
}

/* Non-stretched, backbuffer-only blit so we can extract texture buffer */
void IOGfxDisplay::flipDebug(IOGfxSurface* backbuffer) {
	if (backbuffer == NULL)
		SDL_SetError("IOGfxDisplay::flipDebug: passed a NULL surface");
	SDL_Rect dstrect = {0,0,backbuffer->w,backbuffer->h};
	flip(backbuffer, &dstrect, false, false);
}


void IOGfxDisplay::centerScaledSurface(IOGfxSurface* surf, SDL_Rect* dst) {
	double game_ratio = 1.0 * surf->w / surf->h;
	double disp_ratio = 1.0 * w / h;
	if (game_ratio < disp_ratio) {
		// left/right bars
		dst->w = surf->w * h / surf->h;
		dst->h = h;
		dst->x = (w - dst->w) / 2;
		dst->y = 0;
	} else {
		// top/bottom bars
		dst->w = w;
		dst->h = surf->h * w / surf->w;
		dst->x = 0;
		dst->y = (h - dst->h) / 2;
	}
}

/* Remake (fix اندروید): نرمال‌شده پنجره → مختصات بازی 640x480 (معکوس letterbox) */
extern IOGfxDisplay* g_display;
void remake_screen_to_game(float nx, float ny, int* gx, int* gy) {
	if (g_display == NULL) { *gx = (int)(nx*640); *gy = (int)(ny*480); return; }
	double gr = 640.0 / 480.0;
	double dr = 1.0 * g_display->w / g_display->h;
	int dw, dh, dx, dy;
	if (gr < dr) { dh = g_display->h; dw = (int)(640.0 * g_display->h / 480.0); dx = (g_display->w - dw) / 2; dy = 0; }
	else         { dw = g_display->w; dh = (int)(480.0 * g_display->w / 640.0); dx = 0; dy = (g_display->h - dh) / 2; }
	double px = nx * g_display->w, py = ny * g_display->h;
	int x = (int)((px - dx) * 640.0 / dw);
	int y = (int)((py - dy) * 480.0 / dh);
	if (x < 0) x = 0; if (x > 639) x = 639;
	if (y < 0) y = 0; if (y > 479) y = 479;
	*gx = x; *gy = y;
}

void IOGfxDisplay::surfToDisplayCoords(IOGfxSurface* backbuffer, int &x, int &y) {
	SDL_Rect r;
	centerScaledSurface(backbuffer, &r);
	// center of scaled pixels:
	double fx = x+0.5;
	double fy = y+0.5;
	x = r.x + (fx / backbuffer->w) * r.w;
	y = r.y + (fy / backbuffer->h) * r.h;
}


SDL_Surface* IOGfxDisplay::screenshot() {
	SDL_Rect rect = { 0,0,w,h };
	return screenshot(&rect);
}

void IOGfxDisplay::screenshot(const char* output_file) {
	SDL_Surface* image = screenshot();
	SDL_SaveBMP(image, output_file);
	SDL_FreeSurface(image);
}
