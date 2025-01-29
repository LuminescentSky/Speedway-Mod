#include <common.h>
#include <multitap.h>
#include <spyro.h>
#include <sound.h>
#include <custom_text.h>
#include <shared_variables.h>
#include <moby.h>

#define OPCODE1 0x3C028008;

#define OPCODE2_NTSC 0x8C428AB4;
#define OPCODE2_PAL 0x8C42F444;    

bool isSupercharging = false;
bool newTriangleDrop = false;
char sharpTurnFlag = 0;
char chargeTransitionFlag = 0;
char xButtonFlag = 0;
char xButtonDuration = 0;
char xButtonDurationInterval = 0;
bool lootFirstHeightcap = false;
int defaultOffset = 3000;
int superchargeHeightStartedOffset = 0;
int chargeTransitionMomentumStart = 0;
int chargeTransitionMomentum = 0;
int chargeTransitionTime = 0;
int previousSpyroState = 0;
int levelHeightCaps[40] = {
    13100,
    28100,
    30600,
    19600,
    14100,
    0,

    63300,
    13300,
    13300,
    26600,
    16200,
    0,

    25500,
    17000,
    45300,
    25400,
    15100,
    0,

    27000,
    23200,
    15400,
    15000,
    15000,
    0,

    24400,
    56600,
    40500,
    39400,
    41000,
    0,

    17700,
    28600,
    17500,
    18000,

    // LOOT
    15900,
    18000,
    20000,
    22100,
    24100,
    26200
};

// 22784 is max pal absolute momentum when supercharging, default offset 18300 achieves that
void setDefaultOffset(void) {
    defaultOffset = 3000;
    int i = 1;
    while (i < modOptions[0]) {
        defaultOffset += 5100;
        i++;
    }
}

bool IsInAnySuperchargeSubState(void) {
    // Added substate supercharge to hopefully avoid rare crashes?
    return (_spyro.state == SUPERCHARGE || _spyro.state == CHARGING_AIR_SC_JUMP || _spyro.state == CHARGING_AIR || _spyro.subState == SUBSTATE_SUPERCHARGE);
}

bool XButtonCap(void) {
    return (_spyro.airMomentum.z > -6144 && _spyro.airMomentum.z < 6144 && _spyro.position.z < _heightCap - 128);
}

//bool IsInSuperchargeLevel(void) {
    // Magic Crafters, High Caves, Wizard Peak, Tree Tops, Lofty Castle, Haunted Towers, Twilight Harbour
//    return (_altLevelID == 30 || _altLevelID == 32 || _altLevelID == 33 || _altLevelID == 43 || _altLevelID == 52 || _altLevelID == 53 || _altLevelID == 62);
//}

