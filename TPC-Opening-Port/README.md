# NSMBW: The Prankster Comets Opening Port
This ports the opening/flipbook sequence from NSMBWTPC to Newer 1.30, as well as NSMBWer+!

![Static Badge](https://img.shields.io/badge/Version-1.0.0-default)
![Static Badge](https://img.shields.io/badge/Supports-NewerSMBW%20(1.3.0)-maroon)
![Static Badge](https://img.shields.io/badge/Supports-NSMBWer+-DCDC73)
![Static Badge](https://img.shields.io/badge/Modifies-Cutscenes-EF6031)
![Static Badge](https://img.shields.io/badge/Ported%20Feature-AA00AA)

If you find any bugs, issues, or have any questions, make an issue in the Issues tab.

![Screenshot of the first scene of the opening, showing Peach's Castle at night.](/TPC-Opening-Port/image.png)

For a video of this in-game, see this video for [NewerSMBW](https://youtu.be/yuk37IsK4eU) and this one for [NSMBWer+](https://youtu.be/MIpEEY4dcnI)!

## Changes
The biggest change is obviously support for NewerSMBW 1.30 and NSMBWer+!

However there were some other changes made:
- Removed unnecessarily large textures from the layout, significantly shrinking the filesize
- Fixed some minor layout animation issues
- Increased the maximum supported number of text characters from 150 to 256
- Made the page TPLs load from an `OpeningPics` folder, rather than from an archive file
- And some other small fixes!

# Editing the Opening
The `PranksterToolbox.exe` tool can edit various file formats made for the mod, such as the Opening binary.
To edit it, open the tool, select the Opening Editor, and in the window that opens, import the `Opening.bin` file. The options that
the tool offers should be pretty straight-forward.


It can be downloaded from [here][toolbox].
Credits obviously go to RedStoneMatt/Asu-chan for making this tool!


## Installation Instructions
First off, take the `Game` folder from the release and drop it onto your Riivolution patch, and it should prompt you to replace some files.
If you haven't recompiled Newer/NSMBWer+'s code in the past, then you're finished, you can load your mod and test the opening!

---
If your mod does use custom code, you'll need to compile this with your code. However, the opening needs the [MoreSFX][moresfx] mod, if you
don't already have it. Install it and make sure your mod works with it installed.

Once MoreSFX is installed, add the following to `/include/asm_sfx.S`:
```cpp
#define SFX_BUTTONPRESS 2000
#define SFX_PAGEFLIP 2001
#define SFX_CHARACTERSWOOO 2002
#define SFX_CHARACTERSHAPPY 2003
#define SFX_BOWSERTHUNDER 2004
#define SFX_CHARACTERSGO 2005
```
If you're already using MoreSFX, you'll need to change the sound effect IDs to the next available values.

Next, add the following filenames to the `SFXNameList` array in `/src/newSfx.cpp`:
```cpp
"ButtonPress",		// 2000
"PageFlip",		// 2001
"CharactersWooo",	// 2002
"CharactersHappy",	// 2003
"BowserThunder",	// 2004
"CharactersGo",		// 2005
```
If you already have custom sound effects, you can change the ID in the comment, but it's not required.

Next up, in `/include/game.h`, find the `SaveBlock` class, and add the following two functions at the bottom of it:
```cpp
    void OrR5AgaginstByteFromWorldAvailableArrayAt0x32ofWorldData(u32 unk1, u32 unk2);
    void ClearR5FromByteInWorldAvailableArrayAt0x32ofWorldData(u32 unk1, u32 unk2);
```
Then, add these functions to the `SaveFile` class:
```cpp
	void SaveCurrentData_Maybe();
	void create_hash_for_savefile();
	void WriteSavefileToRegularBuffer();
```

In `kamek_pal.x`, we need to add a few symbols:
```
    OrR5AgaginstByteFromWorldAvailableArrayAt0x32ofWorldData__9SaveBlockFUiUi = 0x800CE450;
	ClearR5FromByteInWorldAvailableArrayAt0x32ofWorldData__9SaveBlockFUiUi = 0x800CE470;
	create_hash_for_savefile__8SaveFileFv = 0x800E05A0;
	WriteSavefileToRegularBuffer__8SaveFileFv = 0x800E0630;
	SaveCurrentData_Maybe__8SaveFileFv = 0x800E1780;
	AssembleDefaultScWorldMapSettings = 0x80102B50;
```
These do not need to be added to the other `*.x` files, as Kamek will do that automatically.

In your Riivolution patch XML, add the following line so the game can load the page textures:
```xml
<folder external="OpeningPics" disc="/OpeningPics" create="true" />
```

If you're using NewerSMBW, you'll need to disable Newer's opening level. Open the `cutScene.yaml` file,
and remove/comment out the following lines:
```yaml
  - name: DoOpeningLevel
    type: branch_insn
    branch_type: bl
    src_addr_pal: 0x809191D4
    target_func: 'EnterOpeningLevel'
```

If you're using NSMBWer+, you'll need to add a new profile for the opening. Open `/include/profileid.h`, and
add the following line at the bottom of the file, **before** the `Num` line:
```cpp
TPC_OPENING, //804
```
You can change the number in the comment, if you wish.


Once that's done, move the files from the `Code` folder to the `Kamek` folder in your custom code.

Reference `opening.yaml` in your project YAML, and then compile the code. If you have any issues, go over
these steps again to make sure you did nothing wrong, and if that's the case, make an Issue here on Github.

## Other Notes:
Pressing + during the opening will open a dialog prompt that'll skip the opening. You can [edit the BMG][bmg]
and change message ID 55 (0x37) to ask the user if they want to skip it.

TPC replaces the ghost house BRSTMs with custom ones, however these aren't included in this release as it may
interfere with some mods that may replace those tracks. If you want these tracks, get them from TPC's [game files][tpc].

Newer's MessageFont replaces the `:` character with a level node icon, so you may want to edit the font to add the symbol back,
or remove the font entirely and use the retail MessageFont.

[toolbox]: https://github.com/Asu-chan/NSMBWThePranksterComets/raw/refs/heads/clang-no-translations/PranksterToolbox/PranksterToolbox/bin/Release/PranksterToolbox.exe
[moresfx]: https://github.com/Developers-Collective/NSMBW-Custom-Sprites/releases/tag/MoreSFX
[bmg]: https://horizon.miraheze.org/wiki/Game_Text_Editing#BMG_Text_Editing
[tpc]: https://drive.google.com/file/d/11UPNLxT45MTsbRYdXjAEgHRMXuQ9SKNd/view?usp=sharing
