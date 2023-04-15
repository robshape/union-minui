#ifndef _SDLUTILS_H_
#define _SDLUTILS_H_

#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "window.h"

namespace SDL_utils
{
    // Text alignment
    typedef enum
    {
        T_TEXT_ALIGN_LEFT = 0,
        T_TEXT_ALIGN_RIGHT,
        T_TEXT_ALIGN_CENTER
    }
    T_TEXT_ALIGN;

#if defined(PLATFORM_RG350) || defined(PLATFORM_ODBETA)	
	// identify IPU if available
	void checkIPU(void);
	void setIPUSharpness(const char *svalue);
#endif

    // Load an image using SDL_image
    SDL_Surface *loadImage(const std::string &p_filename);

    // Load a TTF font
    TTF_Font *loadFont(const std::string &p_font, const int p_size);

    // Apply a surface on another surface
    void applySurface(const Sint16 p_x, const Sint16 p_y, SDL_Surface* p_source, SDL_Surface* p_destination, SDL_Rect *p_clip = NULL);

    // Render a text
    SDL_Surface *renderText(TTF_Font *p_font, const std::string &p_text, const SDL_Color &p_fg);

    // Render a text and apply on a given surface
    void applyText(const Sint16 p_x, const Sint16 p_y, SDL_Surface* p_destination, TTF_Font *p_font, const std::string &p_text, const SDL_Color &p_fg, const T_TEXT_ALIGN p_align = T_TEXT_ALIGN_LEFT);

    // Create an image filled with the given color
    SDL_Surface *createImage(const int p_width, const int p_height, const Uint32 p_color);

    // Render all opened windows
    void renderAll(void);

    // Cleanup and quit
    void hastalavista(void);

    // Display a waiting window
    void pleaseWait(void);
}

// Globals
namespace Globals
{
    // Screen
    extern SDL_Surface *g_screen;
#if defined(PLATFORM_MIYOOMINI) || defined(PLATFORM_RG35XX)
    extern SDL_Surface *g_scaled;
#endif
    // Colors
    extern const SDL_Color g_colorTextNormal;
    extern const SDL_Color g_colorTextTitle;
    extern const SDL_Color g_colorTextDir;
    extern const SDL_Color g_colorTextSelected;
    // The list of opened windows
    extern std::vector<CWindow *> g_windows;
}

#if defined(PLATFORM_MIYOOMINI) || defined(PLATFORM_RG35XX)
void upscale16NEON(void* src, void* dst);
#endif

#endif
