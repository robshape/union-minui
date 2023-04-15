#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <SDL/SDL.h>

class CWindow
{
    public:

    // Destructor
    virtual ~CWindow(void);

    // Execute main loop of the window
    const int execute(void);

    // Return value
    const int getReturnValue(void) const;

    // Draw window
    virtual void render(const bool p_focus) const = 0;

    // Is window full screen?
    virtual const bool isFullScreen(void) const;

    protected:

    // Constructor
    CWindow(void);

    // Key press management
    virtual const bool keyPress(const SDL_Event &p_event);

    // Key hold management
    virtual const bool keyHold(void);

    // Timer tick
    const bool tick(const Uint8 p_held);

    // Timer for key hold
    unsigned int m_timer;
#if defined(PLATFORM_RG35XX)
    uint8_t m_lastPressed;
#else
    SDLKey m_lastPressed;
#endif
    
    // Return value
    int m_retVal;

    private:

    // Forbidden
    CWindow(const CWindow &p_source);
    const CWindow &operator =(const CWindow &p_source);

};

#endif
