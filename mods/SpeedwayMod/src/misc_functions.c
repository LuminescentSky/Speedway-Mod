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

GuidebookDialogState guidebookDialogState = INVENTORY;

void UndoInventoryChanges() {
    _gemsPerLevel[_levelIDIndex] = currentGems;
    _dragonsPerLevel[_levelIDIndex] = currentDragons;
    _eggsPerLevel[_levelIDIndex] = currentEggs;
}

void BeforeDeath() {
    if (_globalLives == 0) {
        timeTrialFailedReason = 4;
        _spyro.health = 3;
        LoadIGTMenu(true);
        if (_altLevelID % 10 == 5) {
            _flightMenuState = 0;
            _gameState = GAMESTATE_FLIGHT_LEVEL_MENU;
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
    _artisansLiveFlag |= 0x40000000;
    _peaceKeepersLiveFlag |= 0x04000000;
    _magicCraftersLiveFlag |= 0x00400000;
    _beastMakersLiveFlag |= 0x01000000;
    _dreamWeaversLiveFlag |= 0x00000880;
    _keyState = 0;
    _ptr_keyChestHostGem = NULL; // Key Glitch Fix
    warpingUnlocked = false;
    theEgg = 0x4F59414A;
    memcpy(&_ptr_exitLevel, &_ptr_quit, 36);
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

PrepareWarping() {
    warpLevelID = inventoryIndex / 6 * 10 + 10 + inventoryIndex % 6;
    _altLevelID = 0;
    ExitLevel(0);
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
    if (_altLevelID % 10 == 5 && _gameState != GAMESTATE_FLIGHT_LEVEL_MENU) {
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

void NoDragonAnimation() {
    Moby* mobyIteration = _ptrLevelMobys;
        char dragonLoop = 0;
        do {
            if (mobyIteration->type == 250) {
                *(int*)((char*)mobyIteration->ptr_mobyData + 0x18) = -1;
                dragonLoop++;
            }
            mobyIteration++;
        } while (dragonLoop < _maxDragonsPerLevel[realLevelIDIndex] && _altLevelID != GNASTYS_WORLD_ID);
}
