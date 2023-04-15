#ifndef _VIEWER_H_
#define _VIEWER_H_

#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "window.h"

#define VIEWER_LINE_HEIGHT   13
#define VIEWER_NB_LINES      17
#define VIEWER_Y_LIST        18
#define VIEWER_MARGIN        1
#define VIEWER_X_STEP        32
#define VIEWER_SIZE_MAX      16777216  // = 16 MB

class CViewer : public CWindow
{
    public:

    // Constructor
    CViewer(const std::string &p_fileName);

    // Destructor
    virtual ~CViewer(void);

    private:

    // Forbidden
    CViewer(void);
    CViewer(const CViewer &p_source);
    const CViewer &operator =(const CViewer &p_source);

    // Key press management
    virtual const bool keyPress(const SDL_Event &p_event);

    // Key hold management
    virtual const bool keyHold(void);

    // Draw
    virtual void render(const bool p_focus) const;

    // Is window full screen?
    virtual const bool isFullScreen(void) const;

    // Scroll
    const bool moveUp(const unsigned int p_step);
    const bool moveDown(const unsigned int p_step);
    const bool moveLeft(void);
    void moveRight(void);

    // The viewed file name
    std::string m_fileName;

    // Coordinates
    unsigned int m_firstLine;
    mutable SDL_Rect m_clip;

    // Background image
    SDL_Surface *m_image;

    // List of read lines
    std::vector<std::string> m_lines;

    // Pointers to resources
    TTF_Font *m_font;
};

#endif
