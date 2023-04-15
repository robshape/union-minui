#include <iostream>
#include <sstream>
#include "commander.h"
#include "resourceManager.h"
#include "sdlutils.h"
#include "def.h"
#include "dialog.h"
#include "fileutils.h"
#include "viewer.h"
#include "keyboard.h"

#define X_LEFT                1
#define X_RIGHT               162

CCommander::CCommander(const std::string &p_pathL, const std::string &p_pathR):
    CWindow::CWindow(),
    m_panelLeft(p_pathL, X_LEFT),
    m_panelRight(p_pathR, X_RIGHT),
    m_panelSource(NULL),
    m_panelTarget(NULL),
    m_background(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_BG))
{
    m_panelSource = &m_panelLeft;
    m_panelTarget = &m_panelRight;
}

CCommander::~CCommander(void)
{
}

void CCommander::render(const bool p_focus) const
{
    INHIBIT(std::cout << "CCommander::render  fullscreen: " << isFullScreen() << "  focus: " << p_focus << std::endl;)
    // Draw background image
    SDL_utils::applySurface(0, 0, m_background, Globals::g_screen);
    // Draw panels
    m_panelLeft.render(p_focus && (m_panelSource == &m_panelLeft));
    m_panelRight.render(p_focus && (m_panelSource == &m_panelRight));
}

const bool CCommander::keyPress(const SDL_Event &p_event)
{
    CWindow::keyPress(p_event);
    bool l_ret(false);
	switch (p_event.key.keysym.sym)
    {
		case MYKEY_SYSTEM:
		case MYKEY_MENU:
            if (openSystemMenu())
                m_panelSource->refresh();
            l_ret = true;
            break;
        case MYKEY_UP:
            l_ret = m_panelSource->moveCursorUp(1);
            break;
        case MYKEY_DOWN:
            l_ret = m_panelSource->moveCursorDown(1);
            break;
        case MYKEY_PAGEUP:
            l_ret = m_panelSource->moveCursorUp(NB_VISIBLE_LINES - 1);
            break;
        case MYKEY_PAGEDOWN:
            l_ret = m_panelSource->moveCursorDown(NB_VISIBLE_LINES - 1);
            break;
        case MYKEY_LEFT:
            if (m_panelSource == &m_panelRight)
            {
                m_panelSource = &m_panelLeft;
                m_panelTarget = &m_panelRight;
                l_ret = true;
            }
            break;
        case MYKEY_RIGHT:
            if (m_panelSource == &m_panelLeft)
            {
                m_panelSource = &m_panelRight;
                m_panelTarget = &m_panelLeft;
                l_ret = true;
            }
            break;
        case MYKEY_OPEN:
            if (m_panelSource->isDirectoryHighlighted())
            {
                // It's a dir => open it
                l_ret = m_panelSource->open();
            }
            else
            {
                // It's a file => open execute menu
                openExecuteMenu();
                l_ret = true;
            }
            break;
        case MYKEY_PARENT:
            l_ret = m_panelSource->goToParentDir();
            break;
        case MYKEY_OPERATION:
            // If there's no file in the select list, add current file
            if (m_panelSource->getSelectList().empty() && m_panelSource->getHighlightedItem() != "..")
                m_panelSource->addToSelectList(false);
            if (!m_panelSource->getSelectList().empty())
            {
                if (openCopyMenu())
                {
                    // Refresh file lists
                    m_panelSource->refresh();
                    m_panelTarget->refresh();
                }
                else
                {
                    if (m_panelSource->getSelectList().size() == 1 && (*m_panelSource->getSelectList().begin()) == m_panelSource->getHighlightedIndex())
                        m_panelSource->selectNone();
                }
                l_ret = true;
            }
            break;
        case MYKEY_SELECT:
            l_ret = m_panelSource->addToSelectList(true);
            break;
        case MYKEY_TRANSFER:
            if (m_panelSource->isDirectoryHighlighted() && m_panelSource->getHighlightedItem() != "..")
                l_ret = m_panelTarget->open(m_panelSource->getHighlightedItemFull());
            else
                l_ret = m_panelTarget->open(m_panelSource->getCurrentPath());
            break;
        default:
            break;
    }
    return l_ret;
}

const bool CCommander::keyHold(void)
{
    bool l_ret(false);
    switch(m_lastPressed)
    {
        case MYKEY_UP:
            if (tick(SDL_GetKeyState(NULL)[MYKEY_UP]))
                l_ret = m_panelSource->moveCursorUp(1);
            break;
        case MYKEY_DOWN:
            if (tick(SDL_GetKeyState(NULL)[MYKEY_DOWN]))
                l_ret = m_panelSource->moveCursorDown(1);
            break;
        case MYKEY_PAGEUP:
            if (tick(SDL_GetKeyState(NULL)[MYKEY_PAGEUP]))
                l_ret = m_panelSource->moveCursorUp(NB_VISIBLE_LINES - 1);
            break;
        case MYKEY_PAGEDOWN:
            if (tick(SDL_GetKeyState(NULL)[MYKEY_PAGEDOWN]))
                l_ret = m_panelSource->moveCursorDown(NB_VISIBLE_LINES - 1);
            break;
        case MYKEY_SELECT:
            if (tick(SDL_GetKeyState(NULL)[MYKEY_SELECT]))
                l_ret = m_panelSource->addToSelectList(true);
            break;
        default:
            break;
    }
    return l_ret;
}

