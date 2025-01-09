#include <common.h>
#include <custom_text.h>

void MobyRender()
{
	if (CheckRegion () == NTSC) {
    	memset((void*)0x8006fcf4, 0, 0x900);
    } else {
    	memset((void*)0x80076298, 0, 0x900);
    }
	_ptrTextUnk = 0;
	CopyHudToShaded();
	DrawShadedMobys();
}

char* stringList[32] = {
	"PRESS START", // 0
	"MOD OPTIONS", // 1
	"START NEW GAME", // 2
	"SUPERCHARGE SPEED", // 3
	"INFINITE FLIGHT", // 4
	"PERMANENT SUPERFLAME", // 5
	"SPARXLESS", // 6
	"RESTORE DEFAULTS", //7
	"DONE", //8
	"SPEEDWAY MOD", //9
	"0.3.1 BETA", // 10
	"TIME ATTACK", // 11
	"120% GAME COMPLETE", // 12
	"CONTINUE TO CUTSCENE", // 13
	"EXIT LEVEL", // 14
	"120 GAME COMPLETE", // 15
	"LEVEL COMPLETE", // 16
	"BEAT THE GAME", // 17
	"RETRY", // 18
	"QUIT", // 19
	"CONTINUE", // 20
	"YOUR TIME", // 21
	"BEST TIME", // 22
	"CRASHED", // 23
	"TRAVEL TO", // 24
	"?", // 25
	"TRAVEL", // 26
	"STAY", // 27
	"LEVEL WARPING", // 28
	"UNLOCKED", // 29
	"NEW BEST TIME", // 30
	"TIME IS UP" // 31
};