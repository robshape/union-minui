#ifndef _DEF_H_
#define _DEF_H_

//~ #define INHIBIT(X) X
#define INHIBIT(X) /* X */

#define SCREEN_WIDTH          320
#define SCREEN_HEIGHT         240
#define SCREEN_BPP            16
#define SURFACE_FLAGS         SDL_SWSURFACE
#define MS_PER_FRAME          33
// Panel
#define LINE_HEIGHT           15
#define NB_VISIBLE_LINES      14
#define Y_LIST                17
#define Y_HEADER              3
#if defined(PLATFORM_TRIMUI)
#define Y_OFFSET              2 // added for larger Commander-11.ttf
#else
#define Y_OFFSET              0
#endif
#define Y_FOOTER              228
#define H_FOOTER              13
// Dialogs
#define DIALOG_BORDER         2
#define DIALOG_MARGIN         8
// Colors
#define COLOR_KEY             255,0,255
#define COLOR_TEXT_NORMAL     70,27,10
#define COLOR_TEXT_TITLE      233,229,227
#define COLOR_TEXT_DIR        75,70,164
#define COLOR_TEXT_SELECTED   255,0,0
#define COLOR_CURSOR_1        232,152,80
#define COLOR_CURSOR_2        232,201,173
#define COLOR_BG_1            255,255,255
#define COLOR_BG_2            232,228,224
#define COLOR_BORDER          102,85,74

#define DINGOO_X SDLK_LSHIFT
#define DINGOO_Y SDLK_LALT

#if defined(PLATFORM_TRIMUI)
#define MYKEY_UP            SDLK_UP           // Up
#define MYKEY_RIGHT         SDLK_RIGHT        // Right
#define MYKEY_DOWN          SDLK_DOWN         // Down
#define MYKEY_LEFT          SDLK_LEFT         // Left
#define MYKEY_SYSTEM        SDLK_LALT         // Y
#define MYKEY_PAGEUP        SDLK_TAB          // L
#define MYKEY_PAGEDOWN      SDLK_BACKSPACE    // R
#define MYKEY_OPEN          SDLK_SPACE        // A
#define MYKEY_PARENT        SDLK_LCTRL        // B
#define MYKEY_OPERATION     SDLK_LSHIFT       // X
#define MYKEY_SELECT        SDLK_RCTRL        // SELECT
#define MYKEY_TRANSFER      SDLK_RETURN       // START
#define MYKEY_MENU          SDLK_ESCAPE       // MENU
#define PATH_DEFAULT        "/"
#define FILE_SYSTEM         "/dev/mmcblk0p1"
#elif defined(PLATFORM_RG35XX)
#define MYKEY_UP			SDLK_KATAKANA
#define MYKEY_RIGHT			SDLK_KATAKANAHIRAGANA
#define MYKEY_DOWN			SDLK_HIRAGANA
#define MYKEY_LEFT			SDLK_HENKAN
#define MYKEY_SYSTEM		SDLK_RCTRL // Y
#define MYKEY_PAGEUP		SDLK_RALT // L
#define MYKEY_PAGEDOWN		SDLK_BREAK // R
#define MYKEY_OPEN			SDLK_MUHENKAN // A
#define MYKEY_PARENT		SDLK_KP_JPCOMMA // B
#define MYKEY_OPERATION		SDLK_KP_ENTER // X
#define MYKEY_SELECT		SDLK_PRINT // SELECT
#define MYKEY_TRANSFER		SDLK_KP_DIVIDE // START
#define MYKEY_MENU			SDLK_PAGEUP // MENU
#define PATH_DEFAULT		"/"
#define FILE_SYSTEM			"/dev/mmcblk0p1"
#elif defined(PLATFORM_MIYOOMINI)
#define MYKEY_UP            SDLK_UP           // Up
#define MYKEY_RIGHT         SDLK_RIGHT        // Right
#define MYKEY_DOWN          SDLK_DOWN         // Down
#define MYKEY_LEFT          SDLK_LEFT         // Left
#define MYKEY_SYSTEM        SDLK_LALT         // Y
#define MYKEY_PAGEUP        SDLK_e            // L
#define MYKEY_PAGEDOWN      SDLK_t    		  // R
#define MYKEY_OPEN          SDLK_SPACE        // A
#define MYKEY_PARENT        SDLK_LCTRL        // B
#define MYKEY_OPERATION     SDLK_LSHIFT       // X
#define MYKEY_SELECT        SDLK_RCTRL        // SELECT
#define MYKEY_TRANSFER      SDLK_RETURN       // START
#define MYKEY_MENU          SDLK_ESCAPE       // MENU
#define PATH_DEFAULT        "/"
#define FILE_SYSTEM         "/dev/mmcblk0p1"
#elif defined(PLATFORM_RG350) || defined(PLATFORM_ODBETA)
#define MYKEY_UP            SDLK_UP           // Up
#define MYKEY_RIGHT         SDLK_RIGHT        // Right
#define MYKEY_DOWN          SDLK_DOWN         // Down
#define MYKEY_LEFT          SDLK_LEFT         // Left
#define MYKEY_SYSTEM        SDLK_LSHIFT       // X
#define MYKEY_PAGEUP        SDLK_TAB          // L
#define MYKEY_PAGEDOWN      SDLK_BACKSPACE    // R
#define MYKEY_OPEN          SDLK_LCTRL        // A
#define MYKEY_PARENT        SDLK_LALT         // B
#define MYKEY_OPERATION     SDLK_SPACE        // Y
#define MYKEY_SELECT        SDLK_ESCAPE       // SELECT
#define MYKEY_TRANSFER      SDLK_RETURN       // START
#define MYKEY_MENU          SDLK_HOME         // POWER
#define PATH_DEFAULT        "/"
#define FILE_SYSTEM         "/dev/mmcblk0p1"
#elif defined(PLATFORM_RETROFW)
#define MYKEY_UP            SDLK_UP           // Up
#define MYKEY_RIGHT         SDLK_RIGHT        // Right
#define MYKEY_DOWN          SDLK_DOWN         // Down
#define MYKEY_LEFT          SDLK_LEFT         // Left
#define MYKEY_SYSTEM        SDLK_LSHIFT       // X
#define MYKEY_PAGEUP        SDLK_TAB          // L
#define MYKEY_PAGEDOWN      SDLK_BACKSPACE    // R
#define MYKEY_OPEN          SDLK_LCTRL        // A
#define MYKEY_PARENT        SDLK_LALT         // B
#define MYKEY_OPERATION     SDLK_SPACE        // Y
#define MYKEY_SELECT        SDLK_ESCAPE       // SELECT
#define MYKEY_TRANSFER      SDLK_RETURN       // START
#define MYKEY_MENU          SDLK_END          // POWER
#define PATH_DEFAULT        "/"
#define FILE_SYSTEM         "/dev/mmcblk0p1"
#endif

#endif
