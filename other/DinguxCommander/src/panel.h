#ifndef _PANEL_H_
#define _PANEL_H_

#include <string>
#include <set>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "fileLister.h"
#include "def.h"

class CPanel
{
    public:

    // Constructor
    CPanel(const std::string &p_path, const Sint16 p_x);

    // Destructor
    virtual ~CPanel(void);

    // Draw the panel on the screen
    void render(const bool p_active) const;

    // Move cursor
    const bool moveCursorUp(unsigned char p_step);
    const bool moveCursorDown(unsigned char p_step);

    // Open selected item
    const bool open(const std::string &p_path = "");

    // Refresh current directory
    void refresh(void);

    // Go to parent dir
    const bool goToParentDir(void);

    // Selected file with just the name
    const std::string &getHighlightedItem(void) const;

    // Selected file with full path
    const std::string getHighlightedItemFull(void) const;

    // Current path
    const std::string &getCurrentPath(void) const;

    // Selected index
    const unsigned int &getHighlightedIndex(void) const;
    const unsigned int getHighlightedIndexRelative(void) const;

    // True => directory, false => file, or dir ".."
    const bool isDirectoryHighlighted(void) const;

    // Add/remove current file to the select list
    const bool addToSelectList(const bool p_step);

    // Get select list
    const std::set<unsigned int> &getSelectList(void) const;
    void getSelectList(std::vector<std::string> &p_list) const;

    // Clear select list
    void selectAll(void);
    void selectNone(void);

    private:

    // Forbidden
    CPanel(void);
    CPanel(const CPanel &p_source);
    const CPanel &operator =(const CPanel &p_source);

    // Adjust camera
    void adjustCamera(void);

    // File lister
    CFileLister m_fileLister;

    // Current path
    std::string m_currentPath;

    // Index of the first displayed line
    unsigned int m_camera;

    // X coordinate
    const Sint16 m_x;

    // Highlighted line
    unsigned int m_highlightedLine;

    // Selection list
    std::set<unsigned int> m_selectList;

    // Pointers to resources
    SDL_Surface *m_iconDir;
    SDL_Surface *m_iconFile;
    SDL_Surface *m_iconUp;
    SDL_Surface *m_cursor1;
    SDL_Surface *m_cursor2;
    TTF_Font *m_font;
};

#endif
