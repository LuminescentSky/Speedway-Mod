#include <common.h>
#include <multitap.h>
#include <spyro.h>
#include <sound.h>

int sharpTurnFlag = false;

void SuperchargeUpdatePart2(void) {
	if (_gameState == GAMESTATE_GAMEPLAY) {
        // Sharp Turning
        if (!(~_rawButtons & RIGHT_BUTTON || ~_rawButtons & LEFT_BUTTON) || _spyro.state != SUPERCHARGE) {
            sharpTurnFlag = false;
        }
        if (_spyro.state == SUPERCHARGE) {
            if (~_rawButtons & RIGHT_BUTTON) {
                if (!sharpTurnFlag) {
                    _rawButtons = (_rawButtons | RIGHT_BUTTON);
                    _rawLeftAnalogX = 0x3e;
                    sharpTurnFlag = true;
                } else {
                    _rawButtons = (_rawButtons | RIGHT_BUTTON) & ~LEFT_BUTTON;
                    _rawLeftAnalogX = 0x3e;
                }
            }
        }
	}
}