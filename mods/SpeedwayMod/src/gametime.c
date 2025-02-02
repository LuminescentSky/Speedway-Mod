#include <common.h>
#include <main_updates.h>
#include <multitap.h>
#include <shared_funcs.h>
#include <bg_colors.h>
#include <custom_text.h>
#include <sound.h>
#include <spyro.h>
#include <shared_variables.h>

int totalGameTime = 0;
int levelGameTime = 0;
int currentLevel = 0;
int customLootTimer = 0;

int completionFlags[2] = {0, 0};

char sharpTurnOffset = 0;
char calibrationInfoTimer = 0;
char sharpTurnInfo[16];

char alignmentOffset = 0;

int hideGameTime = 0;
char* gameTimeStrings[3] = {
	"L1 TO HIDE GAME TIME",
	"GAME TIME",
	"."
};


char asciiTime[11];
int timerArray[7];

unsigned char sharpTurnOffsetXPosition;

void GameTime(void) {
    
	// Reset IGT on Title Screen
	if (customMenuState == TITLE) {
		totalGameTime = 0;
		levelGameTime = 0;
	}
	if (_gameState == GAMESTATE_GAMEPLAY) {
		totalGameTime += 100;
		if (!(_altLevelID % 10 == 5 && LevelComplete())) {
			levelGameTime += 100;
		}
		if (_altLevelID % 10 == 5) {
			if (levelGameTime < 1492800) {
				_flightIGT = levelGameTime * 100 / 4976;
			} else {
				_flightIGT = 29999;
			}
		}
		// Prevent Overflow
		if (totalGameTime > 895679900) {
			totalGameTime = 895679900;
		}
		if (levelGameTime > 89567900) {
			levelGameTime = 89567900;
		}
        if (calibrationInfoTimer > 0) {
            DrawTextAll(
                sharpTurnInfo,
                &(CapitalTextInfo){.x = sharpTurnOffsetXPosition, .y = SCREEN_BOTTOM_EDGE - 0x8, .size = DEFAULT_SIZE},
                &defaultLowerCase,
                DEFAULT_SPACING,
                MOBY_COLOR_GOLD
            );
            calibrationInfoTimer--;
        } else if (_altLevelID % 10 != 0 && _altLevelID % 10 != 5) {
        	DrawIGT(0xaf, SCREEN_BOTTOM_EDGE - 0x8, 1, false, MOBY_COLOR_GOLD, CENTRE);
        }
        MobyRender();
    	if (_altLevelID != 10 && hideGameTime == 0) {
    		hideGameTime = 1;
    	}
	}
	if (_gameState == GAMESTATE_LOADING || (_gameState == GAMESTATE_BALLOONIST && _levelLoadState != -1)) {
    	if (hideGameTime < 2) {
			DrawIGT(0x10, 0x50, 0, true, MOBY_COLOR_GOLD, LEFT);
	        MobyRender();
	    }
	    if (_currentButtonOneFrame & L1_BUTTON) {
	    	if (hideGameTime < 2) {
	    		hideGameTime = 2;
	    	} else {
	    		hideGameTime = 1;
	    	}
	    }
    }
}

