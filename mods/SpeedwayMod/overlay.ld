__heap_base = __ovr_end;

__ovr_start = 0x8000a91c;

SECTIONS {
    OVERLAY __ovr_start : SUBALIGN(4)
    {
        .asmhooks
        {
            . = . + 0x2e768;
            KEEP(asm/hook.o(.text*))
            KEEP(asm/hook.o(.rodata*))
            KEEP(asm/hook.o(.sdata*))
            KEEP(asm/hook.o(.data*))
            KEEP(asm/hook.o(.sbss*))
            KEEP(asm/hook.o(.bss*))
            KEEP(asm/hook.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asminventoryToPauseHooks
        {
            . = . + 0x29460;
            KEEP(asm/inventoryToPauseHook.o(.text*))
            KEEP(asm/inventoryToPauseHook.o(.rodata*))
            KEEP(asm/inventoryToPauseHook.o(.sdata*))
            KEEP(asm/inventoryToPauseHook.o(.data*))
            KEEP(asm/inventoryToPauseHook.o(.sbss*))
            KEEP(asm/inventoryToPauseHook.o(.bss*))
            KEEP(asm/inventoryToPauseHook.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asminventoryToGameplayHooks
        {
            . = . + 0x2948c;
            KEEP(asm/inventoryToGameplayHook.o(.text*))
            KEEP(asm/inventoryToGameplayHook.o(.rodata*))
            KEEP(asm/inventoryToGameplayHook.o(.sdata*))
            KEEP(asm/inventoryToGameplayHook.o(.data*))
            KEEP(asm/inventoryToGameplayHook.o(.sbss*))
            KEEP(asm/inventoryToGameplayHook.o(.bss*))
            KEEP(asm/inventoryToGameplayHook.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asminventoryToCreditsHooks
        {
            . = . + 0x27d24;
            KEEP(asm/inventoryToCreditsHook.o(.text*))
            KEEP(asm/inventoryToCreditsHook.o(.rodata*))
            KEEP(asm/inventoryToCreditsHook.o(.sdata*))
            KEEP(asm/inventoryToCreditsHook.o(.data*))
            KEEP(asm/inventoryToCreditsHook.o(.sbss*))
            KEEP(asm/inventoryToCreditsHook.o(.bss*))
            KEEP(asm/inventoryToCreditsHook.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmopenInventoryHooks
        {
            . = . + 0x26ed8;
            KEEP(asm/openInventoryHook.o(.text*))
            KEEP(asm/openInventoryHook.o(.rodata*))
            KEEP(asm/openInventoryHook.o(.sdata*))
            KEEP(asm/openInventoryHook.o(.data*))
            KEEP(asm/openInventoryHook.o(.sbss*))
            KEEP(asm/openInventoryHook.o(.bss*))
            KEEP(asm/openInventoryHook.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmcheats
        {
            . = . + 0x27f00;
            KEEP(asm/cheat.o(.text*))
            KEEP(asm/cheat.o(.rodata*))
            KEEP(asm/cheat.o(.sdata*))
            KEEP(asm/cheat.o(.data*))
            KEEP(asm/cheat.o(.sbss*))
            KEEP(asm/cheat.o(.bss*))
            KEEP(asm/cheat.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmbeforeInventoryUpdateHooks
        {
            . = . + 0x2e200;
            KEEP(asm/beforeInventoryUpdateHook.o(.text*))
            KEEP(asm/beforeInventoryUpdateHook.o(.rodata*))
            KEEP(asm/beforeInventoryUpdateHook.o(.sdata*))
            KEEP(asm/beforeInventoryUpdateHook.o(.data*))
            KEEP(asm/beforeInventoryUpdateHook.o(.sbss*))
            KEEP(asm/beforeInventoryUpdateHook.o(.bss*))
            KEEP(asm/beforeInventoryUpdateHook.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmbeforePauseMenuUpdateHooks
        {
            . = . + 0x2e1e8;
            KEEP(asm/beforePauseMenuUpdateHook.o(.text*))
            KEEP(asm/beforePauseMenuUpdateHook.o(.rodata*))
            KEEP(asm/beforePauseMenuUpdateHook.o(.sdata*))
            KEEP(asm/beforePauseMenuUpdateHook.o(.data*))
            KEEP(asm/beforePauseMenuUpdateHook.o(.sbss*))
            KEEP(asm/beforePauseMenuUpdateHook.o(.bss*))
            KEEP(asm/beforePauseMenuUpdateHook.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmbeforeHUDUpdateHooks
        {
            . = . + 0x2e46c;
            KEEP(asm/beforeHUDUpdateHook.o(.text*))
            KEEP(asm/beforeHUDUpdateHook.o(.rodata*))
            KEEP(asm/beforeHUDUpdateHook.o(.sdata*))
            KEEP(asm/beforeHUDUpdateHook.o(.data*))
            KEEP(asm/beforeHUDUpdateHook.o(.sbss*))
            KEEP(asm/beforeHUDUpdateHook.o(.bss*))
            KEEP(asm/beforeHUDUpdateHook.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmHUDUpdateReturnHooks
        {
            . = . + 0x50514;
            KEEP(asm/HUDUpdateReturnHook.o(.text*))
            KEEP(asm/HUDUpdateReturnHook.o(.rodata*))
            KEEP(asm/HUDUpdateReturnHook.o(.sdata*))
            KEEP(asm/HUDUpdateReturnHook.o(.data*))
            KEEP(asm/HUDUpdateReturnHook.o(.sbss*))
            KEEP(asm/HUDUpdateReturnHook.o(.bss*))
            KEEP(asm/HUDUpdateReturnHook.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmquitGames
        {
            . = . + 0x2887c;
            KEEP(asm/quitGame.o(.text*))
            KEEP(asm/quitGame.o(.rodata*))
            KEEP(asm/quitGame.o(.sdata*))
            KEEP(asm/quitGame.o(.data*))
            KEEP(asm/quitGame.o(.sbss*))
            KEEP(asm/quitGame.o(.bss*))
            KEEP(asm/quitGame.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmmenuTextHooks
        {
            . = . + 0x16584;
            KEEP(asm/menuTextHook.o(.text*))
            KEEP(asm/menuTextHook.o(.rodata*))
            KEEP(asm/menuTextHook.o(.sdata*))
            KEEP(asm/menuTextHook.o(.data*))
            KEEP(asm/menuTextHook.o(.sbss*))
            KEEP(asm/menuTextHook.o(.bss*))
            KEEP(asm/menuTextHook.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmbeforeMenuTextHooks
        {
            . = . + 0x19550;
            KEEP(asm/beforeMenuTextHook.o(.text*))
            KEEP(asm/beforeMenuTextHook.o(.rodata*))
            KEEP(asm/beforeMenuTextHook.o(.sdata*))
            KEEP(asm/beforeMenuTextHook.o(.data*))
            KEEP(asm/beforeMenuTextHook.o(.sbss*))
            KEEP(asm/beforeMenuTextHook.o(.bss*))
            KEEP(asm/beforeMenuTextHook.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmbeforeDeathHooks
        {
            . = . + 0x45214;
            KEEP(asm/beforeDeathHook.o(.text*))
            KEEP(asm/beforeDeathHook.o(.rodata*))
            KEEP(asm/beforeDeathHook.o(.sdata*))
            KEEP(asm/beforeDeathHook.o(.data*))
            KEEP(asm/beforeDeathHook.o(.sbss*))
            KEEP(asm/beforeDeathHook.o(.bss*))
            KEEP(asm/beforeDeathHook.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmdontDrawText1_pals
        {
            . = . + 0x18d44;
            KEEP(asm/dontDrawText1_pal.o(.text*))
            KEEP(asm/dontDrawText1_pal.o(.rodata*))
            KEEP(asm/dontDrawText1_pal.o(.sdata*))
            KEEP(asm/dontDrawText1_pal.o(.data*))
            KEEP(asm/dontDrawText1_pal.o(.sbss*))
            KEEP(asm/dontDrawText1_pal.o(.bss*))
            KEEP(asm/dontDrawText1_pal.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmdontDrawText2_pals
        {
            . = . + 0x18db4;
            KEEP(asm/dontDrawText2_pal.o(.text*))
            KEEP(asm/dontDrawText2_pal.o(.rodata*))
            KEEP(asm/dontDrawText2_pal.o(.sdata*))
            KEEP(asm/dontDrawText2_pal.o(.data*))
            KEEP(asm/dontDrawText2_pal.o(.sbss*))
            KEEP(asm/dontDrawText2_pal.o(.bss*))
            KEEP(asm/dontDrawText2_pal.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmdisableChargeSound_pals
        {
            . = . + 0x39c00;
            KEEP(asm/disableChargeSound_pal.o(.text*))
            KEEP(asm/disableChargeSound_pal.o(.rodata*))
            KEEP(asm/disableChargeSound_pal.o(.sdata*))
            KEEP(asm/disableChargeSound_pal.o(.data*))
            KEEP(asm/disableChargeSound_pal.o(.sbss*))
            KEEP(asm/disableChargeSound_pal.o(.bss*))
            KEEP(asm/disableChargeSound_pal.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmoverwriteRawInputss
        {
            . = . + 0x63cfc;
            KEEP(asm/overwriteRawInputs.o(.text*))
            KEEP(asm/overwriteRawInputs.o(.rodata*))
            KEEP(asm/overwriteRawInputs.o(.sdata*))
            KEEP(asm/overwriteRawInputs.o(.data*))
            KEEP(asm/overwriteRawInputs.o(.sbss*))
            KEEP(asm/overwriteRawInputs.o(.bss*))
            KEEP(asm/overwriteRawInputs.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmexitLevelHooks
        {
            . = . + 0x291a0;
            KEEP(asm/exitLevelHook.o(.text*))
            KEEP(asm/exitLevelHook.o(.rodata*))
            KEEP(asm/exitLevelHook.o(.sdata*))
            KEEP(asm/exitLevelHook.o(.data*))
            KEEP(asm/exitLevelHook.o(.sbss*))
            KEEP(asm/exitLevelHook.o(.bss*))
            KEEP(asm/exitLevelHook.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmstopSoundHooks
        {
            . = . + 0x26d44;
            KEEP(asm/stopSoundHook.o(.text*))
            KEEP(asm/stopSoundHook.o(.rodata*))
            KEEP(asm/stopSoundHook.o(.sdata*))
            KEEP(asm/stopSoundHook.o(.data*))
            KEEP(asm/stopSoundHook.o(.sbss*))
            KEEP(asm/stopSoundHook.o(.bss*))
            KEEP(asm/stopSoundHook.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmstopSoundHook2s
        {
            . = . + 0x2da88;
            KEEP(asm/stopSoundHook2.o(.text*))
            KEEP(asm/stopSoundHook2.o(.rodata*))
            KEEP(asm/stopSoundHook2.o(.sdata*))
            KEEP(asm/stopSoundHook2.o(.data*))
            KEEP(asm/stopSoundHook2.o(.sbss*))
            KEEP(asm/stopSoundHook2.o(.bss*))
            KEEP(asm/stopSoundHook2.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .asmfairyFixs
        {
            . = . + 0x27420;
            KEEP(asm/fairyFix.o(.text*))
            KEEP(asm/fairyFix.o(.rodata*))
            KEEP(asm/fairyFix.o(.sdata*))
            KEEP(asm/fairyFix.o(.data*))
            KEEP(asm/fairyFix.o(.sbss*))
            KEEP(asm/fairyFix.o(.bss*))
            KEEP(asm/fairyFix.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .TitleScreen
        {
            KEEP(src/custom_title_screen.o(.text*))
            KEEP(src/custom_title_screen.o(.rodata*))
            KEEP(src/custom_title_screen.o(.sdata*))
            KEEP(src/custom_title_screen.o(.data*))
            KEEP(src/custom_title_screen.o(.sbss*))
            KEEP(src/custom_title_screen.o(.bss*))
            KEEP(src/custom_title_screen.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .DragonAndFairyUpdate
        {
            . = . + 0x29a98;
            KEEP(src/menu_hooks.o(.text*))
            KEEP(src/main.o(.text*))
            KEEP(src/supercharge.o(.text*))
            KEEP(src/menu_hooks.o(.rodata*))
            KEEP(src/main.o(.rodata*))
            KEEP(src/supercharge.o(.rodata*))
            KEEP(src/menu_hooks.o(.sdata*))
            KEEP(src/main.o(.sdata*))
            KEEP(src/supercharge.o(.sdata*))
            KEEP(src/menu_hooks.o(.data*))
            KEEP(src/main.o(.data*))
            KEEP(src/supercharge.o(.data*))
            KEEP(src/menu_hooks.o(.sbss*))
            KEEP(src/main.o(.sbss*))
            KEEP(src/supercharge.o(.sbss*))
            KEEP(src/menu_hooks.o(.bss*))
            KEEP(src/main.o(.bss*))
            KEEP(src/supercharge.o(.bss*))
            KEEP(src/menu_hooks.o(.ctors))
            KEEP(src/main.o(.ctors))
            KEEP(src/supercharge.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .DrawDragonAndFairyText
        {
            . = . + 0x17000;
            KEEP(src/misc_update.o(.text*))
            KEEP(src/custom_text.o(.text*))
            KEEP(src/gametime.o(.text*))
            KEEP(src/misc_update.o(.rodata*))
            KEEP(src/custom_text.o(.rodata*))
            KEEP(src/gametime.o(.rodata*))
            KEEP(src/misc_update.o(.sdata*))
            KEEP(src/custom_text.o(.sdata*))
            KEEP(src/gametime.o(.sdata*))
            KEEP(src/misc_update.o(.data*))
            KEEP(src/custom_text.o(.data*))
            KEEP(src/gametime.o(.data*))
            KEEP(src/misc_update.o(.sbss*))
            KEEP(src/custom_text.o(.sbss*))
            KEEP(src/gametime.o(.sbss*))
            KEEP(src/misc_update.o(.bss*))
            KEEP(src/custom_text.o(.bss*))
            KEEP(src/gametime.o(.bss*))
            KEEP(src/misc_update.o(.ctors))
            KEEP(src/custom_text.o(.ctors))
            KEEP(src/gametime.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
        .MemoryCardArea
        {
            . = . + 0x61398;
            KEEP(src/misc_functions.o(.text*))
            KEEP(src/misc_functions.o(.rodata*))
            KEEP(src/misc_functions.o(.sdata*))
            KEEP(src/misc_functions.o(.data*))
            KEEP(src/misc_functions.o(.sbss*))
            KEEP(src/misc_functions.o(.bss*))
            KEEP(src/misc_functions.o(.ctors))
            
            . = ALIGN(4);
            __ovr_end = .;
        }
    }
}
