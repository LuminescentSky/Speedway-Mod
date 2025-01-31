#include <common.h>
#include <main_updates.h>
#include <multitap.h>
#include <shared_funcs.h>
#include <bg_colors.h>
#include <custom_text.h>
#include <sound.h>
#include <shared_variables.h>

unsigned char levelStringXPosition[35] = {
    0xc1,   // ARTISANS
    0xb2,   // STONE HILL
    0xa9,   // DARK HOLLOW
    0xa9,   // TOWN SQUARE
    0xd3,   // TOASTY
    0xa0,   // SUNNY FLIGHT
    0x97,   // PEACE KEEPERS
    0xb2,   // DRY CANYON
    0xb2,   // CLIFF TOWN
    0xb2,   // ICE CAVERN
    0xa0,   // DOCTOR SHEMP
    0xa0,   // NIGHT FLIGHT
    0x8e,   // MAGIC CRAFTERS
    0xa0,   // ALPINE RIDGE
    0xb2,   // HIGH CAVES
    0xa9,   // WIZARD PEAK
    0xc1,   // BLOWHARD
    0x8e,   // CRYSTAL FLIGHT
    0xa0,   // BEAST MAKERS
    0x85,   // TERRACE VILLAGE
    0xbb,   // MISTY BOG
    0xbb,   // TREE TOPS
    0xb8,   // METALHEAD
    0xa9,   // WILD FLIGHT
    0x97,   // DREAM WEAVERS
    0xa0,   // DARK PASSAGE
    0xa0,   // LOFTY CASTLE
    0x8e,   // HAUNTED TOWERS
    0xca,   // JACQUES
    0xb2,   // ICY FLIGHT
    0x8e,   // GNASTY'S WORLD
    0xb2,   // GNORC COVE
    0x7c,   // TWILIGHT HARBOUR
    0xa0,   // GNASTY GNORC
    0x97    // GNASTY'S LOOT
};

bool warping = false;
bool selectedQuitGame = false;
int warpLevelID = 0;
LowercaseTextInfo defaultLowerCase = {.spacing = DEFAULT_SPACING, .yOffset = 0, .size = DEFAULT_SIZE};


//Not here to stay but just so freeing dragons doesn't break the game for now:


// Hooked on OpenInventory() RETURN
void OpenInventoryHook() {
    if (guidebookDialogState == INVENTORY) {
        SaveAndLoadCollectables();
        previousInventoryPage = 10;
    }
}

// Hooked on maybe_drawMenuText() CALL
void BeforeMenuText() {
    if (_gameState == GAMESTATE_PAUSED) {
        if (!selectedQuitGame) {
            if (_altLevelID % 10 == 5) {
                memcpy(&_hook_quit, &_hook_exitLevel, 0x24);
            } else {
                memcpy(&_hook_exitLevel, &_hook_quit, 0x24);
            }
        } else {
            if (_altLevelID % 10 == 5) {
                memcpy(&_hook_quit, &_hook_quitGame, 0x24);
            } else {
                memcpy(&_hook_exitLevel, &_hook_quitGame, 0x24);
            }
        }
    }
    if (_gameState == GAMESTATE_INVENTORY) {
        if (guidebookDialogState == INVENTORY) {
            _levelIDIndex = inventoryIndex;
            _midInventoryHookAddress = 0x0c00c2d6;
        } else {
            _midInventoryHookAddress = 0x0800839a;
        }
    }
    maybe_drawMenuText();
}

