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
char timerArray[7];

void GameTime(void) {
    // Set level complete flag
    if (LevelComplete()) {
        completionFlags[realLevelIDIndex / 32] |= 1 << (realLevelIDIndex % 32);
    }
    
	// Reset IGT on Title Screen
	if (customMenuState == TITLE) {
		totalGameTime = 0;
		levelGameTime = 0;
	}
	if (_gameState == GAMESTATE_GAMEPLAY) {
		totalGameTime++;
		levelGameTime++;
		if (_altLevelID % 10 == 5) {
			_flightIGT = levelGameTime * 2;
		}
        if (_currentButton & R3_BUTTON) {
            if (_spyro.isMovementLocked == 0) {
                _spyro.isMovementLocked = 1;
            }
            if (_currentButtonOneFrame & UP_BUTTON) {
                sharpTurnOffset++;
            } else if (_currentButtonOneFrame & DOWN_BUTTON) {
                sharpTurnOffset--;
            }
            calibrationInfoTimer = 60;
            if (CheckRegion() == PAL || sharpTurnOffset >= 0) {
                sprintf(&sharpTurnInfo, "OFFSET %d", sharpTurnOffset);
            } else {
                sprintf(&sharpTurnInfo, "OFFSET NEG. %d", (sharpTurnOffset * -1));
            }
        }
        if (calibrationInfoTimer > 0) {
            DrawTextAll(
                sharpTurnInfo,
                &(CapitalTextInfo){.x = 0x20, .y = SCREEN_BOTTOM_EDGE - 0x8, .size = DEFAULT_SIZE},
                &(LowercaseTextInfo){.spacing = DEFAULT_SPACING, .yOffset = 0, .size = DEFAULT_SIZE},
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
			DrawIGT(0x10, 0x50, 0, true, MOBY_COLOR_EMERALD_GREEN, LEFT);
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
            &(LowercaseTextInfo){.spacing = DEFAULT_SPACING, .yOffset = 0, .size = DEFAULT_SIZE},
            DEFAULT_SPACING,
            MOBY_COLOR_GOLD
        );
	}
    DrawTextAll(
        asciiTime,
        &(CapitalTextInfo){.x = alignmentOffset + xPosition, .y = yPosition, .size = DEFAULT_SIZE},
        &(LowercaseTextInfo){.spacing = DEFAULT_SPACING, .yOffset = 0, .size = DEFAULT_SIZE},
        DEFAULT_SPACING,
        colour
    );
    if (timerArray[0] > 0) {
        DrawTextAll(
            gameTimeStrings[2],
            &(CapitalTextInfo){.x = colonOffset(alignment) + xPosition, .y = yPosition -6, .size = DEFAULT_SIZE},
            &(LowercaseTextInfo){.spacing = DEFAULT_SPACING, .yOffset = 0, .size = DEFAULT_SIZE},
            DEFAULT_SPACING,
            colour
        );
    }
    if (timerArray[2] > 0 || timerArray[1] > 0 || timerArray[0] > 0) {
        DrawTextAll(
            gameTimeStrings[2],
            &(CapitalTextInfo){.x = colonOffset(alignment) + 0x36 + xPosition, .y = yPosition -6, .size = DEFAULT_SIZE},
            &(LowercaseTextInfo){.spacing = DEFAULT_SPACING, .yOffset = 0, .size = DEFAULT_SIZE},
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
	    char frameRate;
	    if (CheckRegion() == NTSC) {
	    	frameRate = 30;
	    } else {
	    	frameRate = 25;
	    }
	    timerArray[0] = inputFrameTime / (3600 * frameRate);
	    timerArray[2] = (inputFrameTime % (3600 * frameRate)) / (60 * frameRate);
	    timerArray[4] = (inputFrameTime % (60 * frameRate)) / frameRate;
	    timerArray[6] = ((inputFrameTime % frameRate) * 100 / frameRate);
	    i = 6;
	    while (i >= 2) {
	        if (timerArray[i] >= 10) {
	        	timerArray[i - 1] = timerArray[i] / 10;
	        	timerArray[i] = timerArray[i] % 10;
	        }
	    	i -= 2;
	    }
	    if (timerArray[0] < 10) {
	    	if (timerArray[0] > 0) {
	    		sprintf(&asciiTime, "%d.%d%d.%d%d.%d%d", timerArray[0], timerArray[1], timerArray[2], timerArray[3], timerArray[4], timerArray[5], timerArray[6]);
	    		alignmentOffset = 0;
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
	    	sprintf(&asciiTime, "9.59.59.99");
	    }
	} else {
	    alignmentOffset = 72;
		sprintf(&asciiTime, "......");
	}
}

int colonOffset(alignment) {
	if (alignment == RIGHT || timerArray[0] > 0) {
		return 18;
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
