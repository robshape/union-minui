#include <iostream>
#include <fstream>
#include "viewer.h"
#include "resourceManager.h"
#include "def.h"
#include "sdlutils.h"

CViewer::CViewer(const std::string &p_fileName):
    CWindow(),
    m_fileName(p_fileName),
    m_firstLine(0),
    m_image(NULL),
    m_font(CResourceManager::instance().getFont())
{
    // Init clip rect
    m_clip.x = 0;
    m_clip.y = 0;
    m_clip.w = SCREEN_WIDTH - 2 * VIEWER_MARGIN;
    // Create background image
    m_image = SDL_utils::createImage(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_MapRGB(Globals::g_screen->format, COLOR_BG_1));
    {
        SDL_Rect l_rect;
        l_rect.x = 0;
        l_rect.y = 0;
        l_rect.w = SCREEN_WIDTH;
        l_rect.h = Y_LIST;
        SDL_FillRect(m_image, &l_rect, SDL_MapRGB(m_image->format, COLOR_BORDER));
    }
    // Print title
    SDL_Surface *l_surfaceTmp = SDL_utils::renderText(m_font, m_fileName, Globals::g_colorTextTitle);
    if (l_surfaceTmp->w > m_image->w - 2 * VIEWER_MARGIN)
    {
        SDL_Rect l_rect;
        l_rect.x = l_surfaceTmp->w - (m_image->w - 2 * VIEWER_MARGIN);
        l_rect.y = 0;
        l_rect.w = m_image->w - 2 * VIEWER_MARGIN;
        l_rect.h = l_surfaceTmp->h;
        SDL_utils::applySurface(VIEWER_MARGIN, Y_HEADER, l_surfaceTmp, m_image, &l_rect);
    }
    else
    {
        SDL_utils::applySurface(VIEWER_MARGIN, Y_HEADER, l_surfaceTmp, m_image);
    }
    m_clip.h = l_surfaceTmp->h;
    SDL_FreeSurface(l_surfaceTmp);
    // Read file
    std::ifstream l_file(m_fileName.c_str());
    if (l_file.is_open())
    {
        std::string l_line("");
        while (std::getline(l_file, l_line))
        {
            m_lines.push_back(l_line);
        }
        l_file.close();
    }
    else
        std::cerr << "Error: unable to open file " << m_fileName << std::endl;
    INHIBIT(std::cout << "CViewer: " << m_lines.size() << " lines read" << std::endl;)
}

CViewer::~CViewer(void)
{
    // Free surfaces
    if (m_image != NULL)
    {
        SDL_FreeSurface(m_image);
        m_image = NULL;
    }
}

void CViewer::render(const bool p_focus) const
{
    INHIBIT(std::cout << "CViewer::render  fullscreen: " << isFullScreen() << "  focus: " << p_focus << std::endl;)
    // Draw background
    SDL_utils::applySurface(0, 0, m_image, Globals::g_screen);
    // Draw lines
    SDL_Surface *l_surfaceTmp(NULL);
    unsigned int l_i(0);
    std::vector<std::string>::const_iterator l_it = m_lines.begin() + m_firstLine;
    while (l_it != m_lines.end() && l_i < VIEWER_NB_LINES)
    {
        if (!l_it->empty())
        {
            l_surfaceTmp = SDL_utils::renderText(m_font, *l_it, Globals::g_colorTextNormal);
            SDL_utils::applySurface(VIEWER_MARGIN, VIEWER_Y_LIST + l_i * VIEWER_LINE_HEIGHT - Y_OFFSET, l_surfaceTmp, Globals::g_screen, &m_clip);
            SDL_FreeSurface(l_surfaceTmp);
        }
        // Next line
        ++l_it;
        ++l_i;
    }
}

const bool CViewer::keyPress(const SDL_Event &p_event)
{
    CWindow::keyPress(p_event);
    bool l_ret(false);
	switch (p_event.key.keysym.sym)
    {
        case MYKEY_PARENT:
        case MYKEY_MENU:
            m_retVal = -1;
            l_ret = true;
            break;
        case MYKEY_UP:
            l_ret = moveUp(1);
            break;
        case MYKEY_DOWN:
            l_ret = moveDown(1);
            break;
        case MYKEY_PAGEUP:
            l_ret = moveUp(VIEWER_NB_LINES - 1);
            break;
        case MYKEY_PAGEDOWN:
            l_ret = moveDown(VIEWER_NB_LINES - 1);
            break;
        case MYKEY_LEFT:
            l_ret = moveLeft();
            break;
        case MYKEY_RIGHT:
            moveRight();
            l_ret = true;
            break;
        default:
            break;
    }
    return l_ret;
}

const bool CViewer::keyHold(void)
{
    bool l_ret(false);
    switch(m_lastPressed)
    {
        case MYKEY_UP:
            if (tick(SDL_GetKeyState(NULL)[MYKEY_UP]))
                l_ret = moveUp(1);
            break;
        case MYKEY_DOWN:
            if (tick(SDL_GetKeyState(NULL)[MYKEY_DOWN]))
                l_ret = moveDown(1);
            break;
        case MYKEY_PAGEUP:
            if (tick(SDL_GetKeyState(NULL)[MYKEY_PAGEUP]))
                l_ret = moveUp(VIEWER_NB_LINES - 1);
            break;
        case MYKEY_PAGEDOWN:
            if (tick(SDL_GetKeyState(NULL)[MYKEY_PAGEDOWN]))
                l_ret = moveDown(VIEWER_NB_LINES - 1);
            break;
        case MYKEY_LEFT:
            if (tick(SDL_GetKeyState(NULL)[MYKEY_LEFT]))
                l_ret = moveLeft();
            break;
        case MYKEY_RIGHT:
            if (tick(SDL_GetKeyState(NULL)[MYKEY_RIGHT]))
            {
                moveRight();
                l_ret = true;
            }
            break;
        default:
            break;
    }
    return l_ret;
}

const bool CViewer::isFullScreen(void) const
{
    return true;
}

const bool CViewer::moveUp(const unsigned int p_step)
{
    bool l_ret(false);
    if (m_firstLine)
    {
        if (m_firstLine > p_step)
            m_firstLine -= p_step;
        else
            m_firstLine = 0;
        l_ret = true;
    }
    return l_ret;
}

const bool CViewer::moveDown(const unsigned int p_step)
{
    bool l_ret(false);
    if (m_firstLine + VIEWER_NB_LINES + 1 < m_lines.size())
    {
        if (m_firstLine + VIEWER_NB_LINES + 1 + p_step > m_lines.size())
            m_firstLine = m_lines.size() - VIEWER_NB_LINES - 1;
        else
            m_firstLine += p_step;
        l_ret = true;
    }
    return l_ret;
}

const bool CViewer::moveLeft(void)
{
    bool l_ret(false);
    if (m_clip.x > 0)
    {
        if (m_clip.x > VIEWER_X_STEP)
            m_clip.x -= VIEWER_X_STEP;
        else
            m_clip.x = 0;
        l_ret = true;
    }
    return l_ret;
}

void CViewer::moveRight(void)
{
    m_clip.x += VIEWER_X_STEP;
}