// Hooked INSIDE maybe_drawMenuText()
void MenuTextMain() {
    char completionString;
    int completionPosition;
    _setTextBoxTransparency = 0x24020040;
    if (_gameState == GAMESTATE_INVENTORY) {
        if (guidebookDialogState == INVENTORY) {
            //if (CheckRegion() == NTSC) {
                //DrawIGT(0x10, 0x10, 0, false, MOBY_COLOR_GOLD, LEFT);
            //} else {
                DrawIGT(0x10, 0xa, 0, false, MOBY_COLOR_GOLD, LEFT);
            //}
        } else if (guidebookDialogState == CONFIRM_WARP) {
            DrawTextBox(0x60, SCREEN_RIGHT_EDGE - 0x60, 0x38, SCREEN_BOTTOM_EDGE - 0x30);
            // TRAVEL TO
            DrawTextAll(
                stringList[14],
                &(CapitalTextInfo){.x = 0xbb, .y = 0x48, .size = DEFAULT_SIZE},
                &defaultLowerCase,
                DEFAULT_SPACING,
                MOBY_COLOR_GOLD
            );
            if (inventoryIndex % 6 != 0) {
                DrawTextAll(
                    (&_ptr_levelName)[inventoryIndex],
                    &(CapitalTextInfo){.x = levelStringXPosition[inventoryIndex] - 9, .y = 0x60, .size = DEFAULT_SIZE},
                    &defaultLowerCase,
                    DEFAULT_SPACING,
                    MOBY_COLOR_PURPLE
                );
            } else {
                DrawTextAll(
                    (&_ptr_homeworldName)[inventoryIndex / 6],
                    &(CapitalTextInfo){.x = levelStringXPosition[inventoryIndex] - 9, .y = 0x60, .size = DEFAULT_SIZE},
                    &defaultLowerCase,
                    DEFAULT_SPACING,
                    MOBY_COLOR_PURPLE
                );
            }
            // ?
            DrawTextAll(
                stringList[15],
                &(CapitalTextInfo){.x = SCREEN_RIGHT_EDGE + 9 - levelStringXPosition[inventoryIndex], .y = 0x60, .size = DEFAULT_SIZE},
                &defaultLowerCase,
                DEFAULT_SPACING,
                MOBY_COLOR_GOLD
            );
            // TRAVEL
            DrawTextAll(
                stringList[16],
                &(CapitalTextInfo){.x = 0xd3, .y = 0x90, .size = DEFAULT_SIZE},
                &defaultLowerCase,
                DEFAULT_SPACING,
                customMenuColorArray[0]
            );
            // STAY
            DrawTextAll(
                stringList[17],
                &(CapitalTextInfo){.x = 0xe5, .y = 0xa8, .size = DEFAULT_SIZE},
                &defaultLowerCase,
                DEFAULT_SPACING,
                customMenuColorArray[1]
            );
        } else if (guidebookDialogState == UNLOCKED) {
            DrawTextBox(0x60, SCREEN_RIGHT_EDGE - 0x60, 0x38, SCREEN_BOTTOM_EDGE - 0x48);
            // LEVEL WARPING
            DrawTextAll(
                stringList[18],
                &(CapitalTextInfo){.x = 0x97, .y = 0x48, .size = DEFAULT_SIZE},
                &defaultLowerCase,
                DEFAULT_SPACING,
                MOBY_COLOR_GOLD
            );
            // UNLOCKED
            DrawTextAll(
                stringList[19],
                &(CapitalTextInfo){.x = 0xc1, .y = 0x60, .size = DEFAULT_SIZE},
                &defaultLowerCase,
                DEFAULT_SPACING,
                MOBY_COLOR_GOLD
            );
            // DONE
            DrawTextAll(
                &_DONE,
                &(CapitalTextInfo){.x = 0xe5, .y = 0x90, .size = DEFAULT_SIZE},
                &defaultLowerCase,
                DEFAULT_SPACING,
                customMenuColorArray[0]
            );
        } else if (guidebookDialogState == IGT_DISPLAY) {
            if (endingType != 0 && timeTrialFailedReason == 0 && !LevelComplete()) {
                IGTpage = 1;
            }
            if (LevelComplete() && (levelGameTime < bestTimes[realLevelIDIndex] || bestTimes[realLevelIDIndex] == 0)) {
                newBestTime = true;
            }
            if (timeTrialFailedReason == 2) {
                // TIME IS UP
                DrawTextAll(
                    &_TIME_IS_UP,
                    &(CapitalTextInfo){.x = 0x84, .y = 0x3c, .size = 0xa00},
                    &(LowercaseTextInfo){.spacing = DEFAULT_SPACING + 12, .yOffset = 0, .size = 0xa00},
                    DEFAULT_SPACING + 12,
                    MOBY_COLOR_SHINY_PURPLE
                );
            } else if (timeTrialFailedReason == 3) {
                // QUIT
                DrawTextAll(
                    &_QUIT,
                    &(CapitalTextInfo){.x = 0xd4, .y = 0x3c, .size = 0xa00},
                    &(LowercaseTextInfo){.spacing = DEFAULT_SPACING + 12, .yOffset = 0, .size = 0xa00},
                    DEFAULT_SPACING + 12,
                    MOBY_COLOR_SHINY_PURPLE
                );
            } else if (timeTrialFailedReason == 4) {
                // CRASHED
                DrawTextAll(
                    &_CRASHED,
                    &(CapitalTextInfo){.x = 0xa7, .y = 0x3c, .size = 0xa00},
                    &(LowercaseTextInfo){.spacing = DEFAULT_SPACING + 12, .yOffset = 0, .size = 0xa00},
                    DEFAULT_SPACING + 12,
                    MOBY_COLOR_SHINY_PURPLE
                );
            } else if (newBestTime && IGTpage == 0) {
                // NEW BEST TIME
                DrawTextAll(
                    stringList[20],
                    &(CapitalTextInfo){.x = 0x57, .y = 0x3c, .size = 0xa00},
                    &(LowercaseTextInfo){.spacing = DEFAULT_SPACING + 12, .yOffset = 0, .size = 0xa00},
                    DEFAULT_SPACING + 12,
                    MOBY_COLOR_SHINY_PURPLE
                );
            } else {
                // TIME ATTACK
                DrawTextAll(
                    &_TIME_ATTACK,
                    &(CapitalTextInfo){.x = 0x70, .y = 0x3c, .size = 0xa00},
                    &(LowercaseTextInfo){.spacing = DEFAULT_SPACING + 12, .yOffset = 0, .size = 0xa00},
                    DEFAULT_SPACING + 12,
                    MOBY_COLOR_SHINY_PURPLE
                );
            }
            if (IGTpage == 1) {
                if (endingType == 1) {
                    // BEAT THE GAME
                    DrawTextAll(
                        stringList[13],
                        &(CapitalTextInfo){.x = 0x9a, .y = 0x58, .size = DEFAULT_SIZE},
                        &defaultLowerCase,
                        DEFAULT_SPACING,
                        MOBY_COLOR_PURPLE
                    );
                } else {
                    /*if (CheckRegion() == NTSC) {
                        completionString = 11;
                        completionPosition = 0x76;
                    } else {
                        completionString = 9;
                        completionPosition = 0x6d;
                    }*/
                    // 120% GAME COMPLETE
                    DrawTextAll(
                        stringList[9],
                        &(CapitalTextInfo){.x = 0x6d, .y = 0x58, .size = DEFAULT_SIZE},
                        &defaultLowerCase,
                        DEFAULT_SPACING,
                        MOBY_COLOR_PURPLE
                    );
                }
            } else {
                // LEVEL NAME
                DrawTextAll(
                    (&_ptr_levelName)[realLevelIDIndex],
                    &(CapitalTextInfo){.x = levelStringXPosition[realLevelIDIndex], .y = 0x58, .size = DEFAULT_SIZE},
                    &defaultLowerCase,
                    DEFAULT_SPACING,
                    MOBY_COLOR_PURPLE
                );
                if (LevelComplete()) {
                    // LEVEL COMPLETE
                    DrawTextAll(
                        stringList[12],
                        &(CapitalTextInfo){.x = 0x8e, .y = 0x70, .size = DEFAULT_SIZE},
                        &defaultLowerCase,
                        DEFAULT_SPACING,
                        MOBY_COLOR_PURPLE
                    );
                }
            }
            if (IGTpage == 1) {
                // your time (game beaten)
                DrawTextAll(
                    &_YOUR_TIME,
                    &(CapitalTextInfo){.x = 0xbb, .y = 0x70, .size = DEFAULT_SIZE},
                    &defaultLowerCase,
                    DEFAULT_SPACING,
                    MOBY_COLOR_GOLD
                );
                DrawIGT(0xaf, 0x88, 0, false, MOBY_COLOR_GOLD, CENTRE);
            } else {
                // your time (IL)
                DrawTextAll(
                    &_YOUR_TIME,
                    &(CapitalTextInfo){.x = 0x5c, .y = 0x88, .size = DEFAULT_SIZE},
                    &defaultLowerCase,
                    DEFAULT_SPACING,
                    MOBY_COLOR_GOLD
                );
                if (LevelComplete()) {
                    DrawIGT(0x50, 0xa0, 1, false, MOBY_COLOR_GOLD, CENTRE);
                } else {
                    DrawIGT(0x50, 0xa0, 3, false, MOBY_COLOR_GOLD, CENTRE);
                }
                // best time (IL)
                DrawTextAll(
                    &_BEST_TIME,
                    &(CapitalTextInfo){.x = 0x11b, .y = 0x88, .size = DEFAULT_SIZE},
                    &defaultLowerCase,
                    DEFAULT_SPACING,
                    MOBY_COLOR_GOLD
                );
                if (newBestTime && bestTimes[realLevelIDIndex] == 0) {
                    DrawIGT(0x10f, 0xa0, 1, false, MOBY_COLOR_GOLD, CENTRE);
                } else {
                    DrawIGT(0x10f, 0xa0, 2, false, MOBY_COLOR_GOLD, CENTRE);
                }
            }
            if (IGTpage == 1) {
                DrawTextBox(0x40, SCREEN_RIGHT_EDGE - 0x40, 0x28, 0x94);
                // CONTINUE TO CUTSCENE
                DrawTextAll(
                    stringList[10],
                    &(CapitalTextInfo){.x = 0x5b, .y = SCREEN_BOTTOM_EDGE - 0x40, .size = DEFAULT_SIZE},
                    &defaultLowerCase,
                    DEFAULT_SPACING,
                    customMenuColorArray[0]
                );
            } else {
                DrawTextBox(0x40, SCREEN_RIGHT_EDGE - 0x40, 0x28, 0xac);
                if (endingType != 0) {
                    // CONTINUE
                    DrawTextAll(
                        &_CONTINUE,
                        &(CapitalTextInfo){.x = 0xc1, .y = SCREEN_BOTTOM_EDGE - 0x28, .size = DEFAULT_SIZE},
                        &defaultLowerCase,
                        DEFAULT_SPACING,
                        customMenuColorArray[1]
                    );
                }
            }
            if (endingType == 0 || IGTpage == 1) {
                // RETRY
                DrawTextAll(
                    &_RETRY,
                    &(CapitalTextInfo){.x = 0xdc, .y = SCREEN_BOTTOM_EDGE - 0x28, .size = DEFAULT_SIZE},
                    &defaultLowerCase,
                    DEFAULT_SPACING,
                    customMenuColorArray[1]
                );
                // EXIT LEVEL
                DrawTextAll(
                    &_EXIT_LEVEL,
                    &(CapitalTextInfo){.x = 0xb2, .y = SCREEN_BOTTOM_EDGE - 0x10, .size = DEFAULT_SIZE},
                    &defaultLowerCase,
                    DEFAULT_SPACING,
                    customMenuColorArray[2]
                );
            }
        }
    } else if (_gameState == GAMESTATE_PAUSED) {
        /*DrawTextAll(
            "UEUE",
            &(CapitalTextInfo){.x = 0x150, .y = 0x78, .size = 0x1300},
            &(LowercaseTextInfo){.spacing = DEFAULT_SPACING - 1, .yOffset = 0, .size = 0x1300},
            DEFAULT_SPACING - 1,
            MOBY_COLOR_GOLD
        );*/
        DrawEggCount();
        if (_startMenuState == 0) {
            if (realLevelIDIndex % 6 != 0) {
                DrawTextAll(
                    (&_ptr_levelName)[realLevelIDIndex],
                    &(CapitalTextInfo){.x = levelStringXPosition[realLevelIDIndex], .y = SCREEN_BOTTOM_EDGE - 0x20, .size = DEFAULT_SIZE},
                    &defaultLowerCase,
                    DEFAULT_SPACING,
                    MOBY_COLOR_SHINY_PURPLE
                );
                DrawIGT(0xaf, SCREEN_BOTTOM_EDGE - 0x8, 1, false, MOBY_COLOR_GOLD, CENTRE);
                if (_startMenuSelection == 3) {
                    DrawArrow(
                        &(CapitalTextInfo){.x = 0x88, .y = SCREEN_BOTTOM_EDGE - 0x38, .size = DEFAULT_SIZE},
                        _menuTimer << 1,
                        1
                    );
                    DrawArrow(
                        &(CapitalTextInfo){.x = SCREEN_RIGHT_EDGE - 0x88, .y = SCREEN_BOTTOM_EDGE - 0x38, .size = DEFAULT_SIZE},
                        _menuTimer << 1,
                        0
                    );
                }
            } else {
                DrawTextAll(
                    (&_ptr_homeworldName)[realLevelIDIndex / 6],
                    &(CapitalTextInfo){.x = levelStringXPosition[realLevelIDIndex], .y = SCREEN_BOTTOM_EDGE - 0x18, .size = DEFAULT_SIZE},
                    &defaultLowerCase,
                    DEFAULT_SPACING,
                    MOBY_COLOR_SHINY_PURPLE
                );
            }
        }
    }
    _levelIDIndex = realLevelIDIndex;
    _setTextBoxTransparency = 0x24020070;
    CopyHudToShaded();
}