void DrawIGT(int xPosition, int yPosition, char IGTtype, bool gameTimeText, char colour, char alignment) {
	// IGTtype: 0 = game time, 1 = level time, 2 = best time, 3 = ......
	alignmentOffset = 0;
	if (IGTtype == 0) {
		getCurrentIGT(totalGameTime);
	} else if (IGTtype == 1) {
		getCurrentIGT(levelGameTime);
	} else if (IGTtype == 2) {
		getCurrentIGT(bestTimes[_levelIDIndex]);
	} else {
		getCurrentIGT(0);
	}
	if (alignment == LEFT) {
		alignmentOffset = 0;
	} else if (alignment == CENTRE) {
		alignmentOffset /= 2;
	}
	if (gameTimeText) {
        DrawTextAll(
            gameTimeStrings[hideGameTime],
            &(CapitalTextInfo){.x = xPosition, .y = 0x38, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            MOBY_COLOR_PURPLE
        );
	}
    DrawTextAll(
        asciiTime,
        &(CapitalTextInfo){.x = alignmentOffset + xPosition, .y = yPosition, .size = DEFAULT_SIZE},
        &defaultLowerCase,
        DEFAULT_SPACING,
        colour
    );
    if (timerArray[0] > 0) {
        DrawTextAll(
            gameTimeStrings[2],
            &(CapitalTextInfo){.x = colonOffset(alignment) + xPosition, .y = yPosition -6, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            colour
        );
    }
    if (timerArray[2] > 0 || timerArray[1] > 0 || timerArray[0] > 0) {
        DrawTextAll(
            gameTimeStrings[2],
            &(CapitalTextInfo){.x = colonOffset(alignment) + 0x36 + xPosition, .y = yPosition -6, .size = DEFAULT_SIZE},
            &defaultLowerCase,
            DEFAULT_SPACING,
            colour
        );
    }
}

void getCurrentIGT(int inputFrameTime) {
    char i = 6;
    while (i >= 0) {
    	timerArray[i] = 0;
    	i--;
    }
	if (inputFrameTime != 0) {
	    /*char frameRate;
	    if (CheckRegion() == NTSC) {
	    	frameRate = 30;
	    } else {
	    	frameRate = 25;
	    }*/
	    timerArray[0] = inputFrameTime / (3600 * 2488);
	    timerArray[2] = (inputFrameTime % (3600 * 2488)) / (60 * 2488);
	    timerArray[4] = (inputFrameTime % (60 * 2488)) / 2488;
	    timerArray[6] = ((inputFrameTime % 2488) * 100 / 2488);
	    i = 6;
	    while (i >= 2) {
	        if (timerArray[i] >= 10) {
	        	timerArray[i - 1] = timerArray[i] / 10;
	        	timerArray[i] = timerArray[i] % 10;
	        }
	    	i -= 2;
	    }
    	if (timerArray[0] > 0) {
    		sprintf(&asciiTime, "%d.%d%d.%d%d.%d%d", timerArray[0], timerArray[1], timerArray[2], timerArray[3], timerArray[4], timerArray[5], timerArray[6]);
    		if (timerArray[0] >= 10) {
    			alignmentOffset = -18;
    		} else {
    			alignmentOffset = 0;
    		}
    	} else if (timerArray[1] > 0) {
    		sprintf(&asciiTime, "%d%d.%d%d.%d%d", timerArray[1], timerArray[2], timerArray[3], timerArray[4], timerArray[5], timerArray[6]);
    		alignmentOffset = 36;
    	} else if (timerArray[2] > 0) {
    		sprintf(&asciiTime, "%d.%d%d.%d%d", timerArray[2], timerArray[3], timerArray[4], timerArray[5], timerArray[6]);
    		alignmentOffset = 54;
    	} else if (timerArray[3] > 0) {
    		sprintf(&asciiTime, "%d%d.%d%d", timerArray[3], timerArray[4], timerArray[5], timerArray[6]);
    		alignmentOffset = 90;
    	} else {
    		sprintf(&asciiTime, "%d.%d%d", timerArray[4], timerArray[5], timerArray[6]);
    		alignmentOffset = 108;
    	}
	} else {
	    alignmentOffset = 72;
		sprintf(&asciiTime, "......");
	}
}

int colonOffset(alignment) {
	if (alignment == RIGHT || (timerArray[0] > 0 && timerArray[0] < 10)) {
		return 18;
	} else if (timerArray[0] >= 10) {
		if (alignment == LEFT) {
			return 36;
		} else {
			return 27;
		}
	} else if (timerArray[1] > 0) {
		if (alignment == LEFT) {
			return -18;
		} else {
			return 0;
		}
	} else {
		if (alignment == LEFT) {
			return -36;
		} else {
			return -9;
		}
	}
}
