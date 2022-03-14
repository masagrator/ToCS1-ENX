# The Legend of Heroes: Trails of Cold Steel Kai ~ THORS MILITARY ACADEMY 1204 ~
# English Translation Mod for Nintendo Switch version

Translation mod is based on files from official Steam PC and PS4 releases. 
It works fully only with modded Nintendo Switch. Only Atmosphere is supported.
Only game version 1.0.3 is supported.

Main changes:
- Complete translation of game, textures and movies
- Removed completely fixed kernings for better text rendering
- Many custom text tweaks for better readability and blocking text from overflowing
- Added base support to plugin that will allow switching between Japanese and English audio (currently English dubbing is not added)
- Added custom menu that allows enabling 60 FPS, changing rendering resolution and switching audio language (press R in title menu to get access). Settings are saved to sd:/config/ToCS1/subsdk9.save
- Removed from save description character names and their levels (game was stuck in infinite loop if name was too long like "Instructor Sara")
- Force enabled Anisotropic Filtering and set it to 16

You can find mod in Releases. Put "atmosphere" folder to root of sdcard. Play game.</br>
It is recommended to OC GPU to at least 460 MHz for best experience.

Known issues:
- When 60 FPS is activated, cutscene slow motion timing is broken, so they are taking many times more time to get back to original speed. This happens only few times across whole game, only one cutscene in 5th chapter is glitched because of that, but there were no softlocks. It is recommended to activate Turbo Mode when you have slow motion for more than 3 seconds.


Special thanks to:
- Graber for providing files and main technical support based on experience from older games
- TwnKey for providing source code containing informations how scripts are build </br>
 https://github.com/TwnKey/SenScriptsDecompiler/blob/main/headers/CS1InstructionsSet.h
- Ouroboros for info about dialogue opcodes
- thesupersonic16 for PKG unpacker</br>
 https://github.com/thesupersonic16/PKGTool
- Inochi-PM for providing new logos
