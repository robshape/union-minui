#ifndef _COMMANDER_H_
#define _COMMANDER_H_

#include <SDL/SDL.h>
#include "panel.h"
#include "window.h"

class CCommander : public CWindow
{
    public:

    // Constructor
    CCommander(const std::string &p_pathL, const std::string &p_pathR);

    // Destructor
    virtual ~CCommander(void);

    private:

    // Forbidden
    CCommander(void);
    CCommander(const CCommander &p_source);
    const CCommander &operator =(const CCommander &p_source);

    // Key press management
    virtual const bool keyPress(const SDL_Event &p_event);

    // Key hold management
    virtual const bool keyHold(void);

    // Draw
    virtual void render(const bool p_focus) const;

    // Is window full screen?
    virtual const bool isFullScreen(void) const;

    // Open the file operation menus
    const bool openCopyMenu(void) const;
    void openExecuteMenu(void) const;

    // Open the selection menu
    const bool openSystemMenu(void);

    // The two panels
    CPanel m_panelLeft;
    CPanel m_panelRight;
    CPanel* m_panelSource;
    CPanel* m_panelTarget;

    // Pointers to resources
    SDL_Surface *m_background;
};

#endif
