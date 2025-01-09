

# Spyro 1 Speedway Mod
Speedway Mod is a Romhack for Spyro the Dragon (PAL and NTSC-U) with the concept to turn the entire game into speedways, similar to the ones from Spyro 2 and Spyro 3. The idea is comparable to the "Level Storage" glitch in Spyro Reignited brought to the original Spyro game!

## Download
Go to this website to patch your Spyro the Dragon (PAL or NTSC-U) copy!
- [Speedway Mod Patcher](https://luminescentsky.github.io/SpeedwayMod/)
- [Changelog](https://luminescentsky.github.io/SpeedwayMod/changelog)

## Features
 - Every time you press square to charge, you will supercharge instead!
 - Free flight has been modified to be similar to Spyro 2, where you can press X to "flap your wings" and even do superfly jumps! Try pressing X and Square while flying at the right timing, think of double jump inputs!
 - Every level is a time trial! Your record times are saved once you fully complete a level!
 - Dragons are skipped instantly, so you don't have to wait around while trying to go fast!
 - Warping between levels from the Guidebook can be unlocked by using the All Levels cheat, to make practising individual levels easier!
 - Quality of life improvements like the ability to quit game out of any level to make resetting runs on console easier and quicker!

## Credits

- A huge, huge thanks to [Composer](https://github.com/C0mposer/) for taking so much of their time to teach me how to get started with romhacking, as well as allowing me to use their source code for [Spyro 1 Practice ROM](https://github.com/C0mposer/Spyro-1-Practice-Rom/) and [Supercharge Anywhere](https://github.com/C0mposer/Spyro-1-Supercharge-Anywhere) which this project was originally built off of!
- Thank you also to [OddKara](https://github.com/OddKara) for helping me understand how to add sharp turning into this Mod!

## Contact

If you need any help setting up Speedway Mod, got questions about it in general, or would like to give me some feedback on it, feel free to reach out to me on discord: **LuminescentSky** :)

## Compiling & Building
If you wish to compile the mod yourself, you'll need a few prerequisites
1. Compiling this mod requires the [psx-modding-toolchain by Redhot](https://github.com/mateusfavarin/psx-modding-toolchain). 
2. Once you have installed it and its pre-requisites, place the folder [from the source code download](https://github.com/C0mposer/Spyro-1-Practice-Codes/archive/refs/heads/master.zip), into the **/games/** folder of your psx-modding-toolchain directory.
4. Place your personal Spyro the Dragon bin file either NTSC-U renamed to **spyro1.bin** or PAL renamed to **pal.bin** in **/build/**
5. Browse to the **/mods/SpeedwayMod/** directory, and open the MOD.bat file. From here you should be prompted with multiple options in a command line gui

#### Building BIN/CUE
	
	1. First type 1 then press enter to compile, then, type a number between 1-4 according to your desired version. 5-7 were experiments and will not compile.
	2. After it has compiled, type 4 then enter for "Build Iso", then, type a number between 1-4 according to your desired version. 5-7 were experiments and will not build.
	3. After it has built, your bin/cue should be in /build/
	4. Be sure you built for the correct version. PS2 will not work on PS1/EMU, and visa-versa.

#### Generating xdelta patch
	
	1. Follow the above steps to build your BIN/CUE first. This is important, you need to have the BIN file built to generate an xdelta Patch from it.
	2. After it has compiled, type 5 then press enter for "Generate xdetla Patch", then type a number between 1-4 according to your desired version. 5-7 were experiments and will not work.
	3. After it has built, your xdelta patch should be in /build/
	4. Be sure you built for the correct version. PS2 will not work on PS1/EMU, and visa-versa.
	

## Tools Used In Creation

 - [psx-modding-toolchain by Redhot](https://github.com/mateusfavarin/psx-modding-toolchain) for the automation of compiling C code to MIPS object code targeted for the PS1, and building a bin/cue with the mod files injected correctly.
 - [Bizhawk](https://github.com/TASEmulators/BizHawk) for searching/poking PS1 ram values easily
 - [DuckStation](https://duckstation.org/) for quickly testing changes while working on the Mod, with its helpful fast forward feature.
 - [PCSX-Redux](https://github.com/grumpycoders/pcsx-redux/) for its debugging features.
 - [Ghidra](https://github.com/NationalSecurityAgency/ghidra) in combination with the [PSX Executable Loader Plugin](https://github.com/lab313ru/ghidra_psx_ldr) for reverse engineering functions in the game.
