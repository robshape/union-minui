#include <iostream>
#include "dialog.h"
#include "sdlutils.h"
#include "resourceManager.h"
#include "def.h"

CDialog::CDialog(const std::string &p_title, const Sint16 p_x, const Sint16 p_y):
    CWindow(),
    m_nbTitle(false),
    m_nbLabels(0),
    m_nbOptions(0),
    m_highlightedLine(0),
    m_image(NULL),
    m_cursor1(NULL),
    m_cursor2(NULL),
    m_x(p_x),
    m_y(p_y),
    m_cursorX(0),
    m_cursorY(0),
    m_font(CResourceManager::instance().getFont()),
    m_font_sm(CResourceManager::instance().getSmallFont())
{
    // Title
    if (!p_title.empty())
    {
        m_nbTitle = true;
        m_lines.push_back(p_title);
    }
    // Init clip
    m_clip.x = 0;
    m_clip.y = 0;
    m_clip.w = 0;
    m_clip.h = 0;
}

CDialog::~CDialog(void)
{
    // Free surfaces
    if (m_image != NULL)
    {
        SDL_FreeSurface(m_image);
        m_image = NULL;
    }
    if (m_cursor1 != NULL)
    {
        SDL_FreeSurface(m_cursor1);
        m_cursor1 = NULL;
    }
    if (m_cursor2 != NULL)
    {
        SDL_FreeSurface(m_cursor2);
        m_cursor2 = NULL;
    }
    for (std::vector<SDL_Surface *>::iterator l_it = m_linesImg.begin(); l_it != m_linesImg.end(); ++l_it)
    {
        if (*l_it != NULL)
        {
            SDL_FreeSurface(*l_it);
            *l_it = NULL;
        }
    }
}

void CDialog::addLabel(const std::string &p_label)
{
    m_lines.push_back(p_label);
    ++m_nbLabels;
}

void CDialog::addOption(const std::string &p_option)
{
    m_lines.push_back(p_option);
    ++m_nbOptions;
}

void CDialog::init(void)
{
    // The width of the window depends on the width of the largest line
    int l_width(0);
    int l_cursorWidth(0);
    SDL_Surface *l_surfaceTmp(NULL);
    // Render every line
    for (std::vector<std::string>::const_iterator l_it = m_lines.begin(); l_it != m_lines.end(); ++l_it)
    {
        // Render line
        l_surfaceTmp = SDL_utils::renderText(m_font, *l_it, (m_nbTitle && l_it == m_lines.begin()) ? Globals::g_colorTextTitle : Globals::g_colorTextNormal);
        if (l_surfaceTmp->w > l_width)
            l_width = l_surfaceTmp->w;
        m_linesImg.push_back(l_surfaceTmp);
    }
    // Cursor width
    l_cursorWidth = l_width + 2 * DIALOG_MARGIN;
    if (l_cursorWidth > SCREEN_WIDTH - 2 * DIALOG_BORDER)
        l_cursorWidth = SCREEN_WIDTH - 2 * DIALOG_BORDER;
    // Line clip
    m_clip.h = m_linesImg.front()->h;
    m_clip.w = l_cursorWidth - DIALOG_MARGIN - 1;
    // Adjust image width
    l_width = l_width + 2 * DIALOG_MARGIN + 2 * DIALOG_BORDER;
    if (l_width > SCREEN_WIDTH)
        l_width = SCREEN_WIDTH;
    // Create dialog image
    m_image = SDL_utils::createImage(l_width, m_linesImg.size() * LINE_HEIGHT + 2 * DIALOG_BORDER, SDL_MapRGB(Globals::g_screen->format, COLOR_BORDER));
    {
        SDL_Rect l_rect;
        l_rect.x = DIALOG_BORDER;
        l_rect.y = DIALOG_BORDER + m_nbTitle * LINE_HEIGHT;
        l_rect.w = m_image->w - 2 * DIALOG_BORDER;
        l_rect.h = m_image->h - 2 * DIALOG_BORDER - m_nbTitle * LINE_HEIGHT;
        SDL_FillRect(m_image, &l_rect, SDL_MapRGB(m_image->format, COLOR_BG_1));
    }
    // Create cursor image
    m_cursor1 = SDL_utils::createImage(l_cursorWidth, LINE_HEIGHT, SDL_MapRGB(Globals::g_screen->format, COLOR_CURSOR_1));
    m_cursor2 = SDL_utils::createImage(l_cursorWidth, LINE_HEIGHT, SDL_MapRGB(Globals::g_screen->format, COLOR_CURSOR_2));
    // Adjust dialog coordinates
    if (!m_x)
        m_x = (SCREEN_WIDTH - m_image->w) >> 1;
    if (!m_y)
    {
        m_y = (SCREEN_HEIGHT - m_image->h) >> 1;
    }
    else
    {
        m_y = m_y - (m_image->h >> 1) + (LINE_HEIGHT >> 1);
        if (m_y < Y_LIST)
            m_y = Y_LIST;
        if (m_y + m_image->h > Y_FOOTER + 1)
            m_y = Y_FOOTER + 1 - m_image->h;
    }
    // Cursor coordinates
    m_cursorX = m_x + DIALOG_BORDER;
    m_cursorY = m_y + DIALOG_BORDER + (m_nbTitle + m_nbLabels) * LINE_HEIGHT;
}

