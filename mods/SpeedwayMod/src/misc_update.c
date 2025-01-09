#include <common.h>
#include <main_updates.h>
#include <multitap.h>
#include <shared_funcs.h>
#include <bg_colors.h>
#include <custom_text.h>
#include <sound.h>
#include <shared_variables.h>

char gnastyTimer = 0;
char endingType = 0;
char timeTrialFailedReason = 0;
char IGTpage = 0;
char newBestTime = false;
bool seenFlightCollectables = false;
int startLevel = 0;
char currentFlightCollectableFlags[5];

bool madeDragonsInstant = false;

void MiscUpdate() {

    // Allow dragons to be skipped instantly
    if (_levelLoadState != -1 || _gameState == GAMESTATE_DEATH) {
        madeDragonsInstant = false;
    }
    if (_gameState == GAMESTATE_GAMEPLAY && !madeDragonsInstant) {
        NoDragonAnimation();
        madeDragonsInstant = true;
    }
    if (collectedDragonsPerLevel[INDEX_GNASTYS_WORLD] == 2) {
        _gnastysWorldDragonFlag |= 1;
    }

    // Keep track of the level ID Index every frame as it is used to alter the inventory menu for level warping.
    // Save and load collectables every frame as long as the inventory isn't opened.
    if (_gameState != GAMESTATE_INVENTORY) {
        realLevelIDIndex = _levelIDIndex;
        SaveAndLoadCollectables();
    }

    // Freezing in an animation state that isn't loaded in another level before travelling there crashes the game
    // so setting isSupercharging during load screens to false is a safe way to avoid that and unfreeze animations.
    if (_gameState == GAMESTATE_LOADING) {
        isSupercharging = false;
        if (warping) {
            _levelID = warpLevelID;
        }
    }

    // No lives in homeworlds.
    if (_altLevelID % 10 == 0) {
        _globalLives = 99;
        _HUDlivesInfo = 0;
    }

    // Start level
    if (_gameState == GAMESTATE_LOADING && (_levelID % 10 != 0 || warping)) {
        startLevel = 1;
    } else if (startLevel == 1) {
        StopDrawWorldAndObjects();
        startLevel = 2;
        if (_levelID % 10 != 5 && _levelID % 10 != 0) {
            ResetLevelCollectables();
        }
        _globalLives = 0;
        _orbCount = 0;
        char i = 0;
        while (i < 5) {
            currentFlightCollectableFlags[i] = _permanentFlightCollectableFlags[realLevelIDIndex / 6 * 5 + i];
            i++;
        }
        if (_levelID == BLOWHARD_ID) {
            _blowhardPhase = 0;
        }
        levelGameTime = 0;
        _keyState = 0;
        _ptr_keyChestHostGem = NULL; // Key Glitch Fix
        warping = false;
        _gameState = GAMESTATE_DEATH;
    } else if (startLevel == 2 && _gameState == GAMESTATE_GAMEPLAY) {
        endingType = 0;
        gnastyTimer = 0;
        startLevel = 0;
        if (soundEffectDuration == 3) {
            PlaySoundEffect(0x19, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
            soundEffectDuration = 0;
        }
        RestartDrawWorldAndObjects();
        NoDragonAnimation();
    }

    // Custom menu screens extended
    if (customMenuState != NONE && customMenuState != RETURN_TO_TITLE) {
        DrawTextAll(
            stringList[9],
            &(CapitalTextInfo){.x = 0x2f, .y = 0x35, .size = 0x600},
            &(LowercaseTextInfo){.spacing = DEFAULT_SPACING + 20, .yOffset = 0, .size = 0x900},
            DEFAULT_SPACING + 28,
            MOBY_COLOR_PURPLE
        );
        if (customMenuState != TITLE_MOD_OPTIONS) {
	        DrawTextAll(
                stringList[10],
	            &(CapitalTextInfo){.x = SCREEN_RIGHT_EDGE - 0x80, .y = 0x54, .size = 0x1a00},
	            &(LowercaseTextInfo){.spacing = 0xb, .yOffset = 0, .size = 0x1a00},
	            0xb,
	            MOBY_COLOR_GREY
	        );
	    }
    }

    // IGT menu triggers
    if (_gameState == GAMESTATE_GAMEPLAY) {
        if (_levelID == GNASTYS_LOOT_ID) {
            _lootTimer = 50;
            if (_spyro.state != WHIRLWIND && customLootTimer != 0) {
                if (CheckRegion() == NTSC) {
                    customLootTimer = 30;
                } else {
                    customLootTimer = 25;
                }
            } else {
                customLootTimer--;
            }
            if (customLootTimer == 0 && endingType == 0 && (_globalGems == 14000 || LevelComplete())) {
                LoadIGTMenu(true);
                if (_globalGems == 14000) {
                    endingType = 2;
                    if (!LevelComplete()) {
                        customMenuSelection = 0;
                    }
                }
            } else if (endingType == 2) {
                _lootTimer = 0;
            }
        } else if (_levelID == GNASTY_GNORC_ID) {
            if (_gnastyGnorc == 9 && endingType == 0) {
                gnastyTimer++;
                if ((CheckRegion() == NTSC && gnastyTimer == 29) || (CheckRegion() == PAL && gnastyTimer == 24)) {
                    endingType = 1;
                    LoadIGTMenu(true);
                    if (!LevelComplete()) {
                        customMenuSelection = 0;
                    }
                }
            }
        } else if (LevelComplete() && _altLevelID % 10 != 5) {
            LoadIGTMenu(true);
        } else if (_altLevelID % 10 == 5 && _flightLevelExpirationTimer == 0) {
            timeTrialFailedReason = 2;
        }
    } else if (_gameState == GAMESTATE_FLIGHT_LEVEL_MENU) {
        if (selectedQuitGame) {
            _gameState = GAMESTATE_PAUSED;
            _startMenuSelection = 1;
            _startMenuState = 2;
        } else {
            if (_flightLevelTimes[realLevelIDIndex / 6] == 0) {
                _flightLevelTimes[realLevelIDIndex / 6] = 0x1000000;
            }
            if (_flightMenuState != 100) {
                seenFlightCollectables = true;
            } else if (_flightMenuState == 100) {
                char i = 0;
                while (i < 5) {
                    _permanentFlightCollectableFlags[realLevelIDIndex / 6 * 5 + i] = currentFlightCollectableFlags[i];
                    i++;
                }
                if (warping) {
                    PrepareWarping();
                } else {
                    if (_quitFlightFromPause) {
                        timeTrialFailedReason = 3;
                    } else if (!LevelComplete() && timeTrialFailedReason != 2) {
                        timeTrialFailedReason = 4;
                    }
                    LoadIGTMenu(false);
                }
                seenFlightCollectables = false;
            }
        }
    }

    if (_gameState != GAMESTATE_PAUSED || _startMenuSelection != 3 || _startMenuState != 0) {
        selectedQuitGame = false;
    }

    if (_gameState == GAMESTATE_LOADING || _gameState == GAMESTATE_CREDITS || _gameState == GAMESTATE_DEATH) {
        endingType = 0;
    }
}
