#include <common.h>
#include <custom_text.h>

void MobyRender()
{
	//if (CheckRegion () == NTSC) {
    //	memset((void*)0x8006fcf4, 0, 0x900);
    //} else {
    	memset((void*)0x80076298, 0, 0x900);
    //}
	_ptrTextUnk = 0;
	CopyHudToShaded();
	DrawShadedMobys();
}

char* stringList[21] = {
	"MOD OPTIONS", // 0
	"START NEW GAME", // 1
	"SUPERCHARGE SPEED", // 2
	"SHARP TURNING", // 3
	"PERMANENT SUPERFLAME", // 4
	"SPARXLESS", // 5
	"RESTORE DEFAULTS", //6
	"SPEEDWAY MOD", //7
	"1.0.3", // 8
	"120% GAME COMPLETE", // 9
	"CONTINUE TO CUTSCENE", // 10
	"120 GAME COMPLETE", // 11
	"LEVEL COMPLETE", // 12
	"BEAT THE GAME", // 13
	"TRAVEL TO", // 14
	"?", // 15
	"TRAVEL", // 16
	"STAY", // 17
	"LEVEL WARPING", // 18
	"UNLOCKED", // 19
	"NEW BEST TIME", // 20
	//"TIME ATTACK", // 21
	//"CRASHED", // 22
	//"YOUR TIME", // 23
	//"BEST TIME" // 24

};