void SuperchargeUpdate(void) {
    // Enable Superfly everywhere but disable when not in gameplay due to possible crashes, also wait out fly out and fly in animations
    if (_gameState == GAMESTATE_GAMEPLAY && !_spyro.isMovementLocked) {
        _spyro.canSuperfly = 1;
    } else if (_gameState == GAMESTATE_LOADING) {
        _spyro.canSuperfly = 0;
    }

    if (_gameState == GAMESTATE_DEATH) {
        lootFirstHeightcap = false;
    }
    if (_gameState == GAMESTATE_GAMEPLAY) {

        // level IGT debugging by giving myself gems and dragons
        if (_currentButtonOneFrame & CIRCLE_BUTTON) {
            //_gemsPerLevel[realLevelIDIndex]++;
            //_dragonsPerLevel[realLevelIDIndex]++;
            //_eggsPerLevel[realLevelIDIndex]++;
        }

        // moved calibration here because of movement lock
        if (_currentButton & R3_BUTTON) {
            if (_spyro.isMovementLocked == 0) {
                _spyro.isMovementLocked = 1;
            }
            if (_currentButtonOneFrame & UP_BUTTON) {
                sharpTurnOffset++;
            } else if (_currentButtonOneFrame & DOWN_BUTTON) {
                sharpTurnOffset--;
            }
            calibrationInfoTimer = 50;
            //if (CheckRegion() == PAL || sharpTurnOffset >= 0) {
            sprintf(&sharpTurnInfo, "OFFSET %d", sharpTurnOffset);
            //} else {
            //    sprintf(&sharpTurnInfo, "OFFSET NEG. %d", (sharpTurnOffset * -1));
            //}
            if (sharpTurnOffset <= -100) {
                sharpTurnOffsetXPosition = 0xa9;
            } else if (sharpTurnOffset <= -10 || sharpTurnOffset >= 100) {
                sharpTurnOffsetXPosition = 0xb2;
            } else if (sharpTurnOffset <= -1 || sharpTurnOffset >= 10) {
                sharpTurnOffsetXPosition = 0xbb;
            } else {
                sharpTurnOffsetXPosition = 0xc4;
            }
        }

        if (modOptions[2]) {
            _superflameTimer = 0x200;
        }

        if (modOptions[3] && _spyro.health > 0) {
            _spyro.health = 0;
        }

        // Always allow to reglide (fly) despite having glided and then supercharged
        _spyro.glideUsed = 0;

        // Flight levels handle the inability to charge out of flying in a weird way
        // Many annoyances are just simply fixed by using the glide state to replace superfly
        if (_spyro.state == SUPERFLY || _spyro.state == SUPERFLY_ASCENDING || _spyro.state == SUPERFLY_DESCENDING) {
            _spyro.state = GLIDE;
        }
        if (_spyro.canSuperfly == 1) {
            //if (CheckRegion() == NTSC) {
            //    xButtonDurationInterval = 6;
            //} else {
                xButtonDurationInterval = 7;
            //}

            /*if (!xButtonFlag && xButtonDuration < 42 && (_currentButton & X_BUTTON) && _spyro.state == GLIDE && _spyro.airMomentum.z > -6144 && _spyro.airMomentum.z < 6144 && _spyro.position.z < _heightCap - 128) {
                xButtonDuration += xButtonDurationInterval;
            } else if (xButtonDuration > 0) {
                xButtonFlag = true;
                xButtonDuration -= xButtonDurationInterval;
            } else if (!(_currentButton & X_BUTTON) && _spyro.state != CHARGING_AIR) {
                xButtonFlag = false;
            }*/

            if (!xButtonFlag && (_currentButtonOneFrame & X_BUTTON) && _spyro.state == GLIDE && XButtonCap() && _spyro.framesInCurrentState > 1 && !_spyro.isMovementLocked) {
                xButtonFlag = 1;
            }
            if (xButtonFlag == 1) {
                if (xButtonDuration >= 42 || _spyro.state != GLIDE || (!XButtonCap() && xButtonDuration >= 21)) {
                    xButtonFlag = 2;
                } else {
                    xButtonDuration += xButtonDurationInterval;
                }
            }
            if (xButtonFlag == 2 && xButtonDuration > 0) {
                xButtonDuration -= xButtonDurationInterval;
            }
            if (xButtonDuration == 0) {
                xButtonFlag = 0;
            }

            // Loot first heightcap
            if (_spyro.isGrounded && _spyro.position.z > levelHeightCaps[34]) {
                lootFirstHeightcap = true;
            }

            // Normalise flight height cap and speed to always be the same (except loot)
            if (_spyro.state == GLIDE) {
                if (_levelID != GNASTYS_LOOT_ID && !_isInFlightLevel) {
                    _heightCap = levelHeightCaps[_levelIDIndex];
                }
                if (_levelID == GNASTYS_LOOT_ID) {
                    if (_lootKey4 == 3) {
                        _heightCap = levelHeightCaps[39];
                    } else if (_lootKey3 == 3) {
                        _heightCap = levelHeightCaps[38];
                    } else if (_lootKey2 == 3) {
                        _heightCap = levelHeightCaps[37];
                    } else if (_lootKey1 == 3) {
                        _heightCap = levelHeightCaps[36];
                    } else if (lootFirstHeightcap) {
                        _heightCap = levelHeightCaps[35];
                    } else {
                        _heightCap = levelHeightCaps[34];
                    }
                }
                _variableHeightCap = _heightCap;
                _spyro.flightMomentum.z += xButtonDuration * 4;
                if (_spyro.airMomentum.z > 0) {
                    chargeTransitionMomentumStart = _spyro.airMomentum.z / 2;
                } else {
                    chargeTransitionMomentumStart = _spyro.airMomentum.z;
                }
                chargeTransitionFlag = 1;
                chargeTransitionTime = 0;
            } else if (_spyro.state != CHARGING_AIR) {
                chargeTransitionFlag = 0;
            }
        }

        // Stuff that checks for charging in air and charging state to make Spyro supercharge instead
        // When charging, use some janky level dissociation workaround to make it work
        if (_spyro.state == CHARGING_AIR) {
            if (_currentAnimationState == 3 && !isSupercharging) {
                // Cancel charging in air when roll bouncing to prevent crashing
                _spyro.state = previousSpyroState;
            } else {
                // Sometimes Supercharge is lost when supercharge jumping, so always give supercharge when charging in air
                _spyro.subState = SUBSTATE_SUPERCHARGE;
                if (!isSupercharging) {
                    // Only set to 3000 when supercharge was just initiated
                    superchargeHeightStartedOffset = defaultOffset;
                }
                if (chargeTransitionFlag > 0) {
                    if (xButtonDuration > 0 && (_currentButton & X_BUTTON) && !_spyro.isMovementLocked) {
                        chargeTransitionMomentumStart += xButtonDuration * 64;
                    }

                    // Funny quadratic function that I spent way too many hours on (superfly jumping):
                    chargeTransitionMomentum = (-(chargeTransitionTime*chargeTransitionTime)) * 20 + chargeTransitionMomentumStart;
                    
                    _spyro.airMomentum.z = chargeTransitionMomentum;
                
                    chargeTransitionTime++;
                }
                isSupercharging = true;
            }
        } else if (_spyro.state == CHARGE) {
            // Prevent supercharging when still in a roll or roll bounce animation to prevent crashing
            if (_currentAnimationState != 3 && (_currentButton & SQUARE_BUTTON) && !_spyro.isMovementLocked) {
                _spyro.state = SUPERCHARGE;
                superchargeHeightStartedOffset = defaultOffset;
                isSupercharging = true;
            } else {
                _spyro.state = WALKING;
            }
        } else if (!IsInAnySuperchargeSubState()) {
            isSupercharging = false;
        }

        // Allow instant triangle dropping upon gliding (slide from X to Triangle), also allow this in flights
        if (_currentButtonOneFrame & TRIANGLE_BUTTON && _spyro.state == GLIDE && _spyro.isMovementLocked == 0) {
            _spyro.state = FLOP;
            newTriangleDrop = true;
        } else if (newTriangleDrop && _spyro.state == FLOP) {
            _spyro.airMomentum.x = 0;
            _spyro.airMomentum.y = 0;
        } else if (newTriangleDrop) {
            newTriangleDrop = false;
        }

        if (_spyro.state == TRIANGLE_DROP) {
            _spyro.state = FLOP;
        }

        // DEBUG (in-game "ram watch")
        /*sprintf(&sharpTurnInfo, "%d %d %d", 0, xButtonDuration, xButtonFlag);
        DrawTextAll(
            sharpTurnInfo,
            &(CapitalTextInfo){.x = 0x10, .y = SCREEN_BOTTOM_EDGE - 0x20, .size = DEFAULT_SIZE},
            &(LowercaseTextInfo){.spacing = DEFAULT_SPACING, .yOffset = 0, .size = DEFAULT_SIZE},
            DEFAULT_SPACING,
            MOBY_COLOR_GOLD
        );*/
        previousSpyroState = _spyro.state;

        // Flight level fixes
        if (_altLevelID == 15) {
            _sunnyFlightSubStateCheck1 = 0x284200c1;
            _sunnyFlightSubStateCheck2 = 0x284200c1;
            _sunnyFlightSubStateCheck3 = 0x284200c1;
        } else if (_altLevelID == 25) {
            _nightFlightSubStateCheck1 = 0x284200c1;
        } else if (_altLevelID == 35) {
            _crystalFlightSubStateCheck1 = 0x284200c1;
        } else if (_altLevelID == 45) {
            _wildFlightSubStateCheck1 = 0x284200c1;
            _wildFlightSubStateCheck2 = 0x284200c1;
        } else if (_altLevelID == 55) {
            _icyFlightSubStateCheck1 = 0x284200c1;
            _icyFlightSubStateCheck2 = 0x284200c1;
            _icyFlightSubStateCheck3 = 0x284200c1;
        }
    }
    if (isSupercharging) {


        // Make UpdateSpyroAnim return immediately
        // Attempted to load supercharge animations in supercharge levels however flaming and supercharging at the same time
        // can crash the game (too many particles?) so not doing that for now
//            if (!IsInSuperchargeLevel()) {
            UpdateSpyroAnim = JR_RA;
            UpdateSpyroAnim_2 = NOP;
//            }

        // Update Speed
/*          if (_currentButtonOneFrame & R1_BUTTON)
        {
            superchargeHeightStartedOffset += 2000;
        }
        else if (_currentButtonOneFrame & L1_BUTTON)
        {
            superchargeHeightStartedOffset -= 2000;
        }*/ 
        /*if (_gameState == GAMESTATE_GAMEPLAY && _altLevelID % 10 == 5 && !(completionFlags[realLevelIDIndex / 32] & (1 << (realLevelIDIndex % 32)))) {
            if (_spyro.subState == SUBSTATE_SUPERCHARGE) {
                // timer freezes when supercharging, so advancing it forward when that is the case
                _flightExpirationVisualTimer -= 2;
                _flightLevelExpirationTimer -= 2;
            }
        }*/

        // Use a constant speed to forcibly normalise differences between supercharging being started from charging in air and charging
        // This also means supercharge speed will always be the same regardless of altitude unless L1 or R1 are used
        _somethingSuperchargeRelated = 0x2000;
        _spyro.superchargeHeightStarted = _spyro.position.z + superchargeHeightStartedOffset;
    } else if (!isSupercharging) {
        // Set original opcodes for UpdateSpyroAnim
        UpdateSpyroAnim = OPCODE1;
        /*if(CheckRegion() == NTSC)
        {
            UpdateSpyroAnim_2 = OPCODE2_NTSC;
        }
        else
        {*/
            UpdateSpyroAnim_2 = OPCODE2_PAL
        //}

    }
}