const bool CCommander::openCopyMenu(void) const
{
    bool l_ret(false);
    int l_dialogRetVal(0);
    bool l_rename(false);
    // List of selected files
    std::vector<std::string> l_list;
    m_panelSource->getSelectList(l_list);
    // The rename option appears only if one item is selected
    l_rename = (l_list.size() == 1);
    {
        bool l_loop(false);
        std::ostringstream l_stream;
        l_stream << l_list.size() << " selected:";
        // File operation dialog
        CDialog l_dialog(l_stream.str(), 0, Y_LIST + m_panelSource->getHighlightedIndexRelative() * LINE_HEIGHT);
        l_dialog.addOption(m_panelSource == &m_panelLeft ? "Copy >" : "< Copy");
        l_dialog.addOption(m_panelSource == &m_panelLeft ? "Move >" : "< Move");
        if (l_rename)
            l_dialog.addOption("Rename");
        l_dialog.addOption("Delete");
        l_dialog.addOption("Disk used");
        l_dialog.init();
        do
        {
            l_loop = false;
            l_dialogRetVal = l_dialog.execute();
            if (l_dialogRetVal == 3 + l_rename)
            {
                CDialog l_dialog2("", l_dialog.getX() + l_dialog.getImage()->w - DIALOG_BORDER, l_dialog.getY() + DIALOG_BORDER + (l_dialog.getHighlightedIndex() + 1) * LINE_HEIGHT);
                l_dialog2.addOption("Yes");
                l_dialog2.addOption("No");
                l_dialog2.init();
                if (l_dialog2.execute() != 1)
                    l_loop = true;
            }
        }
        while (l_loop);
    }
    // Perform operation
    switch (l_dialogRetVal)
    {
        case 1:
            // Copy
            File_utils::copyFile(l_list, m_panelTarget->getCurrentPath());
            l_ret = true;
            break;
        case 2:
            // Move
            File_utils::moveFile(l_list, m_panelTarget->getCurrentPath());
            l_ret = true;
            break;
        case 3:
            if (l_rename)
            {
                // Rename
                CKeyboard l_keyboard(m_panelSource->getHighlightedItem());
                if (l_keyboard.execute() == 1 && !l_keyboard.getInputText().empty() && l_keyboard.getInputText() != m_panelSource->getHighlightedItem())
                {
                    File_utils::renameFile(m_panelSource->getHighlightedItemFull(), m_panelSource->getCurrentPath() + (m_panelSource->getCurrentPath() == "/" ? "" : "/") + l_keyboard.getInputText());
                    l_ret = true;
                }
            }
            else
            {
                // Delete
                File_utils::removeFile(l_list);
                l_ret = true;
            }
            break;
        case 4:
            if (l_rename)
            {
                // Delete
                File_utils::removeFile(l_list);
                l_ret = true;
            }
            else
                // Disk used
                File_utils::diskUsed(l_list);
            break;
        case 5:
            if (l_rename)
                // Disk used
                File_utils::diskUsed(l_list);
            break;
        default:
            break;
    }
    return l_ret;
}

const bool CCommander::openSystemMenu(void)
{
    bool l_ret(false);
    int l_dialogRetVal(0);
    // Selection dialog
    {
        CDialog l_dialog("System:", 0, Y_LIST + m_panelSource->getHighlightedIndexRelative() * LINE_HEIGHT);
        l_dialog.addOption("Select all");
        l_dialog.addOption("Select none");
        l_dialog.addOption("New directory");
        l_dialog.addOption("Disk info");
        l_dialog.addOption("Quit");
        l_dialog.init();
        l_dialogRetVal = l_dialog.execute();
    }
    switch (l_dialogRetVal)
    {
        case 1:
            // Select all
            m_panelSource->selectAll();
            break;
        case 2:
            // Select none
            m_panelSource->selectNone();
            break;
        case 3:
            // New dir
            {
                CKeyboard l_keyboard("");
                if (l_keyboard.execute() == 1 && !l_keyboard.getInputText().empty())
                {
                    File_utils::makeDirectory(m_panelSource->getCurrentPath() + (m_panelSource->getCurrentPath() == "/" ? "" : "/") + l_keyboard.getInputText());
                    l_ret = true;
                }
            }
            break;
        case 4:
            // Disk info
            File_utils::diskInfo();
            break;
        case 5:
            // Quit
            m_retVal = -1;
            break;
        default:
            break;
    }
    return l_ret;
}

void CCommander::openExecuteMenu(void) const
{
    int l_dialogRetVal(0);
    // Dialog
    {
        CDialog l_dialog(m_panelSource->getHighlightedItem() + ":", 0, Y_LIST + m_panelSource->getHighlightedIndexRelative() * LINE_HEIGHT);
        l_dialog.addOption("View");
        l_dialog.addOption("Execute");
        l_dialog.init();
        l_dialogRetVal = l_dialog.execute();
    }
    // Perform operation
    switch (l_dialogRetVal)
    {
        case 1:
            // View
            {
                // Check size
                const std::string l_file(m_panelSource->getHighlightedItemFull());
                INHIBIT(std::cout << "File size: " << File_utils::getFileSize(l_file) << std::endl;)
                if (File_utils::getFileSize(l_file) > VIEWER_SIZE_MAX)
                {
                    // File is too big to be viewed!
                    CDialog l_dialog("Error:", 0, 0);
                    l_dialog.addLabel("File is too big!");
                    l_dialog.addOption("OK");
                    l_dialog.init();
                    l_dialog.execute();
                }
                else
                {
                    CViewer l_viewer(m_panelSource->getHighlightedItemFull());
                    l_viewer.execute();
                }
            }
            break;
        case 2:
            // Execute
            File_utils::executeFile(m_panelSource->getHighlightedItemFull());
            break;
        default:
            break;
    }
}

const bool CCommander::isFullScreen(void) const
{
    return true;
}
