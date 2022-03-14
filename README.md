# The Legend of Heroes: Trails of Cold Steel Kai ~ THORS MILITARY ACADEMY 1204 ~
# English Translation Mod for Nintendo Switch version

![logo](https://i.imgur.com/v8q3LSI.png)

Translation mod is based on files from official Steam PC and PS4 releases. 
It works fully only with modded Nintendo Switch. Only Atmosphere is supported.
Only game version 1.0.3 is supported.

Main changes:
- Complete translation of game, textures and movies
- Removed completely fixed kernings for better text rendering
- Many custom text tweaks for better readability and blocking text from overflowing
- Added base support to plugin that will allow switching between Japanese and English audio (currently English dubbing is not added)
- Added custom menu (press R in title menu to get access). Settings are saved to sd:/config/ToCS1/subsdk9.save. Options:
  - FPS 30 or 60, 
  - changing rendering resolution from 360p up to 1080p
  - switching audio language. 
- Removed from save description character names and their levels (game was stuck in infinite loop if name was too long like "Instructor Sara")
- Force enabled Anisotropic Filtering and set it to 16

You can find mod in Releases. Put "atmosphere" folder to root of sdcard. Play game.</br>
It is recommended to OC GPU to at least 460 MHz for best experience.

Signs of properly installed mod:
- Title menu descriptions are translated
- Logo in right upper corner is translated
- "Translation mod made by MasaGratoR & Graber" is rendered by plugin in right lower corner.
![SCR1](https://i.imgur.com/v4o1Epc.jpg)

Known issues:
- When 60 FPS is activated, cutscene slow motion timing is broken, so slow motion takes several times as long to get back to original speed. This happens only few times across whole game, only one cutscene in 5th chapter is glitched because of that, but there were no softlocks. It is recommended to activate Turbo Mode when you have slow motion for more than 3 seconds.

Special thanks to:
- Graber for providing files and main technical support based on experience from older games
- Inochi-PM for providing reedits of English logos
- TwnKey for providing source code containing informations how scripts are build (not directly involved with this project) </br>
 https://github.com/TwnKey/SenScriptsDecompiler/blob/main/headers/CS1InstructionsSet.h
- Ouroboros for info about dialogue opcodes (not directly involved with this project)
- thesupersonic16 for PKG unpacker (not directly involved with this project)</br>
 https://github.com/thesupersonic16/PKGTool
- XSEED North American Translation Team for bringing the best translation possible to West (not directly involved with this project). Full credits are available in Ending video. 
