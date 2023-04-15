#include <iostream>
#include <sstream>
#include "panel.h"
#include "resourceManager.h"
#include "sdlutils.h"
#include "fileutils.h"

#define PANEL_SIZE         158
#define NAME_SIZE          140

CPanel::CPanel(const std::string &p_path, const Sint16 p_x):
    m_currentPath(""),
    m_camera(0),
    m_x(p_x),
    m_highlightedLine(0),
    m_iconDir(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_FOLDER)),
    m_iconFile(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_FILE)),
    m_iconUp(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_UP)),
    m_cursor1(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_CURSOR1)),
    m_cursor2(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_CURSOR2)),
    m_font(CResourceManager::instance().getFont())
{
    // List the given path
    if (m_fileLister.list(p_path))
    {
        // Path OK
        m_currentPath = p_path;
    }
    else
    {
        // The path is wrong => take default
        m_fileLister.list(PATH_DEFAULT);
        m_currentPath = PATH_DEFAULT;
    }
}

CPanel::~CPanel(void)
{
}

void CPanel::render(const bool p_active) const
{
    // Draw cursor
    SDL_utils::applySurface(m_x - 1, Y_LIST + (m_highlightedLine - m_camera) * LINE_HEIGHT, p_active ? m_cursor1 : m_cursor2, Globals::g_screen);
    // Draw panel
    const Sint16 l_x = m_x + m_iconDir->w + 2;
    const unsigned int l_nbTotal = m_fileLister.getNbTotal();
    Sint16 l_y = Y_LIST;
    SDL_Surface *l_surfaceTmp = NULL;
    const SDL_Color *l_color = NULL;
    SDL_Rect l_rect;
    // Current dir
    l_surfaceTmp = SDL_utils::renderText(m_font, m_currentPath, Globals::g_colorTextTitle);
    if (l_surfaceTmp->w > PANEL_SIZE)
    {
        l_rect.x = l_surfaceTmp->w - PANEL_SIZE;
        l_rect.y = 0;
        l_rect.w = PANEL_SIZE;
        l_rect.h = l_surfaceTmp->h;
        SDL_utils::applySurface(m_x, Y_HEADER - Y_OFFSET, l_surfaceTmp, Globals::g_screen, &l_rect);
    }
    else
    {
        SDL_utils::applySurface(m_x, Y_HEADER - Y_OFFSET, l_surfaceTmp, Globals::g_screen);
    }
    SDL_FreeSurface(l_surfaceTmp);
    // Content
    for (unsigned int l_i = m_camera; l_i < m_camera + NB_VISIBLE_LINES && l_i < l_nbTotal; ++l_i)
    {
        // Icon and color
        if (m_fileLister.isDirectory(l_i))
        {
            // Icon
            if (m_fileLister[l_i].m_name == "..")
                l_surfaceTmp = m_iconUp;
            else
                l_surfaceTmp = m_iconDir;
            // Color
            if (m_selectList.find(l_i) != m_selectList.end())
                l_color = &Globals::g_colorTextSelected;
            else
                l_color = &Globals::g_colorTextDir;
        }
        else
        {
            // Icon
            l_surfaceTmp = m_iconFile;
            // Color
            if (m_selectList.find(l_i) != m_selectList.end())
                l_color = &Globals::g_colorTextSelected;
            else
                l_color = &Globals::g_colorTextNormal;
        }
        SDL_utils::applySurface(m_x, l_y, l_surfaceTmp, Globals::g_screen);
        // Text
        l_surfaceTmp = SDL_utils::renderText(m_font, m_fileLister[l_i].m_name, *l_color);
        if (l_surfaceTmp->w > NAME_SIZE)
        {
            l_rect.x = 0;
            l_rect.y = 0;
            l_rect.w = NAME_SIZE;
            l_rect.h = l_surfaceTmp->h;
            SDL_utils::applySurface(l_x, l_y + 2 - Y_OFFSET, l_surfaceTmp, Globals::g_screen, &l_rect);
        }
        else
        {
            SDL_utils::applySurface(l_x, l_y + 2 - Y_OFFSET, l_surfaceTmp, Globals::g_screen);
        }
        SDL_FreeSurface(l_surfaceTmp);
        // Next line
        l_y += LINE_HEIGHT;
    }
    // Footer
    std::string l_footer("-");
    if (!m_fileLister.isDirectory(m_highlightedLine))
    {
        std::ostringstream l_s;
        l_s << m_fileLister[m_highlightedLine].m_size;
        l_footer = l_s.str();
        File_utils::formatSize(l_footer);
    }
    SDL_utils::applyText(m_x + 2, Y_FOOTER - Y_OFFSET, Globals::g_screen, m_font, "Size:", Globals::g_colorTextTitle);
    SDL_utils::applyText(m_x + PANEL_SIZE - 2, Y_FOOTER - Y_OFFSET, Globals::g_screen, m_font, l_footer, Globals::g_colorTextTitle, SDL_utils::T_TEXT_ALIGN_RIGHT);
}

const bool CPanel::moveCursorUp(unsigned char p_step)
{
    bool l_ret(false);
    if (m_highlightedLine)
    {
        // Move cursor
        if (m_highlightedLine > p_step)
            m_highlightedLine -= p_step;
        else
            m_highlightedLine = 0;
        // Adjust camera
        adjustCamera();
        // Return true for new render
        l_ret = true;
    }
    return l_ret;
}

