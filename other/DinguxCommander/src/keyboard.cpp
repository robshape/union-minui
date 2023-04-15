#include <iostream>
#include "keyboard.h"
#include "sdlutils.h"
#include "resourceManager.h"
#include "def.h"

#define KB_X           28
#define KB_Y           128
#define FIELD_Y        98
#define FIELD_W        258

CKeyboard::CKeyboard(const std::string &p_inputText):
    CWindow(),
    m_imageKeyboard(NULL),
    m_textField(NULL),
    m_inputText(p_inputText),
    m_selected(0),
    m_footer(NULL),
    m_keySet(0),
    m_font(CResourceManager::instance().getFont()),
    m_font_sm(CResourceManager::instance().getSmallFont())
{
    // Key sets
    m_keySets[0] = "abcdefghijklmnopqrstuvwxyz0123456789., ";
    m_keySets[1] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789., ";
    m_keySets[2] = "!#$%&'()[]{}+-;=@^_`´~¡¿¢£¤¥¦§«»±×÷©®° ";
    m_keySets[3] = "áàäâãåéèëêæœçíìïîóòöôõðøñþúùüûýÿøßµ¹²³ ";
    m_keySets[4] = "ÁÀÄÂÃÅÉÈËÊÆŒÇÍÌÏÎÓÒÖÔÕÐØÑÞÚÙÜÛÝŸØßµ¼½¾ ";
    // Create keyboard image
    {
        SDL_Rect l_rect;
        // Create keyboard image
        m_imageKeyboard = SDL_utils::createImage(265, 84, SDL_MapRGB(Globals::g_screen->format, COLOR_BORDER));
        l_rect.x = 2;
        l_rect.y = 2;
        l_rect.w = 261;
        l_rect.h = 80;
        SDL_FillRect(m_imageKeyboard, &l_rect, SDL_MapRGB(m_imageKeyboard->format, COLOR_BG_2));
        // Keys
        for (unsigned int l_y = 0; l_y < 3; ++l_y)
        {
            for (unsigned int l_x = 0; l_x < 13; ++l_x)
            {
                l_rect.x = 3 + 20 * l_x;
                l_rect.y = 3 + 20 * l_y;
                l_rect.w = 19;
                l_rect.h = 18;
                SDL_FillRect(m_imageKeyboard, &l_rect, SDL_MapRGB(m_imageKeyboard->format, COLOR_BORDER));
                ++l_rect.x;
                ++l_rect.y;
                l_rect.w -= 2;
                l_rect.h -= 2;
                SDL_FillRect(m_imageKeyboard, &l_rect, SDL_MapRGB(m_imageKeyboard->format, COLOR_BG_1));
            }
        }
        // Buttons Cancel and OK
        l_rect.x = 3;
        l_rect.y = 63;
        l_rect.w = 129;
        l_rect.h = 18;
        SDL_FillRect(m_imageKeyboard, &l_rect, SDL_MapRGB(m_imageKeyboard->format, COLOR_BORDER));
        l_rect.x = 133;
        SDL_FillRect(m_imageKeyboard, &l_rect, SDL_MapRGB(m_imageKeyboard->format, COLOR_BORDER));
        l_rect.w -= 2;
        l_rect.h -= 2;
        ++l_rect.y;
        l_rect.x = 4;
        SDL_FillRect(m_imageKeyboard, &l_rect, SDL_MapRGB(m_imageKeyboard->format, COLOR_BG_1));
        l_rect.x = 134;
        SDL_FillRect(m_imageKeyboard, &l_rect, SDL_MapRGB(m_imageKeyboard->format, COLOR_BG_1));
        // Create text field image
        m_textField = SDL_utils::createImage(265, 19, SDL_MapRGB(Globals::g_screen->format, COLOR_BORDER));
        l_rect.x = 2;
        l_rect.y = 2;
        l_rect.w = 261;
        l_rect.h = 15;
        SDL_FillRect(m_textField, &l_rect, SDL_MapRGB(m_imageKeyboard->format, COLOR_BG_1));
    }
    // Create footer
    m_footer = SDL_utils::createImage(SCREEN_WIDTH, H_FOOTER, SDL_MapRGB(Globals::g_screen->format, COLOR_BORDER));
    SDL_utils::applyText(SCREEN_WIDTH >> 1, 1, m_footer, m_font_sm, "A-Input   B-Cancel   START-OK   L/R-Change   Y-Backspace   X-Space", Globals::g_colorTextTitle, SDL_utils::T_TEXT_ALIGN_CENTER);
}

CKeyboard::~CKeyboard(void)
{
    // Free surfaces
    if (m_imageKeyboard != NULL)
    {
        SDL_FreeSurface(m_imageKeyboard);
        m_imageKeyboard = NULL;
    }
    if (m_textField != NULL)
    {
        SDL_FreeSurface(m_textField);
        m_textField = NULL;
    }
    if (m_footer != NULL)
    {
        SDL_FreeSurface(m_footer);
        m_footer = NULL;
    }
}

