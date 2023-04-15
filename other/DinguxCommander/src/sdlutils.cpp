#include <iostream>
#include "sdlutils.h"
#include <SDL/SDL_image.h>
#include "def.h"
#include "resourceManager.h"


#if defined(PLATFORM_RG350) || defined(PLATFORM_ODBETA)
#include <dirent.h>
std::string ipuscaling = "NONE";

// copied from gambatte-dms
void SDL_utils::checkIPU(void) {
	FILE *aspect_ratio_file = NULL;
	DIR *ipu_dir = NULL;
	std::string ipu_OpenDinguxLegacy = ("/sys/devices/platform/jz-lcd.0/keep_aspect_ratio");
	std::string ipu_RetroFW10 = ("/proc/jz/ipu_ratio");
	std::string ipu_RetroFW20 = ("/proc/jz/ipu");
	std::string ipu_OpenDingux = ("/sys/devices/platform/13080000.ipu");

	aspect_ratio_file = fopen(ipu_OpenDinguxLegacy.c_str(), "r+");
	if (aspect_ratio_file != NULL) {
		fclose(aspect_ratio_file);
		ipuscaling = ipu_OpenDinguxLegacy;
		printf("Detected IPU scaling - OpenDinguxLegacy\n");
		return;
	}
	aspect_ratio_file = fopen(ipu_RetroFW10.c_str(), "r+");
	if (aspect_ratio_file != NULL) {
		fclose(aspect_ratio_file);
		ipuscaling = ipu_RetroFW10;
		printf("Detected IPU scaling - RetroFW 1.X\n");
		return;
	}
	aspect_ratio_file = fopen("/proc/jz/gpio", "r+"); //workaround to check if the fw is retrofw2
	if (aspect_ratio_file != NULL) {
		fclose(aspect_ratio_file);
		ipuscaling = ipu_RetroFW20;
		printf("Detected IPU scaling - RetroFW 2.X\n");
		return;
	}
	ipu_dir = opendir("/sys/devices/platform/13080000.ipu");
	if (ipu_dir != NULL) {
		closedir(ipu_dir);
		ipuscaling = "NEW_OD_IPU";
		printf("Detected IPU scaling - OpenDingux\n");
		return;
	}
	printf("Could not detect IPU scaling\n");
	return;
}

void SDL_utils::setIPUSharpness(const char *svalue){
	if (ipuscaling == "NONE") return;
	else if (ipuscaling == "NEW_OD_IPU") {
		if(svalue == "0"){
			SDL_putenv("SDL_VIDEO_KMSDRM_SCALING_SHARPNESS=0");
		} else if(svalue == "1"){
			SDL_putenv("SDL_VIDEO_KMSDRM_SCALING_SHARPNESS=1");
		} else if(svalue == "2"){
			SDL_putenv("SDL_VIDEO_KMSDRM_SCALING_SHARPNESS=2");
		} else if(svalue == "3"){
			SDL_putenv("SDL_VIDEO_KMSDRM_SCALING_SHARPNESS=3");
		} else if(svalue == "4"){
			SDL_putenv("SDL_VIDEO_KMSDRM_SCALING_SHARPNESS=4");
		} else if(svalue == "5"){
			SDL_putenv("SDL_VIDEO_KMSDRM_SCALING_SHARPNESS=5");
		} else if(svalue == "6"){
			SDL_putenv("SDL_VIDEO_KMSDRM_SCALING_SHARPNESS=6");
		} else if(svalue == "7"){
			SDL_putenv("SDL_VIDEO_KMSDRM_SCALING_SHARPNESS=7");
		} else if(svalue == "8"){
			SDL_putenv("SDL_VIDEO_KMSDRM_SCALING_SHARPNESS=8");
		}
		return;
	}
	FILE *sharpness_file = NULL;
	sharpness_file = fopen("/sys/devices/platform/jz-lcd.0/sharpness_upscaling", "r+");
	if (sharpness_file != NULL) {
		fclose(sharpness_file);
		sharpness_file = fopen("/sys/devices/platform/jz-lcd.0/sharpness_upscaling", "w");
		fwrite(svalue, 1, 1, sharpness_file);
		fclose(sharpness_file);
	}
}
#endif

// Load an image using SDL_image
SDL_Surface *SDL_utils::loadImage(const std::string &p_filename)
{
    INHIBIT(std::cout << "SDL_utils::loadImage(" << p_filename << ")" << std::endl;)
    // Load image
    SDL_Surface* l_img = IMG_Load(p_filename.c_str());
    SDL_Surface* l_img2 = NULL;
    if(l_img != NULL)
    {
        // Optimize the image
        l_img2 = SDL_DisplayFormat(l_img);
        // Free the first image
        SDL_FreeSurface(l_img);
        // Set color key
        if (l_img2 != NULL)
            SDL_SetColorKey(l_img2, SDL_SRCCOLORKEY, SDL_MapRGB(l_img2->format, COLOR_KEY));
    }
    // Check errors
    if (l_img2 == NULL)
        std::cerr << "SDL_utils::loadImage: " << SDL_GetError() << std::endl;
    return l_img2;
}