void SharpTurning(void) {
    if (_gameState == GAMESTATE_GAMEPLAY && modOptions[1]) {
        // Credits to OddKara for helping me with sharp turning
        if ((!((~_rawButtons & RAW_RIGHT) && (~_rawButtons & RAW_DOWN) || (~_rawButtons & RAW_LEFT) && (~_rawButtons & RAW_DOWN)) && _rawLeftAnalogY < 0xc0) || _spyro.state != SUPERCHARGE || _spyro.isMovementLocked) {
            sharpTurnFlag = 0;
        }
        if (_spyro.state == SUPERCHARGE && !_spyro.isMovementLocked) {
            // Maybe forcing him to be grounded even if he should be already fixes the rare occasions where sharp turning doesn't work?
            if ((~_rawButtons & RAW_RIGHT) && (~_rawButtons & RAW_DOWN) || (_rawLeftAnalogY >= 0xc0 && _rawLeftAnalogX > 0x80)) {
                if (sharpTurnFlag != 1) {
                    _rawButtons = (_rawButtons | RAW_RIGHT | RAW_DOWN);
                    _rawLeftAnalogX = 0x3e + sharpTurnOffset;
                    sharpTurnFlag = 1;
                } else {
                    _rawButtons = (_rawButtons | RAW_RIGHT | RAW_DOWN) & ~RAW_LEFT;
                }
            } else if ((~_rawButtons & RAW_LEFT) && (~_rawButtons & RAW_DOWN) || (_rawLeftAnalogY >= 0xc0 && _rawLeftAnalogX < 0x80)) {
                if (sharpTurnFlag != 2) {
                    _rawButtons = (_rawButtons | RAW_LEFT | RAW_DOWN);
                    _rawLeftAnalogX = 0xba + sharpTurnOffset;
                    sharpTurnFlag = 2;
                } else {
                    _rawButtons = (_rawButtons | RAW_LEFT | RAW_DOWN) & ~RAW_RIGHT;
                }
            }
        }
    } else if (_gameState == GAMESTATE_FLIGHT_LEVEL_MENU) {
        _rawButtons = (_rawButtons | RAW_DOWN | RAW_UP);
    }
}