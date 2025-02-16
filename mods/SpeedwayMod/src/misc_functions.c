#include <common.h>
#include <main_updates.h>
#include <multitap.h>
#include <shared_funcs.h>
#include <bg_colors.h>
#include <custom_text.h>
#include <sound.h>
#include <shared_variables.h>

char soundEffectDuration = false;

int bestTimes[35];

short collectedGemsPerLevel[35];
char collectedDragonsPerLevel[35];
char collectedEggsPerLevel[18];

short currentGems;
char currentDragons;
char currentEggs;

bool warpingUnlocked = false;
char inventoryIndex = 0;
char realLevelIDIndex = 0;
char previousInventoryPage = 0;

bool spinDragonCounter;
short spinVar = 256;
char startingDigit = 5;
char totalDigits = 1;
char realGlobalEggs;
char customEggsHUD;
bool newDragon;

GuidebookDialogState guidebookDialogState = INVENTORY;

// Region specific Variables (default PAL):
/*char exitLevelInstructionsSize = 0x24;
int originalInventoryInstructions = 0x0c00c2d6;
int skipInventoryInstructions = 0x0800839a;*/

void UndoInventoryChanges() {
    _gemsPerLevel[_levelIDIndex] = currentGems;
    _dragonsPerLevel[_levelIDIndex] = currentDragons;
    _eggsPerLevel[_levelIDIndex] = currentEggs;
}

void BeforeDeath() {
    if (_globalLives == 0) {
        timeTrialFailedReason = 4;
        if (_altLevelID % 10 == 5) {
            OpenFlightMenu();
        } else {
            LoadIGTMenu(true);
        }
    } else {
        SetGamestateDeath();
    }
}

void PreventStopSound(int uselessVariable) {
}

