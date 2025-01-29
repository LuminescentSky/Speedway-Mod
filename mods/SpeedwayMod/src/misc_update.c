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
char dogLoop = 0;
bool seenFlightCollectables = false;
int startLevel = 0;
char currentFlightCollectableFlags[5];

bool loopedMobys = false;

void MiscUpdate() {

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

    // Remove Tuco RNG
    if (_gameState == GAMESTATE_BALLOONIST) {
        if (_balloonistDialogState >= 33 && !collectedEggsPerLevel[INDEX_DRY_CANYON]) {
            // 33 = Stone Hill
            // 34 = Dry Canyon
            // 35 = Cliff Town
            // 36 = Magic Crafters
            // 37 = Wizard Peak 1
            // 38 = Wizard Peak 2
            _balloonistDialogState = 34;
        }
        // Reset Dry Canyon upon entering it via Tuco
        if (_altLevelID == DRY_CANYON_ID) {
            InitiateLevel();
        }
    }

    // No lives in homeworlds.
    if (_altLevelID % 10 == 0) {
        _globalLives = 99;
        _HUDElements.lifeHUDStatus = 0;
    }

    // Start level
    if (_gameState == GAMESTATE_LOADING && (_levelID % 10 != 0 || warping)) {
        startLevel = 1;
    } else if (startLevel == 1) {
        StopDrawWorldAndObjects();
        InitiateLevel();
        startLevel = 2;
        _gameState = GAMESTATE_DEATH;
    } else if (startLevel == 2 && _gameState == GAMESTATE_GAMEPLAY) {
        endingType = 0;
        gnastyTimer = 0;
        _spyro.absoluteMomentum = 0;
        startLevel = 0;
        if (soundEffectDuration == 3) {
            PlaySoundEffect(0x19, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
            soundEffectDuration = 0;
        }
        RestartDrawWorldAndObjects();
    }

    // IGT menu triggers
    if (_gameState == GAMESTATE_GAMEPLAY) {
        if (_levelID == GNASTYS_LOOT_ID) {
            _lootTimer = 50;
            if (_spyro.state != WHIRLWIND && customLootTimer != 0) {
                //if (CheckRegion() == NTSC) {
                //    customLootTimer = 30;
                //} else {
                    customLootTimer = 25;
                //}
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
                if (/*(CheckRegion() == NTSC && gnastyTimer == 29) || (CheckRegion() == PAL && */gnastyTimer == 24/*)*/) {
                    endingType = 1;
                    LoadIGTMenu(true);
                    if (!LevelComplete()) {
                        customMenuSelection = 0;
                    }
                }
            }
        } else if (LevelComplete() && _altLevelID % 10 != 5) {
            LoadIGTMenu(true);
        }
        // Putting the dragon code here reduces the amount of RAM use (causes a bit of a code mess but whatever)
        if (!loopedMobys) {
            Moby* mobyIteration = _ptrLevelMobys;
            char dragonLoop = 0;
            char dogLoop = 0;
            do {
                if (mobyIteration->type == 250) {
                    *(int*)((char*)mobyIteration->ptr_mobyData + 0x18) = -1;
                    dragonLoop++;
                }
                if (_altLevelID == TOASTY_ID && mobyIteration->type == 335) {
                    *(int*)((char*)mobyIteration->ptr_mobyData + 0x30) = 0;
                    dogLoop++;
                }
                mobyIteration++;
            } while ((dragonLoop < _maxDragonsPerLevel[realLevelIDIndex] && _altLevelID != GNASTYS_WORLD_ID) || (dogLoop < 17 && _altLevelID == TOASTY_ID));
            loopedMobys = true;
        }
        // Putting this here too because it reduces the amount of RAM use lmao
        DrawEggCount();
    } else if (_gameState == GAMESTATE_FLIGHT_LEVEL_MENU) {
        if (selectedQuitGame) {
            _gameState = GAMESTATE_PAUSED;
            _startMenuSelection = 1;
            _startMenuState = 2;
        } else {
            _flightLevelTimes[realLevelIDIndex / 6] = 0x1000000; // Only doing that so it shows "Press X Button" instead of retry
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
                    if (_flightLevelExpirationTimer == -2) {
                        timeTrialFailedReason = 2;
                    } else if (_quitFlightFromPause) {
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

    // Set the "Quit" option in the pause menu back to "Quit" from "Quit Game" whenever it's not selected
    if (_gameState != GAMESTATE_PAUSED || _startMenuSelection != 3 || _startMenuState != 0) {
        selectedQuitGame = false;
    }

    // Reset ending type and the dragon animation removal flag during load screens and deaths
    if (_levelLoadState != -1 || _gameState == GAMESTATE_DEATH) {
        _spyro.health = 3;
        endingType = 0;
        loopedMobys = false;
        xButtonFlag = 0;
        xButtonDuration = 0;
        StopSpinningDragonCounter();
    } else if (LevelComplete()) {
        // Set level complete flag
        completionFlags[realLevelIDIndex / 32] |= 1 << (realLevelIDIndex % 32);
        _vortexFlags[realLevelIDIndex] = 1;
    }

    // Patch Delbin Dragon Dupe
    if (collectedDragonsPerLevel[INDEX_GNASTYS_WORLD] == 2) {
        _gnastysWorldDragonFlag |= 1;
    }
}