void CKeyboard::render(const bool p_focus) const
{
    INHIBIT(std::cout << "CKeyboard::render  fullscreen: " << isFullScreen() << "  focus: " << p_focus << std::endl;)
    // Draw input text field
    SDL_utils::applySurface(KB_X, FIELD_Y, m_textField, Globals::g_screen);
    // Input text
    if (!m_inputText.empty())
    {
        SDL_Surface *l_surfaceTmp = SDL_utils::renderText(m_font, m_inputText, Globals::g_colorTextNormal);
        if (l_surfaceTmp->w > FIELD_W)
        {
            // Text is too big => clip it
            SDL_Rect l_rect;
            l_rect.x = l_surfaceTmp->w - FIELD_W;
            l_rect.y = 0;
            l_rect.w = FIELD_W;
            l_rect.h = l_surfaceTmp->h;
            SDL_utils::applySurface(KB_X + 5, FIELD_Y + 4 - Y_OFFSET, l_surfaceTmp, Globals::g_screen, &l_rect);
        }
        else
            SDL_utils::applySurface(KB_X + 5, FIELD_Y + 4 - Y_OFFSET, l_surfaceTmp, Globals::g_screen);
    }
    // Draw keyboard
    SDL_utils::applySurface(KB_X, KB_Y, m_imageKeyboard, Globals::g_screen);
    // Cursor
    {
        SDL_Rect l_rect;
        if (m_selected < 39)
        {
            // A letter is selected
            l_rect.w = 17;
            l_rect.h = 16;
            l_rect.x = KB_X + 4;
            if (m_selected >= 26)
            {
                l_rect.x = KB_X + 4 + (m_selected - 26) * 20;
                l_rect.y = KB_Y + 44;
            }
            else if (m_selected >= 13)
            {
                l_rect.x = KB_X + 4 + (m_selected - 13) * 20;
                l_rect.y = KB_Y + 24;
            }
            else
            {
                l_rect.x = KB_X + 4 + m_selected * 20;
                l_rect.y = KB_Y + 4;
            }
        }
        else
        {
            l_rect.w = 127;
            l_rect.h = 16;
            l_rect.x = KB_X + 4 + (m_selected == 40) * 130;
            l_rect.y = KB_Y + 64;
        }
        SDL_FillRect(Globals::g_screen, &l_rect, SDL_MapRGB(Globals::g_screen->format, COLOR_CURSOR_1));
    }
    // Draw keys text
    {
        unsigned int l_i(0);
        unsigned int l_x(0);
        unsigned int l_y(0);
        std::string l_text("");
        for (l_y = 0; l_y < 3; ++l_y)
        {
            for (l_x = 0; l_x < 13; ++l_x)
            {
                if (utf8Code(m_keySets[m_keySet].at(l_i)))
                {
                    l_text = m_keySets[m_keySet].substr(l_i, 2);
                    l_i += 2;
                }
                else
                {
                    l_text = m_keySets[m_keySet].substr(l_i, 1);
                    l_i += 1;
                }
                SDL_utils::applyText(KB_X + 20 * l_x + 13, KB_Y + 7 + 20 * l_y - Y_OFFSET, Globals::g_screen, m_font, l_text, Globals::g_colorTextNormal, SDL_utils::T_TEXT_ALIGN_CENTER);
            }
        }
    }
    // Buttons text
    SDL_utils::applyText(KB_X + 67, KB_Y + 67 - Y_OFFSET, Globals::g_screen, m_font, "Cancel", Globals::g_colorTextNormal, SDL_utils::T_TEXT_ALIGN_CENTER);
    SDL_utils::applyText(KB_X + 197, KB_Y + 67 - Y_OFFSET, Globals::g_screen, m_font, "OK", Globals::g_colorTextNormal, SDL_utils::T_TEXT_ALIGN_CENTER);
    // Draw footer
    SDL_utils::applySurface(0, 227, m_footer, Globals::g_screen);
}

const bool CKeyboard::keyPress(const SDL_Event &p_event)
{
    CWindow::keyPress(p_event);
    bool l_ret(false);
	switch (p_event.key.keysym.sym)
    {
        case MYKEY_PARENT:
            // B => Cancel
            m_retVal = -1;
            l_ret = true;
            break;
        case MYKEY_UP:
            l_ret = moveCursorUp(true);
            break;
        case MYKEY_DOWN:
            l_ret = moveCursorDown(true);
            break;
        case MYKEY_LEFT:
            l_ret = moveCursorLeft(true);
            break;
        case MYKEY_RIGHT:
            l_ret = moveCursorRight(true);
            break;
        case MYKEY_SYSTEM:
            // Y => Backspace
            l_ret = backspace();
            break;
        case MYKEY_OPERATION:
            // X => Space
            l_ret = type(" ");
            break;
        case MYKEY_OPEN:
            // A => Button pressed
            if (m_selected == 39)
            {
                // Button Cancel
                m_retVal = -1;
                l_ret = true;
            }
            else if (m_selected == 40)
            {
                // Button OK
                m_retVal = 1;
                l_ret = true;
            }
            else
                // A letter button
                l_ret = type();
            break;
        case MYKEY_PAGEDOWN:
            // R => Change keys forward
            m_keySet = (m_keySet + 1) % NB_KEY_SETS;
            l_ret = true;
            break;
        case MYKEY_PAGEUP:
            // L => Change keys backward
            m_keySet = m_keySet ? m_keySet - 1 : NB_KEY_SETS - 1;
            l_ret = true;
            break;
        case MYKEY_TRANSFER:
            // START => OK
            m_retVal = 1;
            l_ret = true;
        default:
            break;
    }
    return l_ret;
}

