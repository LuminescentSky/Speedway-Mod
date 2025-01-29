

# Spyro 1 Speedway Mod
Speedway Mod is a Romhack for Spyro the Dragon (PAL) with the concept to turn the entire game into speedways, similar to the ones from Spyro 2 and Spyro 3. The idea is comparable to the "Level Storage" glitch in Spyro Reignited brought to the original Spyro game!

## Download
Go to this website to patch your Spyro the Dragon (PAL) copy!
- [Speedway Mod Patcher](https://luminescentsky.github.io/SpeedwayMod/)
- [Changelog](https://luminescentsky.github.io/SpeedwayMod/changelog)

## Features
 - Every time you press square to charge, you will supercharge instead!
 - Sharp turn by holding down left or down right while supercharging!*
 - Free flight has been modified to be similar to Spyro 2, where you can press X to "flap your wings" and even do superfly jumps! Try pressing X and Square while flying at the right timing, think of double jump inputs!
 - Every level is a time trial! Your record times are saved once you fully complete a level!
 - Dragons are skipped instantly, so you don't have to wait around while trying to go fast!
 - Warping between levels from the Guidebook can be unlocked by using the All Levels cheat, to make running individual levels easier!
 - Quality of life improvements like the ability to quit game out of any level to make resetting runs on console easier and quicker!

\* Sharp turning requires Analog Mode to be enabled and may have to be calibrated using R3 + Up/Down. Usually, the offset should be somewhere between -4 and 4.

## Credits

- A huge, huge thanks to [Composer](https://github.com/C0mposer/) for taking so much of their time to teach me how to get started with romhacking, as well as allowing me to use their source code for [Spyro 1 Practice ROM](https://github.com/C0mposer/Spyro-1-Practice-Rom/) and [Supercharge Anywhere](https://github.com/C0mposer/Spyro-1-Supercharge-Anywhere) which this project was originally built off of!
- Thank you to [OddKara](https://github.com/OddKara) for helping me understand how to add sharp turning into this Mod!
- Thank you to [ThirstyWraith](https://decomp.me/u/ThirstyWraith) for helping me optimise space usage in my code as well as understand PSX overlays in Ghidra!
- Thank you to [Jamlotte1](https://www.twitch.tv/jamlotte1) for helping me with the design of Skyro (my custom skin) which is also part of this Mod!
- Thank you to [Jayobean](https://www.twitch.tv/jayobean) for being a Beta playtester!

## Contact

If you need any help setting up Speedway Mod, got questions about it in general, or would like to give me some feedback on it, feel free to reach out to me on discord: **LuminescentSky** :)

## Compiling & Building
If you wish to compile the mod yourself, you'll need a few prerequisites
1. Compiling this mod requires the [psx-modding-toolchain by Redhot](https://github.com/mateusfavarin/psx-modding-toolchain). 
2. Once you have installed it and its pre-requisites, place the folder [from the source code download](https://github.com/C0mposer/Spyro-1-Practice-Codes/archive/refs/heads/master.zip), into the **/games/** folder of your psx-modding-toolchain directory.
3. Place your personal Spyro the Dragon (PAL) bin file renamed to **pal.bin** in **/build/**
4. Browse to the **/mods/SpeedwayMod/** directory, and open the MOD.bat file. From here you should be prompted with multiple options in a command line gui	
	- First type 1 then press enter to compile, then, type 2 for the PAL PS1 version or 4 for the PAL PS2 version. Other versions are not currently supported or were experiments.
	- After it has compiled, type 4 then enter for "Build Iso", then, type 2 for the PAL PS1 version or 4 for the PAL PS2 version. Other versions are not currently supported or were experiments.
	- After it has built, your bin/cue should be in /build/
	- Be sure you built for the correct version. PS2 will not work on PS1/EMU, and visa-versa.
5. If you wish to add Skyro (my skin), download [Scales](https://github.com/Hwd405/SCALES/releases/tag/v1.0.0). Open a command prompt (cmd) and type the following command:
	- ```path/to/Scales.exe -i path/to/pal_SpeedwayMod.bin -c path/to/spyro-s1-ej_skyro.bmp```
	- You can drag and drop each file into the command prompt to automatically fill out the full path. The **spyro-s1-ej_skyro.bmp** can be found in the root directory of this (Speedway Mod) repository.

## Tools Used In Creation

 - [psx-modding-toolchain by Redhot](https://github.com/mateusfavarin/psx-modding-toolchain) for the automation of compiling C code to MIPS object code targeted for the PS1, and building a bin/cue with the mod files injected correctly.
 - [Bizhawk](https://github.com/TASEmulators/BizHawk) for searching/poking PS1 ram values easily
 - [DuckStation](https://duckstation.org/) for quickly testing changes while working on the Mod, with its helpful fast forward feature.
 - [PCSX-Redux](https://github.com/grumpycoders/pcsx-redux/) for its debugging features.
 - [Ghidra](https://github.com/NationalSecurityAgency/ghidra) in combination with the [PSX Executable Loader Plugin](https://github.com/lab313ru/ghidra_psx_ldr) for reverse engineering functions in the game.
 - [Spyro Scales by Hwd405](https://github.com/Hwd405/SCALES/) for adding my custom skin into this Mod.