void PreventStopSoundCutscene(int uselessVariable) {
    StopSounds(0);
    PlaySoundEffect(0x19, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
}

//This runs The Adventure Begins cutscene
void SkipIntro()
{
    char i = 0;
    _isPastTitleScreen = TRUE;                            //? This flag is checked by TheAdventureBeings() before it runs, so we must set this flag to TRUE.
    TheAdventureBegins();                                 //Call The Adventure Begins cutscene sequence      
    setDefaultOffset();
    menuColorR = 0x48;
    menuColorG = 0x18;
    while (i <= 35) {
        collectedGemsPerLevel[i] = 0;
        collectedDragonsPerLevel[i] = 0;
        if (i < 18) {
            collectedEggsPerLevel[i] = 0;
        }
        i++;
    }
    completionFlags[0] = 0;
    completionFlags[1] = 0;
    customEggsHUD = 0;
    _dragonsHUD = 0;
    _artisansLiveFlag |= 0x40000000;
    _peaceKeepersLiveFlag |= 0x04000000;
    _magicCraftersLiveFlag |= 0x00400000;
    _beastMakersLiveFlag |= 0x01000000;
    _dreamWeaversLiveFlag |= 0x00000880;
    _keyState = 0;
    _ptr_keyChestHostGem = NULL; // Key Glitch Fix
    warpingUnlocked = false;
    theEgg = 0x4F59414A;
    /*if (CheckRegion() == NTSC) {
        exitLevelInstructionsSize = 0x2c;
        originalInventoryInstructions = 0x0c00aec8;
        skipInventoryInstructions = 0x0800714f;
    }*/
    memcpy(&_hook_exitLevel, &_hook_quit, 0x24);
    sprintf(&_inventoryString, "GUIDEBOOK");
}

void LoadIGTMenu(bool comingFromGameplay) {
    guidebookDialogState = IGT_DISPLAY;
    customMenuColorArray[customMenuSelection] = MOBY_COLOR_GOLD;
    customMenuSelection = 1;
    if (comingFromGameplay) {
        OpenInventory(1);
        StopSounds(0);
    } else {
        _gameState = GAMESTATE_INVENTORY;
    }
    if (timeTrialFailedReason == 0 && _altLevelID % 10 != 5) {
        soundEffectDuration = 0;
        PlaySoundEffect(0xf, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
    } else if (timeTrialFailedReason == 3 && !seenFlightCollectables) {
        soundEffectDuration = 1;
    }
}

void PrepareWarping() {
    warpLevelID = inventoryIndex / 6 * 10 + 10 + inventoryIndex % 6;
    _altLevelID = 0;
    ExitLevel(0);
}

void InitiateLevel() {
    if (_levelID % 10 != 5 && _levelID % 10 != 0) {
        ResetLevelCollectables();
    }
    _globalLives = 0;
    _orbCount = 0;
    if (_levelID % 10 == 5) {
        char i = 0;
        while (i < 5) {
            currentFlightCollectableFlags[i] = _permanentFlightCollectableFlags[realLevelIDIndex / 6 * 5 + i];
            i++;
        }
        if (completionFlags[realLevelIDIndex / 32] & (1 << (realLevelIDIndex % 32))) {
            // Not keeping track of IGT PBs with the built in variables anyway so not gonna matter.
            // I'm only using them as "flags" now to have the top right countdown/timer work the way I need it to.
            _flightLevelTimes[realLevelIDIndex / 6] = 0x1000000;
        } else {
            _flightLevelTimes[realLevelIDIndex / 6] = 0;
        }
        
    }
    if (_levelID == BLOWHARD_ID) {
        _blowhardPhase = 0;
    }
    levelGameTime = 0;
    _keyState = 0;
    _ptr_keyChestHostGem = NULL; // Key Glitch Fix
    warping = false;
}

void LoadCustomTitleScreen() {
    StopSounds(0);
    _gameState = GAMESTATE_TITLE_SCREEN;
    _unkVar1 = 3;
    _unkVar2 = 1;
    customMenuState = TITLE;
}

void ReturnToCustomTitleScreen() {
    StopSounds(0);
    _startMenuSelection = 1 - _startMenuSelection;
    _gameState = GAMESTATE_TITLE_SCREEN;
    customMenuState = RETURN_TO_TITLE;
    customMenuColorArray[customMenuSelection] = MOBY_COLOR_GOLD;
    customMenuSelection = 0;
}

void ResetLevelCollectables() {
    memset((int *)(&_collectablesBitflags + _levelIDIndex * 8), 0, 0x20);
    _gemsPerLevel[_levelIDIndex] = 0;
    _dragonsPerLevel[_levelIDIndex] = 0;
    if (_levelID < 40 ) {
        _eggsPerLevel[_levelIDIndex] = 0;
    }
    _shouldRespawnAtCheckpoint = false;
}

void SaveAndLoadCollectables() {
    
    // loot testing
    /*_gemsPerLevel[INDEX_ARTISANS] = 14000;
    if (_gameState != GAMESTATE_CUTSCENE) {
        _lootKey4 = 3;
    }*/
    if (_altLevelID % 10 == 5) {
        char i = 0;
        while (i < 4) {
            if (_flightCollectedItems[i] == 8 && currentFlightCollectableFlags[i] == 0) {
                currentFlightCollectableFlags[i] = 1;
                _gemsPerLevel[realLevelIDIndex] += 60;
                AddVisualFlightGems();
            }
            i++;
        }
        if (LevelComplete() && currentFlightCollectableFlags[4] == 0) {
            currentFlightCollectableFlags[4] = 1;
            _gemsPerLevel[realLevelIDIndex] += 60;
            AddVisualFlightGems();
        }
    }
    if (_gameState == GAMESTATE_PAUSED) {
        StopSpinningDragonCounter();
    }
    if (currentDragons < _dragonsPerLevel[_levelIDIndex]) {
        if (_gameState == GAMESTATE_GAMEPLAY) {
            // I love nested if statements just to fix frame perfect visual bugs lmao
            PlaySoundEffect(30, 0, SOUND_PLAYBACK_MODE_NORMAL, 0);
        }
        if (_HUDElements.dragonHUDStatus != 2) {
            _HUDElements.dragonHUDStatus = 1;
            spinVar += 16;
        }
        newDragon = true;
        _dragonHUDTimer = 0;
    }
    currentGems = _gemsPerLevel[_levelIDIndex];
    currentDragons = _dragonsPerLevel[_levelIDIndex];
    currentEggs = _eggsPerLevel[_levelIDIndex];
    if (_gemsPerLevel[_levelIDIndex] > collectedGemsPerLevel[_levelIDIndex]) {
        collectedGemsPerLevel[_levelIDIndex] = _gemsPerLevel[_levelIDIndex];
    }
    if (_dragonsPerLevel[_levelIDIndex] > collectedDragonsPerLevel[_levelIDIndex]) {
        collectedDragonsPerLevel[_levelIDIndex] = _dragonsPerLevel[_levelIDIndex];
    }
    if (_levelID < 40 && _eggsPerLevel[_levelIDIndex] > collectedEggsPerLevel[_levelIDIndex]) {
        collectedEggsPerLevel[_levelIDIndex] = _eggsPerLevel[_levelIDIndex];
    }
    char i = 0;
    _globalGems = 0;
    _globalDragons = 0;
    _globalEggs = 0;
    while (i <= 35) {
        _globalGems += collectedGemsPerLevel[i];
        _globalDragons += collectedDragonsPerLevel[i];
        if (_gameState == GAMESTATE_LOADING || _gameState == GAMESTATE_INVENTORY) {
            _gemsPerLevel[i] = collectedGemsPerLevel[i];
            _dragonsPerLevel[i] = collectedDragonsPerLevel[i];
        }
        if (i < 18) {
            _globalEggs += collectedEggsPerLevel[i];
            if (_gameState == GAMESTATE_LOADING || _gameState == GAMESTATE_INVENTORY) {
                _eggsPerLevel[i] = collectedEggsPerLevel[i];
            }
        }
        i++;
    }
    if (_globalDragons != _dragonsHUD && !spinDragonCounter) {
        DetermineDragonDigits();
    }
    if (_HUDElements.dragonHUDStatus == 2 && spinDragonCounter && spinVar > 0) {
        spinVar -= 16;
        HUDSpin(startingDigit, totalDigits, _HUDElements.dragonHUDKeyframe + spinVar);
        if (spinVar == 192) {
            _dragonsHUD++;
            UpdateHUDInfo(5, 2, _dragonsHUD, 1);
        }
        if (spinVar == 0 && _globalDragons != _dragonsHUD) {
            spinVar = 256;
            DetermineDragonDigits();
        }
    } else if (_HUDElements.dragonHUDStatus != 1 && spinDragonCounter) {
        StopSpinningDragonCounter();
    }
    if (newDragon && _HUDElements.dragonHUDStatus == 2) {
        _dragonHUDTimer = 0;
        if (!spinDragonCounter) {
            StopSpinningDragonCounter();
        }
    }
    if ((_HUDElements.eggHUDStatus == 2 || _gameState != GAMESTATE_GAMEPLAY) && customEggsHUD < _globalEggs) {
        customEggsHUD++;
    }
    realGlobalEggs = _globalEggs;
}

void DetermineDragonDigits() {
    totalDigits = 1;
    startingDigit = 5;
    if (_dragonsHUD + 1 >= 10) {
        startingDigit = 6;
        if ((_dragonsHUD + 1) % 10 == 0) {
            startingDigit = 5;
            totalDigits = 2;
        }
    }
    spinDragonCounter = true;
}

void StopSpinningDragonCounter() {
    spinDragonCounter = false;
    spinVar = 256;
    newDragon = false;
}

void HideEggs() {
    if (_globalEggs > 1) {
        _globalEggs = 1;
        _eggsHUD = 1;
    }
}

void DrawEggCount() {
    char eggCountASCII[3];
    short eggCountYPosition;
    if (_HUDElements.eggHUDStatus && _HUDElements.eggCounter && _altLevelID % 10 != 5 && customEggsHUD > 0) {
        if (_HUDElements.eggHUDStatus == 1) {
            eggCountYPosition = 0x105 - (_HUDElements.eggHUDKeyframe * 4);
            char i = 4;
            while (eggCountYPosition < 0xdf && i <= 8) {
                eggCountYPosition += i;
                i += 4;
            }
        } else if (_HUDElements.eggHUDStatus == 2) {
            eggCountYPosition = 0xdf;
        } else if (_HUDElements.eggHUDStatus == 3) {
            eggCountYPosition = 0x101 - (_HUDElements.eggHUDKeyframe * 3);
        }
        if (eggCountYPosition < 0xdf) {
            eggCountYPosition = 0xdf;
        }
        sprintf(&eggCountASCII[0], "%d", customEggsHUD);
        DrawTextAll(
            &eggCountASCII,
            &(CapitalTextInfo){.x = 0x4a, .y = eggCountYPosition, .size = 0x1300},
            &(LowercaseTextInfo){.spacing = DEFAULT_SPACING - 1, .yOffset = 0, .size = 0x1300},
            DEFAULT_SPACING - 1,
            MOBY_COLOR_GOLD
        );
    }
}

void AddVisualFlightGems() {
    // 53 = 1
    // 54 = 2
    // 55 = 5
    // 56 = 10
    // 57 = 25
    char i = 0;
    while (i < 8) {
        _loadScreenVisualGems[i] = 0x57565655;
        _loadScreenVisualGems[i + 1] = 0x55545453;
        i += 2;
    }
    _loadScreenVisualGemsAmount += 8;
}

bool LevelComplete() {
    return (
        (
            (
                (_altLevelID % 10 != 0 && _altLevelID % 10 != 5) && // regular levels
                (_altLevelID != GNASTY_GNORC_ID || _gnastyGnorc == 9) && // gnasty gnorc if gnasty gnorc is dead
                (_altLevelID != GNASTYS_LOOT_ID || customLootTimer == 0) // gnasty's loot once through the loot vortex
            ) &&
            (
                _gemsPerLevel[_levelIDIndex] >= _maxGemsPerLevel[_levelIDIndex] && // all gems or more
                _dragonsPerLevel[_levelIDIndex] >= _maxDragonsPerLevel[_levelIDIndex] && // all dragons or more
                (_altLevelID >= 40 || _eggsPerLevel[_levelIDIndex] >= _maxEggsPerLevel[_levelIDIndex]) // all eggs or more pre beast makers
            )
        ) || ( // flight completion
            _altLevelID % 10 == 5 && _flightCollectedItems[0] == 8 && _flightCollectedItems[1] == 8 && _flightCollectedItems[2] == 8 && _flightCollectedItems[3] == 8
        )
    );
}

void StopDrawWorldAndObjects(void) {
    // Function addresses

    _drawWorldFunc = JR_RA;
    *(int*)((char*)&_drawWorldFunc + 4) = NOP;

    _drawObjectsFunc = JR_RA;
    *(int*)((char*)&_drawObjectsFunc + 4) = NOP;

    _drawParticlesFunc = JR_RA;
    *(int*)((char*)&_drawParticlesFunc + 4) = NOP;
}

void RestartDrawWorldAndObjects(void) {
    // Function addresses

    // Original Opcodes
    _drawWorldFunc = 0x27BDFFE8;
    *(int*)((char*)&_drawWorldFunc + 4) = 0x3C048007;

    _drawObjectsFunc = 0x27BDFFE8;
    *(int*)((char*)&_drawObjectsFunc + 4) = 0xAFBF0010;

    _drawParticlesFunc = 0x3C011F80;
    *(int*)((char*)&_drawParticlesFunc + 4) = 0xAC300000;
}