void SDL_utils::applySurface(const Sint16 p_x, const Sint16 p_y, SDL_Surface* p_source, SDL_Surface* p_destination, SDL_Rect *p_clip)
{
    // Rectangle to hold the offsets
    SDL_Rect l_offset;
    // Set offsets
    l_offset.x = p_x;
    l_offset.y = p_y;
    //Blit the surface
    SDL_BlitSurface(p_source, p_clip, p_destination, &l_offset);
}

TTF_Font *SDL_utils::loadFont(const std::string &p_font, const int p_size)
{
    INHIBIT(std::cout << "SDL_utils::loadFont(" << p_font << ", " << p_size << ")" << std::endl;)
    TTF_Font *l_font = TTF_OpenFont(p_font.c_str(), p_size);
    if (l_font == NULL)
        std::cerr << "SDL_utils::loadFont: " << SDL_GetError() << std::endl;
    return l_font;
}

SDL_Surface *SDL_utils::renderText(TTF_Font *p_font, const std::string &p_text, const SDL_Color &p_fg)
{
    return TTF_RenderUTF8_Solid(p_font, p_text.c_str(), p_fg);
}

void SDL_utils::applyText(const Sint16 p_x, const Sint16 p_y, SDL_Surface* p_destination, TTF_Font *p_font, const std::string &p_text, const SDL_Color &p_fg, const T_TEXT_ALIGN p_align)
{
    SDL_Surface *l_text = renderText(p_font, p_text, p_fg);
    switch (p_align)
    {
        case T_TEXT_ALIGN_LEFT:
            applySurface(p_x, p_y, l_text, p_destination);
            break;
        case T_TEXT_ALIGN_RIGHT:
            applySurface(p_x - l_text->w, p_y, l_text, p_destination);
            break;
        case T_TEXT_ALIGN_CENTER:
            applySurface(p_x - l_text->w / 2, p_y, l_text, p_destination);
            break;
        default:
            break;
    }
    SDL_FreeSurface(l_text);
}

SDL_Surface *SDL_utils::createImage(const int p_width, const int p_height, const Uint32 p_color)
{
    // Create image in the same format as the screen
    SDL_Surface *l_ret = SDL_CreateRGBSurface(SURFACE_FLAGS, p_width, p_height, Globals::g_screen->format->BitsPerPixel, Globals::g_screen->format->Rmask, Globals::g_screen->format->Gmask, Globals::g_screen->format->Bmask, Globals::g_screen->format->Amask);
    if (l_ret == NULL)
        std::cerr << "SDL_utils::createImage: " << SDL_GetError() << std::endl;
    // Fill image with the given color
    SDL_FillRect(l_ret, NULL, p_color);
    return l_ret;
}

void SDL_utils::renderAll(void)
{
    if (Globals::g_windows.empty())
        return;
    // First window to draw is the last fullscreen
    unsigned int l_i = Globals::g_windows.size() - 1;
    while (l_i && !Globals::g_windows[l_i]->isFullScreen())
        --l_i;
    // Draw windows
    for (std::vector<CWindow *>::iterator l_it = Globals::g_windows.begin() + l_i; l_it != Globals::g_windows.end(); ++l_it)
        (*l_it)->render(l_it + 1 == Globals::g_windows.end());
}

void SDL_utils::hastalavista(void)
{
    // Destroy all dialogs except the first one (the commander)
    while (Globals::g_windows.size() > 1)
        delete Globals::g_windows.back();
    // Free resources
    CResourceManager::instance().sdlCleanup();
    
#if defined(PLATFORM_MIYOOMINI) || defined(PLATFORM_RG35XX)
	SDL_FreeSurface(Globals::g_screen);
#endif

    // Quit SDL
    TTF_Quit();
    SDL_Quit();
}

