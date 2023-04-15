#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define NB_SURFACES 6

class CResourceManager
{
    public:

    typedef enum
    {
        T_SURFACE_BG = 0,
        T_SURFACE_FILE,
        T_SURFACE_FOLDER,
        T_SURFACE_UP,
        T_SURFACE_CURSOR1,
        T_SURFACE_CURSOR2
    }
    T_SURFACE;

    // Method to get the instance
    static CResourceManager& instance(void);

    // Cleanup all resources
    void sdlCleanup(void);

    // Get a loaded surface
    SDL_Surface *getSurface(const T_SURFACE p_surface) const;

    // Get the loaded fonts
    TTF_Font *getFont(void) const;
    TTF_Font *getSmallFont(void) const;

    private:

    // Forbidden
    CResourceManager(void);
    CResourceManager(const CResourceManager &p_source);
    const CResourceManager &operator =(const CResourceManager &p_source);

    // Images
    SDL_Surface *m_surfaces[NB_SURFACES];

    // Fonts
    TTF_Font *m_font;
    TTF_Font *m_font_sm;
};

#endif