const bool CPanel::moveCursorDown(unsigned char p_step)
{
    bool l_ret(false);
    const unsigned int l_nb = m_fileLister.getNbTotal();
    if (m_highlightedLine < l_nb - 1)
    {
        // Move cursor
        if (m_highlightedLine + p_step > l_nb - 1)
            m_highlightedLine = l_nb - 1;
        else
            m_highlightedLine += p_step;
        // Adjust camera
        adjustCamera();
        // Return true for new render
        l_ret = true;
    }
    return l_ret;
}

const bool CPanel::open(const std::string &p_path)
{
    bool l_ret(false);
    std::string l_newPath("");
    std::string l_oldDir("");
    if (p_path.empty())
    {
        // Open highlighted dir
        if (m_fileLister[m_highlightedLine].m_name == "..")
        {
            // Go to parent dir
            size_t l_pos = m_currentPath.rfind('/');
            // Remove the last dir in the path
            l_newPath = m_currentPath.substr(0, l_pos);
            if (l_newPath.empty())
                // We're at /
                l_newPath = "/";
            l_oldDir = m_currentPath.substr(l_pos + 1);
        }
        else
        {
            l_newPath = m_currentPath + (m_currentPath == "/" ? "" : "/") + m_fileLister[m_highlightedLine].m_name;
        }
    }
    else
    {
        // Open given dir
        if (p_path == m_currentPath)
            return false;
        l_newPath = p_path;
    }
    // List the new path
    if (m_fileLister.list(l_newPath))
    {
        // Path OK
        m_currentPath = l_newPath;
        // If it's a back movement, restore old dir
        if (!l_oldDir.empty())
            m_highlightedLine = m_fileLister.searchDir(l_oldDir);
        else
            m_highlightedLine = 0;
        // Camera
        adjustCamera();
        // Clear select list
        m_selectList.clear();
        // New render
        l_ret = true;
    }
    INHIBIT(std::cout << "open - new current path: " << m_currentPath << std::endl;)
    return l_ret;
}

const bool CPanel::goToParentDir(void)
{
    bool l_ret(false);
    // Select ".." and open it
    if (m_currentPath != "/")
    {
        m_highlightedLine = 0;
        l_ret = open();
    }
    return l_ret;
}

void CPanel::adjustCamera(void)
{
    if (m_fileLister.getNbTotal() <= NB_VISIBLE_LINES)
        m_camera = 0;
    else if (m_highlightedLine < m_camera)
        m_camera = m_highlightedLine;
    else if (m_highlightedLine > m_camera + NB_VISIBLE_LINES - 1)
        m_camera = m_highlightedLine - NB_VISIBLE_LINES + 1;
}

const std::string &CPanel::getHighlightedItem(void) const
{
    return m_fileLister[m_highlightedLine].m_name;
}

const std::string CPanel::getHighlightedItemFull(void) const
{
    return m_currentPath + (m_currentPath == "/" ? "" : "/") + m_fileLister[m_highlightedLine].m_name;
}

const std::string &CPanel::getCurrentPath(void) const
{
    return m_currentPath;
}

const unsigned int &CPanel::getHighlightedIndex(void) const
{
    return m_highlightedLine;
}

const unsigned int CPanel::getHighlightedIndexRelative(void) const
{
    return m_highlightedLine - m_camera;
}

void CPanel::refresh(void)
{
    // List current path
    if (m_fileLister.list(m_currentPath))
    {
        // Adjust selected line
        if (m_highlightedLine > m_fileLister.getNbTotal() - 1)
            m_highlightedLine = m_fileLister.getNbTotal() - 1;
    }
    else
    {
        // Current path doesn't exist anymore => default
        m_fileLister.list(PATH_DEFAULT);
        m_currentPath = PATH_DEFAULT;
        m_highlightedLine = 0;
    }
    // Camera
    adjustCamera();
    // Clear select list
    m_selectList.clear();
}

const bool CPanel::addToSelectList(const bool p_step)
{
    if (m_fileLister[m_highlightedLine].m_name != "..")
    {
        // Search highlighted element in select list
        std::set<unsigned int>::iterator l_it = m_selectList.find(m_highlightedLine);
        if (l_it == m_selectList.end())
            // Element not present => we add it
            m_selectList.insert(m_highlightedLine);
        else
            // Element present => we remove it from the list
            m_selectList.erase(m_highlightedLine);
        if (p_step)
            moveCursorDown(1);
        return true;
    }
    else
    {
        return false;
    }
}

const std::set<unsigned int> &CPanel::getSelectList(void) const
{
    return m_selectList;
}

void CPanel::getSelectList(std::vector<std::string> &p_list) const
{
    p_list.clear();
    // Insert full path of selected files
    for (std::set<unsigned int>::const_iterator l_it = m_selectList.begin(); l_it != m_selectList.end(); ++l_it)
    {
        if (m_currentPath == "/")
            p_list.push_back(m_currentPath + m_fileLister[*l_it].m_name);
        else
            p_list.push_back(m_currentPath + "/" + m_fileLister[*l_it].m_name);
    }
}

void CPanel::selectAll(void)
{
    const unsigned int l_nb = m_fileLister.getNbTotal();
    for (unsigned int l_i = 1; l_i < l_nb; ++l_i)
        m_selectList.insert(l_i);
}

void CPanel::selectNone(void)
{
    m_selectList.clear();
}

const bool CPanel::isDirectoryHighlighted(void) const
{
    return m_fileLister.isDirectory(m_highlightedLine);
}