void SDL_utils::pleaseWait(void)
{
    SDL_Surface *l_surfaceTmp = renderText(CResourceManager::instance().getFont(), "Please wait...", Globals::g_colorTextNormal);
    SDL_Rect l_rect;
    l_rect.x = (SCREEN_WIDTH - (l_surfaceTmp->w + 2 * DIALOG_MARGIN + 2 * DIALOG_BORDER)) >> 1;
    l_rect.y = (SCREEN_HEIGHT - (l_surfaceTmp->h + 9)) >> 1;
    l_rect.w = l_surfaceTmp->w + 2 * DIALOG_MARGIN + 2 * DIALOG_BORDER;
    l_rect.h = l_surfaceTmp->h + 9;
    SDL_FillRect(Globals::g_screen, &l_rect, SDL_MapRGB(Globals::g_screen->format, COLOR_BORDER));
    l_rect.x += DIALOG_BORDER;
    l_rect.y += DIALOG_BORDER;
    l_rect.w -= 2 * DIALOG_BORDER;
    l_rect.h -= 2 * DIALOG_BORDER;
    SDL_FillRect(Globals::g_screen, &l_rect, SDL_MapRGB(Globals::g_screen->format, COLOR_BG_1));
    applySurface((SCREEN_WIDTH - l_surfaceTmp->w) >> 1, (SCREEN_HEIGHT - l_surfaceTmp->h) >> 1, l_surfaceTmp, Globals::g_screen);
    SDL_FreeSurface(l_surfaceTmp);
#if defined(PLATFORM_MIYOOMINI) || defined(PLATFORM_RG35XX)
	upscale16NEON(Globals::g_screen->pixels, Globals::g_scaled->pixels);
	SDL_Flip(Globals::g_scaled);
#else
    SDL_Flip(Globals::g_screen);
#endif
}

#if defined(PLATFORM_MIYOOMINI) || defined(PLATFORM_RG35XX)
// eggs amazing SW integer scaler
void upscale16NEON(void* src, void* dst) {
	asm volatile (
	"	vmov.u64 d1,#0xFFFFFFFF	;"
	"	vmvn.u64 d0,#0xFFFFFFFF	;"
	"	add r2,%0,#(320*240*2)	;"
	"1:	add r3,%1,#(640*1*2)	;"
	"	add lr,%1,#(640*2*2)	;"
	"2:	vldmia %0!,{q8-q11}	;"
	"	vdup.16 d2,d23[3]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d23[2]	;"
	"	vand d3,d3,d1		;"
	"	vorr d31,d2,d3		;"
	"	vdup.16 d2,d23[1]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d23[0]	;"
	"	vand d3,d3,d1		;"
	"	vorr d30,d2,d3		;"
	"	vdup.16 d2,d22[3]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d22[2]	;"
	"	vand d3,d3,d1		;"
	"	vorr d29,d2,d3		;"
	"	vdup.16 d2,d22[1]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d22[0]	;"
	"	vand d3,d3,d1		;"
	"	vorr d28,d2,d3		;"
	"	vdup.16 d2,d21[3]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d21[2]	;"
	"	vand d3,d3,d1		;"
	"	vorr d27,d2,d3		;"
	"	vdup.16 d2,d21[1]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d21[0]	;"
	"	vand d3,d3,d1		;"
	"	vorr d26,d2,d3		;"
	"	vdup.16 d2,d20[3]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d20[2]	;"
	"	vand d3,d3,d1		;"
	"	vorr d25,d2,d3		;"
	"	vdup.16 d2,d20[1]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d20[0]	;"
	"	vand d3,d3,d1		;"
	"	vorr d24,d2,d3		;"
	"	vdup.16 d2,d19[3]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d19[2]	;"
	"	vand d3,d3,d1		;"
	"	vorr d23,d2,d3		;"
	"	vdup.16 d2,d19[1]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d19[0]	;"
	"	vand d3,d3,d1		;"
	"	vorr d22,d2,d3		;"
	"	vdup.16 d2,d18[3]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d18[2]	;"
	"	vand d3,d3,d1		;"
	"	vorr d21,d2,d3		;"
	"	vdup.16 d2,d18[1]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d18[0]	;"
	"	vand d3,d3,d1		;"
	"	vorr d20,d2,d3		;"
	"	vdup.16 d2,d17[3]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d17[2]	;"
	"	vand d3,d3,d1		;"
	"	vorr d19,d2,d3		;"
	"	vdup.16 d2,d17[1]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d17[0]	;"
	"	vand d3,d3,d1		;"
	"	vorr d18,d2,d3		;"
	"	vdup.16 d2,d16[3]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d16[2]	;"
	"	vand d3,d3,d1		;"
	"	vorr d17,d2,d3		;"
	"	vdup.16 d2,d16[1]	;"
	"	vand d2,d2,d0		;"
	"	vdup.16 d3,d16[0]	;"
	"	vand d3,d3,d1		;"
	"	vorr d16,d2,d3		;"
	"	vstmia %1!,{q8-q15}	;"
	"	vstmia r3!,{q8-q15}	;"
	"	cmp r3,lr		;"
	"	bne 2b			;"
	"	cmp %0,r2		;"
	"	mov %1,lr		;"
	"	bne 1b			"
	:: "r"(src), "r"(dst)
	: "r2","r3","lr","q0","q1","q8","q9","q10","q11","q12","q13","q14","q15","memory","cc"
	);
}
#endif