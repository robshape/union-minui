#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "def.h"
#include "sdlutils.h"
#include "resourceManager.h"
#include "commander.h"

// Globals
SDL_Surface *Globals::g_screen = NULL;
#if defined(PLATFORM_MIYOOMINI) || defined(PLATFORM_RG35XX)
SDL_Surface *Globals::g_scaled = NULL;
#endif
const SDL_Color Globals::g_colorTextNormal = {COLOR_TEXT_NORMAL};
const SDL_Color Globals::g_colorTextTitle = {COLOR_TEXT_TITLE};
const SDL_Color Globals::g_colorTextDir = {COLOR_TEXT_DIR};
const SDL_Color Globals::g_colorTextSelected = {COLOR_TEXT_SELECTED};
std::vector<CWindow *> Globals::g_windows;

int main(int argc, char** argv)
{
	// Avoid crash due to the absence of mouse
    {
        char l_s[]="SDL_NOMOUSE=1";
        putenv(l_s);
    }

#if defined(PLATFORM_RG35XX)
	// we launch in an overclocked state on rg35xx
	system("echo 504000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed");
#endif
	
    // Init SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Hide cursor
    SDL_ShowCursor(SDL_DISABLE);
	SDL_EnableKeyRepeat(300,100);

    // Screen
#if defined(PLATFORM_MIYOOMINI) || defined(PLATFORM_RG35XX)
    Globals::g_scaled = SDL_SetVideoMode(640, 480, SCREEN_BPP, SURFACE_FLAGS);
	if (Globals::g_scaled == NULL)
    {
        std::cerr << "SDL_SetVideoMode failed: " << SDL_GetError() << std::endl;
        return 1;
    }
	Globals::g_screen = SDL_CreateRGBSurface(SURFACE_FLAGS, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, 0,0,0,0);
	if (Globals::g_screen == NULL)
    {
        std::cerr << "SDL_CreateRGBSurface (screen) failed: " << SDL_GetError() << std::endl;
        return 1;
    }
#else
    Globals::g_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SURFACE_FLAGS);
    if (Globals::g_screen == NULL)
    {
        std::cerr << "SDL_SetVideoMode failed: " << SDL_GetError() << std::endl;
        return 1;
    }
#endif

    // Init font
    if (TTF_Init() == -1)
    {
        std::cerr << "TTF_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }
	
#if defined(PLATFORM_RG350) || defined(PLATFORM_ODBETA)
	SDL_utils::checkIPU();
	SDL_utils::setIPUSharpness("0");
#endif
	
    // Create instances
    CResourceManager::instance();
    char *home = getenv("HOME");
    std::string l_path = home ? home : PATH_DEFAULT;
    CCommander l_commander(l_path, l_path);

    // Main loop
    l_commander.execute();

    //Quit
    SDL_utils::hastalavista();

    return 0;
}
