#include <common.h>
#include <main_updates.h>
#include <multitap.h>
#include <shared_funcs.h>
#include <bg_colors.h>
#include <custom_text.h>
#include <sound.h>

void ignoreFairy() {
    // This function doesn't even need to be called in code but will be called simply when walking up to a fairy due to it being
    // the first function inside the code cave.
    _gameState = GAMESTATE_GAMEPLAY;
}

//*
//* ~ MAIN EVERY FRAME HOOK (GAMESTATE CHECK) ~
//*
void MainFunc() {
    
    //! Other functions to run every frame
    MiscUpdate();
    CustomTitleScreen();
    SuperchargeUpdate();
    GameTime();
}
