MODDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
TARGET = mod

SRCS = asm/hook.s asm/inventoryToPauseHook.s asm/inventoryToGameplayHook.s asm/inventoryToCreditsHook.s asm/openInventoryHook.s asm/cheat.s asm/beforeInventoryUpdateHook.s asm/beforePauseMenuUpdateHook.s asm/beforeHUDUpdateHook.s asm/HUDUpdateReturnHook.s asm/quitGame.s asm/menuTextHook.s asm/beforeMenuTextHook.s asm/beforeDeathHook.s asm/dontDrawText1_pal.s asm/dontDrawText2_pal.s asm/disableChargeSound_pal.s asm/overwriteRawInputs.s asm/exitLevelHook.s asm/stopSoundHook.s asm/stopSoundHook2.s asm/fairyFix.s src/custom_title_screen.c src/menu_hooks.c src/main.c src/supercharge.c src/misc_update.c src/custom_text.c src/gametime.c src/misc_functions.c 
CPPFLAGS = -DBUILD=94229
LDSYMS = -T$(MODDIR)../../symbols/pal.txt 
USE_FUNCTION_SECTIONS ?= true
DISABLE_FUNCTION_REORDER ?= false
USE_PSYQ ?= false
OVERLAYSECTION ?= .asmhooks .asminventoryToPauseHooks .asminventoryToGameplayHooks .asminventoryToCreditsHooks .asmopenInventoryHooks .asmcheats .asmbeforeInventoryUpdateHooks .asmbeforePauseMenuUpdateHooks .asmbeforeHUDUpdateHooks .asmHUDUpdateReturnHooks .asmquitGames .asmmenuTextHooks .asmbeforeMenuTextHooks .asmbeforeDeathHooks .asmdontDrawText1_pals .asmdontDrawText2_pals .asmdisableChargeSound_pals .asmoverwriteRawInputss .asmexitLevelHooks .asmstopSoundHooks .asmstopSoundHook2s .asmfairyFixs .Header .DragonAndFairyUpdate .DrawDragonAndFairyText .MemoryCardArea 
OVR_START_ADDR = 0x8000b0b8
OVERLAYSCRIPT = overlay.ld
BUILDDIR = $(MODDIR)output/
SRCINCLUDEDIR = $(MODDIR)src/
GAMEINCLUDEDIR = $(MODDIR)../../include/
EXTRA_CC_FLAGS = -Os -g

include ../../../common.mk