const bool CKeyboard::keyHold(void)
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
        case MYKEY_LEFT:
            if (tick(SDL_GetKeyState(NULL)[MYKEY_LEFT]))
                l_ret = moveCursorLeft(false);
            break;
        case MYKEY_RIGHT:
            if (tick(SDL_GetKeyState(NULL)[MYKEY_RIGHT]))
                l_ret = moveCursorRight(false);
            break;
        case MYKEY_OPEN:
            // A => Add letter
            if (tick(SDL_GetKeyState(NULL)[MYKEY_OPEN]))
                l_ret = type();
            break;
        case MYKEY_SYSTEM:
            // Y => Backspace
            if (tick(SDL_GetKeyState(NULL)[MYKEY_SYSTEM]))
                l_ret = backspace();
            break;
        case MYKEY_OPERATION:
            // X => Space
            if (tick(SDL_GetKeyState(NULL)[MYKEY_OPERATION]))
                l_ret = type(" ");
            break;
        default:
            break;
    }
    return l_ret;
}

const bool CKeyboard::moveCursorUp(const bool p_loop)
{
    bool l_ret(false);
    if (m_selected == 39)
    {
        m_selected = 29;
        l_ret = true;
    }
    else if (m_selected == 40)
    {
        m_selected = 35;
        l_ret = true;
    }
    else if (m_selected >= 13)
    {
        m_selected -= 13;
        l_ret = true;
    }
    else if (m_selected >= 6)
    {
        if (p_loop)
        {
            m_selected = 40;
            l_ret = true;
        }
    }
    else
    {
        if (p_loop)
        {
            m_selected = 39;
            l_ret = true;
        }
    }
    return l_ret;
}

const bool CKeyboard::moveCursorDown(const bool p_loop)
{
    bool l_ret(false);
    if (m_selected == 39)
    {
        if (p_loop)
        {
            m_selected = 3;
            l_ret = true;
        }
    }
    else if (m_selected == 40)
    {
        if (p_loop)
        {
            m_selected = 9;
            l_ret = true;
        }
    }
    else if (m_selected >= 32)
    {
        m_selected = 40;
        l_ret = true;
    }
    else if (m_selected >= 26)
    {
        m_selected = 39;
        l_ret = true;
    }
    else
    {
        m_selected += 13;
        l_ret = true;
    }
    return l_ret;
}

const bool CKeyboard::moveCursorLeft(const bool p_loop)
{
    bool l_ret(false);
    switch (m_selected)
    {
        case 0:
        case 13:
        case 26:
            if (p_loop)
            {
                m_selected += 12;
                l_ret = true;
            }
            break;
        case 39:
            if (p_loop)
            {
                m_selected = 40;
                l_ret = true;
            }
            break;
        default:
            --m_selected;
            l_ret = true;
            break;
    }
    return l_ret;
}

const bool CKeyboard::moveCursorRight(const bool p_loop)
{
    bool l_ret(false);
    switch (m_selected)
    {
        case 12:
        case 25:
        case 38:
            if (p_loop)
            {
                m_selected -= 12;
                l_ret = true;
            }
            break;
        case 40:
            if (p_loop)
            {
                m_selected = 39;
                l_ret = true;
            }
            break;
        default:
            ++m_selected;
            l_ret = true;
            break;
    }
    return l_ret;
}

const bool CKeyboard::type(const std::string &p_text)
{
    if (p_text.empty())
    {
        // Append selected character to the input text
        if (m_selected < 39)
        {
            // Get real index
            unsigned char l_index(0);
            for (unsigned char l_c = 0; l_c < m_selected; ++l_c)
                l_index += 1 + utf8Code(m_keySets[m_keySet].at(l_index));
            // Get size
            size_t l_size = 1 + utf8Code(m_keySets[m_keySet].at(l_index));
            // Append text
            m_inputText += m_keySets[m_keySet].substr(l_index, l_size);
        }
        else
            std::cerr << "CKeyboard::type : unexpected value: " << m_selected << std::endl;
    }
    else
        // Append given text
        m_inputText += p_text;
    return true;
}

const std::string &CKeyboard::getInputText(void) const
{
    return m_inputText;
}

const bool CKeyboard::backspace(void)
{
    bool l_ret(false);
    if (!m_inputText.empty())
    {
        if (m_inputText.size() >= 2 && utf8Code(m_inputText.at(m_inputText.size() - 2)))
            m_inputText.resize(m_inputText.size() - 2);
        else
            m_inputText.resize(m_inputText.size() - 1);
        l_ret = true;
    }
    return l_ret;
}

const bool CKeyboard::utf8Code(const unsigned char p_c) const
{
    return (p_c >= 194 && p_c <= 198) || p_c == 208 || p_c == 209;
}