// Hooked on PauseMenuUpdate() CALL
void BeforePauseMenuUpdate() {
    if (_globalEggs > 0) {
        HideEggs();
    }
    if (_startMenuSelection == 3 && _altLevelID % 10 != 0) {
        if (_currentButtonOneFrame & LEFT_BUTTON || _currentButtonOneFrame & RIGHT_BUTTON) {
            selectedQuitGame = !selectedQuitGame;
            PlaySoundEffect(0x19, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
        }
    }
    PauseMenuUpdate();
}

// Hooked on InventoryUpdate() CALL
void BeforeInventoryUpdate() {
    bool pageTransitionSelection = false;
    if (guidebookDialogState == INVENTORY) {
        if (previousInventoryPage != _inventoryPage) {
            if (realLevelIDIndex / 6 == _inventoryPage) {
                inventoryIndex = realLevelIDIndex;
            } else if (warpingUnlocked) {
                inventoryIndex = _inventoryPage * 6;
            }
            previousInventoryPage = _inventoryPage;
        }
        if (warpingUnlocked) {
            if (!_inventoryPageTransition) {
                if (_currentButtonOneFrame & UP_BUTTON) {
                    inventoryIndex--;
                    if (inventoryIndex == 29) {
                        inventoryIndex = 34;
                    }
                    if ((inventoryIndex % 6 + 6) % 6 == 5) {
                        inventoryIndex += 6;
                    }
                    PlaySoundEffect(0x19, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
                }
                if (_currentButtonOneFrame & DOWN_BUTTON) {
                    inventoryIndex++;
                    if (inventoryIndex % 6 == 0) {
                        inventoryIndex -= 6;
                    }
                    if (inventoryIndex == 35) {
                        inventoryIndex = 30;
                    }
                    PlaySoundEffect(0x19, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
                }
            }
            if (_currentButtonOneFrame & X_BUTTON || _currentButtonOneFrame & START_BUTTON) {
                if (_inventoryPageTransition) {
                    pageTransitionSelection = true;
                } else if (inventoryIndex != realLevelIDIndex) {
                    PlaySoundEffect(0x1a, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
                    guidebookDialogState = CONFIRM_WARP;
                    customMenuColorArray[customMenuSelection] = MOBY_COLOR_GOLD;
                    customMenuSelection = 1;
                }
            }
        }
        if (guidebookDialogState == INVENTORY && !pageTransitionSelection) {
            InventoryUpdate();
        }
    } else if (guidebookDialogState == UNLOCKED) {
        if (_currentButtonOneFrame & X_BUTTON || _currentButtonOneFrame & START_BUTTON || _currentButtonOneFrame & TRIANGLE_BUTTON) {
            guidebookDialogState = INVENTORY;
            PlaySoundEffect(0x1a, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
        }

    } else if (guidebookDialogState == CONFIRM_WARP) {
        if (_currentButtonOneFrame & UP_BUTTON || _currentButtonOneFrame & DOWN_BUTTON) {
            customMenuColorArray[customMenuSelection] = MOBY_COLOR_GOLD;
            customMenuSelection++;
            if (customMenuSelection == 2) {
                customMenuSelection = 0;
            }
            PlaySoundEffect(0x1a, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
        }
        if (_currentButtonOneFrame & X_BUTTON || _currentButtonOneFrame & START_BUTTON) {
            if (customMenuSelection == 0) {
                StopSounds(0);
                PlaySoundEffect(0x19, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
                guidebookDialogState = INVENTORY;
                UndoInventoryChanges();
                warping = true;
                if (_levelID % 10 == 5) {
                    _gameState = GAMESTATE_FLIGHT_LEVEL_MENU;
                    _quitFlightFromPause = true;
                    _flightMenuState = 0;
                } else {
                    PrepareWarping();
                }
                return;
            } else {
                _currentButtonOneFrame |= TRIANGLE_BUTTON;
            }
        }
        if (_currentButtonOneFrame & TRIANGLE_BUTTON) {
            guidebookDialogState = INVENTORY;
            PlaySoundEffect(0x19, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
        }
    } else if (guidebookDialogState == IGT_DISPLAY) {
        if (soundEffectDuration == 1) {
            StopSounds(0);
            soundEffectDuration = 0;
        }
        if (_menuTimer > 4) {
            if ((_currentButtonOneFrame & TRIANGLE_BUTTON) && LevelComplete() && IGTpage == 1) {
                customMenuColorArray[customMenuSelection] = MOBY_COLOR_GOLD;
                IGTpage = 0;
                customMenuSelection = 1;
                PlaySoundEffect(0x19, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
            }
            if (!(endingType != 0 && IGTpage == 0) && (_currentButtonOneFrame & UP_BUTTON || _currentButtonOneFrame & DOWN_BUTTON)) {
                customMenuColorArray[customMenuSelection] = MOBY_COLOR_GOLD;
                PlaySoundEffect(0x19, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
                soundEffectDuration = 1;
                if (endingType == 0) {
                    customMenuSelection++;
                    if (customMenuSelection == 3) {
                        customMenuSelection = 1;
                    }
                } else {
                    if (_currentButtonOneFrame & UP_BUTTON) {
                        customMenuSelection--;
                    } else if (_currentButtonOneFrame & DOWN_BUTTON) {
                        customMenuSelection++;
                    }
                    if (customMenuSelection == 3) {
                        customMenuSelection = 0;
                    } else if (customMenuSelection == -1) {
                        customMenuSelection = 2;
                    }
                }
            }
            if (_currentButtonOneFrame & START_BUTTON || _currentButtonOneFrame & X_BUTTON) {
                customMenuColorArray[customMenuSelection] = MOBY_COLOR_GOLD;
                if (customMenuSelection == 0) {
                    // CONTINUE TO CUTSCENE
                    IGTpage = 0;
                    guidebookDialogState = INVENTORY;
                    timeTrialFailedReason = 0;
                    InventoryToGameplay();
                } else if (customMenuSelection == 1) {
                    if (endingType == 0 || IGTpage == 1) {
                        // RETRY
                        IGTpage = 0;
                        startLevel = 1;
                        retrying = true;
                        guidebookDialogState = INVENTORY;
                        timeTrialFailedReason = 0;
                        soundEffectDuration = 3;
                        InventoryToGameplay();
                    } else {
                        // CONTINUE
                        IGTpage = 1;
                        customMenuSelection = 0;
                        PlaySoundEffect(0x19, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
                    }
                } else {
                    // EXIT LEVEL
                    IGTpage = 0;
                    timeTrialFailedReason = 0;
                    guidebookDialogState = INVENTORY;
                    PlaySoundEffect(0x19, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
                    ExitLevel();
                }
                if (newBestTime && guidebookDialogState != IGT_DISPLAY) {
                    bestTimes[realLevelIDIndex] = levelGameTime;
                    newBestTime = false;
                }
            }
        }
    }
    if (guidebookDialogState != INVENTORY || pageTransitionSelection) {
        TextAnimation(3);
    }
}

// Hooked inside PauseMenuUpdate() at Exit Level
void QuitSpeedway() {
    if (!selectedQuitGame) {
        timeTrialFailedReason = 3;
        LoadIGTMenu(false);
    } else {
        _startMenuSelection = 1;
        _startMenuState = 2;
    }
}

// Hooked on InventoryCheatCodes() CALL
void BeforeCheatCode(int cheatCode) {
    if (cheatCode == 3) {
        StopSounds(0);
        _enteredLevelFlags[INDEX_TWILIGHT_HARBOR] = true;
        _enteredLevelFlags[INDEX_GNASTY_GNORC] = true;
        _enteredLevelFlags[INDEX_GNASTYS_LOOT] = true;
        guidebookDialogState = UNLOCKED;
        customMenuColorArray[customMenuSelection] = MOBY_COLOR_GOLD;
        customMenuSelection = 0;
        warpingUnlocked = true;
    }
    InventoryCheatCodes(cheatCode);
}

// Hooked on PausingRelated() CALL leaving the Inventory
void InventoryToPause(int uselessVariable) {
    if (_globalEggs > 0) {
        HideEggs();
    }
    UndoInventoryChanges();
    PausingRelated(uselessVariable);
}

// Hooked on InventoryToGameplay() CALL leaving the Inventory
void InventoryToGameplayHook() {
    UndoInventoryChanges();
    InventoryToGameplay();
    if (newDragon) {
        _HUDElements.dragonHUDStatus = 1;
        _HUDElements.dragonHUDKeyframe = 0;
    }
}

// Hooked on LoadCredits() CALL in the inventory cheat code function
void InventoryToCredits(int uselessVariable) {
    UndoInventoryChanges();
    LoadCredits(uselessVariable);
}

// Hooked on HUDUpdates() CALL in GameStateUpdates()
void BeforeHUDUpdate() {
    if (currentEggs == _eggsPerLevel[_levelIDIndex]) {
        HideEggs();
    }
    HUDUpdate();
}

// Hooked on HUDUpdates() RETURN
void HUDUpdateReturn() {
    _globalEggs = realGlobalEggs;
}
