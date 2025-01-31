#include <common.h>
#include <shared_funcs.h>
#include <bg_colors.h>
#include <multitap.h>
#include <custom_text.h>
#include <sound.h>
#include <shared_variables.h>

ModState mod_state = GAME_STARTED;
CustomMenuState customMenuState = NONE;

int customMenuSelection = 0;
int customMenuSelectionColorFrames = 0;
char customMenuColorArray[8] = {
    MOBY_COLOR_GOLD,
    MOBY_COLOR_GOLD,
    MOBY_COLOR_GOLD,
    MOBY_COLOR_GOLD,
    MOBY_COLOR_GOLD,
    MOBY_COLOR_GOLD,
    MOBY_COLOR_GOLD,
    MOBY_COLOR_GOLD
};
char modOptions[4] = {1, true, false, false};
char modDefaultOptions[4] = {1, true, false, false};
char asciiTrue[] = "ON";
char asciiFalse[] = "OFF";
char* boolToAscii(char inputValue) {
    if (inputValue) {
        return &asciiTrue[0];
    } else {
        return &asciiFalse[0];
    }
}

// SPEEDWAY MOD text is in igt_menu.c
void CustomTitleScreen()
{
    int customMenuSelectionColor = MOBY_COLOR_SHINY_GOLD;
    if (_gameState == GAMESTATE_TITLE_SCREEN) {
        customMenuSelectionColorFrames++;
    } else {
        customMenuSelectionColorFrames += 2;
    }
    customMenuSelectionColor = MOBY_COLOR_SHINY_GOLD;
    if (customMenuSelectionColorFrames >= 16 && customMenuSelectionColorFrames < 32) {
        customMenuSelectionColor = MOBY_COLOR_SILVER;
    } else if (customMenuSelectionColorFrames >= 32) {
        customMenuSelectionColorFrames = 0;
    }

    customMenuColorArray[customMenuSelection] = customMenuSelectionColor;

    if (customMenuState != NONE && customMenuState != RETURN_TO_TITLE) {
        DrawTextAll(
            // SPEEDWAT MOD
            stringList[7],
            &(CapitalTextInfo){.x = 0x2f, .y = 0x35, .size = 0x600},
            &(LowercaseTextInfo){.spacing = DEFAULT_SPACING + 20, .yOffset = 0, .size = 0x900},
            DEFAULT_SPACING + 28,
            MOBY_COLOR_PURPLE
        );
        if (customMenuState != TITLE_MOD_OPTIONS) {
            // VERSION STRING
            DrawTextAll(
                stringList[8],
                &(CapitalTextInfo){.x = SCREEN_RIGHT_EDGE - 0x60, .y = 0x54, .size = 0x1a00},
                &(LowercaseTextInfo){.spacing = 0xb, .yOffset = 0, .size = 0x1a00},
                0xb,
                MOBY_COLOR_GREY
            );
        }
    }

    if (mod_state == GAME_STARTED && _globalTimer > 20) {

        // If the code hasn't ran once yet, and the global timer is greater than 20. Checking global timer because I have to wait a few frames to call The Adventure Begins
        LoadCustomTitleScreen();
        mod_state = LOADED_TITLE_SCREEN;
    } else if (mod_state == LEFT_TITLE_SCREEN) {

        // Load intro a frame later, once this function is called again after selecting start game
        SkipIntro();
        mod_state = SKIPPED_INTRO;
    } else if (customMenuState == TITLE) {
        
        // Custom Title Screen
        // PRESS START
        DrawTextAll(
            &_PRESS_START,
            &(CapitalTextInfo){.x = 0xa9, .y = SCREEN_BOTTOM_EDGE - 0x10, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            customMenuColorArray[0]
        );
        if (_currentButtonOneFrame & START_BUTTON || _currentButtonOneFrame & X_BUTTON) {
            customMenuState = TITLE_MENU;
            customMenuColorArray[customMenuSelection] = MOBY_COLOR_GOLD;
            customMenuSelection = 1;
         }
    } else if (customMenuState == TITLE_MENU) {
        // MOD OPTIONS
        DrawTextAll(
            stringList[0],
            &(CapitalTextInfo){.x = 0xa9, .y = SCREEN_BOTTOM_EDGE - 0x28, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            customMenuColorArray[0]
        );
        // START NEW GAME
        DrawTextAll(
            stringList[1],
            &(CapitalTextInfo){.x = 0x91, .y = SCREEN_BOTTOM_EDGE - 0x10, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            customMenuColorArray[1]
        );
        if (_currentButtonOneFrame & UP_BUTTON || _currentButtonOneFrame & DOWN_BUTTON) {
            customMenuColorArray[customMenuSelection] = MOBY_COLOR_GOLD;
            customMenuSelection++;
            if (customMenuSelection == 2) {
                customMenuSelection = 0;
            }
        }
        if (_currentButtonOneFrame & START_BUTTON || _currentButtonOneFrame & X_BUTTON) {
            if (customMenuSelection == 0) {
                customMenuState = TITLE_MOD_OPTIONS;
            } else {
                customMenuState = NONE;
                mod_state = LEFT_TITLE_SCREEN;
            }
         } else if (_currentButtonOneFrame & TRIANGLE_BUTTON) {
            customMenuState = TITLE;
            customMenuColorArray[customMenuSelection] = MOBY_COLOR_GOLD;
            customMenuSelection = 0;
         }
    } else if (customMenuState == TITLE_MOD_OPTIONS) {
        // SUPERCHARGE SPEED
        DrawTextAll(
            stringList[2],
            &(CapitalTextInfo){.x = 0x10, .y = SCREEN_BOTTOM_EDGE - 0x88, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            customMenuColorArray[0]
        );
        char asciiValue[3] = {modOptions[0] + 0x30, 0, 0};
        if (asciiValue[0] >= 0x3a) {
            asciiValue[1] = asciiValue[0] - 0xa;
            asciiValue[0] = 0x31;
        }
        DrawTextAll(
            &asciiValue[0],
            &(CapitalTextInfo){.x = SCREEN_RIGHT_EDGE - 0x34, .y = SCREEN_BOTTOM_EDGE - 0x88, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            customMenuColorArray[0]
        );
        // INFINITE FLIGHT
        DrawTextAll(
            stringList[3],
            &(CapitalTextInfo){.x = 0x10, .y = SCREEN_BOTTOM_EDGE - 0x70, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            customMenuColorArray[1]
        );
        DrawTextAll(
            boolToAscii(modOptions[1]),
            &(CapitalTextInfo){.x = SCREEN_RIGHT_EDGE - 0x34, .y = SCREEN_BOTTOM_EDGE - 0x70, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            customMenuColorArray[1]
        );
        // PERMANENT SUPERFLAME
        DrawTextAll(
            stringList[4],
            &(CapitalTextInfo){.x = 0x10, .y = SCREEN_BOTTOM_EDGE - 0x58, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            customMenuColorArray[2]
        );
        DrawTextAll(
            boolToAscii(modOptions[2]),
            &(CapitalTextInfo){.x = SCREEN_RIGHT_EDGE - 0x34, .y = SCREEN_BOTTOM_EDGE - 0x58, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            customMenuColorArray[2]
        );
        // SPARXLESS
        DrawTextAll(
            stringList[5],
            &(CapitalTextInfo){.x = 0x10, .y = SCREEN_BOTTOM_EDGE - 0x40, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            customMenuColorArray[3]
        );
        DrawTextAll(
            boolToAscii(modOptions[3]),
            &(CapitalTextInfo){.x = SCREEN_RIGHT_EDGE - 0x34, .y = SCREEN_BOTTOM_EDGE - 0x40, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            customMenuColorArray[3]
        );
        // RESTORE DEFAULTS
        DrawTextAll(
            stringList[6],
            &(CapitalTextInfo){.x = 0x7c, .y = SCREEN_BOTTOM_EDGE - 0x28, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            customMenuColorArray[4]
        );
        // DONE
        DrawTextAll(
            &_DONE,
            &(CapitalTextInfo){.x = 0xe5, .y = SCREEN_BOTTOM_EDGE - 0x10, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            customMenuColorArray[5]
        );
        if (_currentButtonOneFrame & UP_BUTTON) {
            customMenuColorArray[customMenuSelection] = MOBY_COLOR_GOLD;
            customMenuSelection--;
            if (customMenuSelection == -1) {
                customMenuSelection = 5;
            }
        } else if (_currentButtonOneFrame & DOWN_BUTTON) {
            customMenuColorArray[customMenuSelection] = MOBY_COLOR_GOLD;
            customMenuSelection++;
            if (customMenuSelection == 6) {
                customMenuSelection = 0;
            }
        }
        if (((_currentButtonOneFrame & START_BUTTON || _currentButtonOneFrame & X_BUTTON) && customMenuSelection == 5) || _currentButtonOneFrame & TRIANGLE_BUTTON) {
            customMenuState = TITLE_MENU;
            customMenuColorArray[customMenuSelection] = MOBY_COLOR_GOLD;
            customMenuSelection = 1;
        } else if ((_currentButtonOneFrame & START_BUTTON || _currentButtonOneFrame & X_BUTTON) && customMenuSelection == 4) {
            int i = 0;
            while (i < 4) {
                modOptions[i] = modDefaultOptions[i];
                i++;
            }
        } else if ((_currentButtonOneFrame & START_BUTTON || _currentButtonOneFrame & X_BUTTON || _currentButtonOneFrame & LEFT_BUTTON || _currentButtonOneFrame & RIGHT_BUTTON) && (customMenuSelection > 0 && customMenuSelection < 4)) {
            if (modOptions[customMenuSelection]) {
                modOptions[customMenuSelection] = false;
            } else {
                modOptions[customMenuSelection] = true;
            }
        } else if (customMenuSelection == 0) {
            if (_currentButtonOneFrame & LEFT_BUTTON && modOptions[0] != 1) {
                modOptions[0]--;
            } else if (_currentButtonOneFrame & RIGHT_BUTTON && modOptions[0] != 4) {
                modOptions[0]++;
            }
        }
    } else if (customMenuState == RETURN_TO_TITLE) {
        customMenuState = TITLE;
    }

}
