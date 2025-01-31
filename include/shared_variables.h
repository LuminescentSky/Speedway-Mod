#ifndef SHARED_VARIABLES_H
#define SHARED_VARIABLES_H

enum ModState {
    GAME_STARTED,
    LOADED_TITLE_SCREEN,
    LEFT_TITLE_SCREEN,
    SKIPPED_INTRO
};
typedef enum ModState ModState;
extern ModState mod_state;

enum CustomMenuState {
    NONE,
    RETURN_TO_TITLE,
    TITLE,
    TITLE_MENU,
    TITLE_MOD_OPTIONS
};
typedef enum CustomMenuState CustomMenuState;
extern CustomMenuState customMenuState;

enum GuidebookDialogState {
    INVENTORY,
    UNLOCKED,
    CONFIRM_WARP,
    IGT_DISPLAY
};
typedef enum GuidebookDialogState GuidebookDialogState;
extern GuidebookDialogState guidebookDialogState;

extern char modOptions[4];
extern char modDefaultOptions[4];
extern char* stringList[21];
extern int customMenuSelection;
extern int customMenuSelectionColorFrames;
extern char customMenuColorArray[8];
extern int bestTimes[35];
extern int levelGameTime;
extern int totalGameTime;
extern int customLootTimer;

extern int UpdateSpyroAnim;
extern int UpdateSpyroAnim_2;
extern char endingType;
extern Vec3 rememberSpyroPos;

extern char timeTrialFailedReason;
extern char soundEffectDuration;

extern char gameTimeSeen;

extern char sharpTurnOffset;
extern char sharpTurnInfo[16];
extern unsigned char sharpTurnOffsetXPosition;

extern char inventoryIndex;


extern short collectedGemsPerLevel[35];
extern char collectedDragonsPerLevel[35];
extern char collectedEggsPerLevel[18];

extern short currentGems;
extern char currentDragons;
extern char currentEggs;

extern bool warpingUnlocked;
extern char guidebookDialogSelection;
extern char realLevelIDIndex;
extern char previousInventoryPage;

extern bool warping;
extern int warpLevelID;
extern bool isSupercharging;

extern char gnastyTimer;
extern char IGTpage;
extern int startLevel;
extern char newBestTime;

extern char copiedIGT;
extern bool seenFlightCollectables;
extern char currentFlightCollectableFlags[5];

extern bool selectedQuitGame;

extern int completionFlags[2];

extern LowercaseTextInfo defaultLowerCase;

extern bool spinDragonCounter;

extern char realGlobalEggs;
extern char customEggsHUD;

extern int timerArray[7];
extern char asciiTime[11];
extern char alignmentOffset;

extern int hideGameTime;
extern char* gameTimeStrings[3];
extern char calibrationInfoTimer;
extern bool newDragon;
extern char startingDigit;
extern char totalDigits;

extern char xButtonFlag;
extern char xButtonDuration;
extern bool retrying;

bool LevelComplete(void);

// Region specific Variables:
/*extern char exitLevelInstructionsSize;
extern int originalInventoryInstructions;
extern int skipInventoryInstructions;*/

#endif /* SHARED_VARIABLES_H */