void CDialog::render(const bool p_focus) const
{
    INHIBIT(std::cout << "CDialog::render  fullscreen: " << isFullScreen() << "  focus: " << p_focus << std::endl;)
    // Draw background
    SDL_utils::applySurface(m_x, m_y, m_image, Globals::g_screen);
    // Draw cursor
    SDL_utils::applySurface(m_cursorX, m_cursorY + m_highlightedLine * LINE_HEIGHT, p_focus ? m_cursor1 : m_cursor2, Globals::g_screen);
    // Draw lines text
    Sint16 l_y(m_y + 4);
    for (std::vector<SDL_Surface *>::const_iterator l_it = m_linesImg.begin(); l_it != m_linesImg.end(); ++l_it)
    {
        SDL_utils::applySurface(m_cursorX + DIALOG_MARGIN, (m_nbTitle && l_it == m_linesImg.begin()) ? l_y - 1 - Y_OFFSET : l_y - Y_OFFSET, *l_it, Globals::g_screen, &m_clip);
        l_y += LINE_HEIGHT;
    }
}

const bool CDialog::keyPress(const SDL_Event &p_event)
{
    CWindow::keyPress(p_event);
    bool l_ret(false);
	switch (p_event.key.keysym.sym)
    {
        case MYKEY_PARENT:
            m_retVal = -1;
            l_ret = true;
            break;
        case MYKEY_UP:
            l_ret = moveCursorUp(true);
            break;
        case MYKEY_DOWN:
            l_ret = moveCursorDown(true);
            break;
        case MYKEY_PAGEUP:
            if (m_highlightedLine)
            {
                m_highlightedLine = 0;
                l_ret = true;
            }
            break;
        case MYKEY_PAGEDOWN:
            if (m_highlightedLine + 1 < m_nbOptions)
            {
                m_highlightedLine = m_nbOptions - 1;
                l_ret = true;
            }
            break;
        case MYKEY_OPEN:
            m_retVal = m_highlightedLine + 1;
            l_ret = true;
            break;
        default:
            break;
    }
    return l_ret;
}

const bool CDialog::moveCursorUp(const bool p_loop)
{
    bool l_ret(false);
    if (m_highlightedLine)
    {
        --m_highlightedLine;
        l_ret = true;
    }
    else if (p_loop && m_highlightedLine + 1 < m_nbOptions)
    {
        m_highlightedLine = m_nbOptions - 1;
        l_ret = true;
    }
    return l_ret;
}

const bool CDialog::moveCursorDown(const bool p_loop)
{
    bool l_ret(false);
    if (m_highlightedLine + 1 < m_nbOptions)
    {
        ++m_highlightedLine;
        l_ret = true;
    }
    else if (p_loop && m_highlightedLine)
    {
        m_highlightedLine = 0;
        l_ret = true;
    }
    return l_ret;
}

const bool CDialog::keyHold(void)
{
    bool l_ret(false);
    switch(m_lastPressed)
    {
        case MYKEY_UP:
            if (tick(SDL_GetKeyState(NULL)[MYKEY_UP]))
                l_ret = moveCursorUp(false);
            break;
        case MYKEY_DOWN:
            if (tick(SDL_GetKeyState(NULL)[MYKEY_DOWN]))
                l_ret = moveCursorDown(false);
            break;
        default:
            break;
    }
    return l_ret;
}

const Sint16 &CDialog::getX(void) const
{
    return m_x;
}

const Sint16 &CDialog::getY(void) const
{
    return m_y;
}

const SDL_Surface * const CDialog::getImage(void) const
{
    return m_image;
}

const unsigned int &CDialog::getHighlightedIndex(void) const
{
    return m_highlightedLine;
}
