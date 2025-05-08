# Retail Map HUD in NewerSMBW
This replaces the Koopatlas HUD with a recreation of the retail World Map HUD!

![Static Badge](https://img.shields.io/badge/Version-1.0.0-default)
![Static Badge](https://img.shields.io/badge/Supports-NewerSMBW%20(1.3.0)-maroon)
![Static Badge](https://img.shields.io/badge/Modifies-User%20Interfaces-blue)
![Static Badge](https://img.shields.io/badge/Modifies-World%20Maps%20(Koopatlas)-25a86d)

If you find any bugs, issues, or have any questions, make an issue in the Issues tab.

![Screenshot of the new World Map HUD.](/Improved-Level-Nodes/image.png)

<!-- For a video of this in-game, see [this YouTube video](https://youtu.be/pleaseMakeAyoutubeVideo)! -->

>[!IMPORTANT]
> Make sure you credit me (as `MandyIGuess`) if you use this for your own project!!!

## Changes
NewerSMBW's Koopatlas HUD has been replaced with the one from the retail game!

Besides the biggest change, this is designed to function similarly to the retail HUD, so it'll work with the retail layout! If you want Newer's player icons,
the mod already comes with a version of the layout with the modified player icons.

Additionally, there's some customizable settings at the top of `hud.cpp`, see the **Additional Features** section for more info!

## Installation Instructions
Download NewerSMBW's source code from the [GitHub repo][newerGit] (press `Code > Download ZIP`). Go through the [setup instructions][newerSetup], and make sure
you are able to **compile NewerSMBW.**

Add the symbols below to your `kamek_pal.x` file. You do not need to add them to the other `.x` files.
```
GetScaleFromAspectRatio = 0x800B5140;
CheckIfCoinCollected__9SaveBlockFiii = 0x800CE300;
isCyuukanStart__10dCyuukan_cFiUcUc = 0x8008F000;
DontResetPlayerColorsB = 0x8042A22C;
DontResetPlayerColorsW = 0x8042A22D;
MarioPlayerColorB = 0x80358D58;
MarioPlayerColorW = 0x80358D68;
assignPlayerColorsToWindow__FPQ34nw4r3lyt4Panei = 0x800B3C50;
```

Now, take the `Kamek` folder (inside the `Code` folder), and drop it into your source code. You should get a prompt about there being 4 files with the same
names. Confirm you want to replace the files.

### Optional Change
Koopatlas calls the function to update the course info before Mario actually reaches the destination path node, which looks strange with this HUD (as it doesn't hide while walking).

To fix this, open `/src/koopatlas/pathmanager.cpp`, then go down to line 1321, and replace the following line:
```cpp
if (distToEnd < 64.0f && dWMHud_c::instance) {
```
with
```cpp
if (distToEnd < 20.0f && dWMHud_c::instance) {
```

### Additional Features
Opening `hud.cpp` will show some quick options at the top of the file, this section explains all of these!

>[!NOTE]
> Make sure to recompile your code whenever you change any settings!


**SHOW_MAP_VIEW** and **SHOW_WORLD_SELECT** will toggle visibility of the View Map and Select World button prompts, since those features don't exist in Newer.
They're visible by default, to disable them, change `true` to `false`.


At the bottom is **FIRST_NODE_IS_START_NODE**, which is a small hack that'll make the first path node of the first map show as a Start Node (if it's a STOP node).
This is for maps like Yoshi's Island, so that the HUD doesn't display a stop icon on the Start Node doodad!


The biggest options are for enabling Newer's HUD features! Before enabling this, make sure to add one of the custom layouts to your mod.

The **NEWER_FEATURES** value will enable/disable all of the code for Newer's HUD features. They're disabled by default.


There are two versions of the Newer layout that you can use, one styled like the retail HUD, and one that's similar to the HUD from NSMBU!
Since both will function slightly differently, there's an additional option that you need to change, named **LYT_USED**.
- If you're using the retail-based layout, keep it as `0`.
- If you're using the NSMBU-based layout, then change it to `1`.

When choosing the layout you want, rename the archive you're using to just `corseSelectUIGuide.arc`, then you're all set.

---
Once you're ready, recompile NewerSMBW, then take the files in the `Build_NewerProject` folder, and put them in `NewerRes` in the Riivolution patch.

Now, test it in-game. If you have any issues, make sure you set it up correctly.

If you cannot get it to work, make an issue on the Issues page, and I'll try to respond as soon as I can.

## Changelog

### v1.0.0 (May 08th, 2025)
- Initial release

[newerGit]: https://github.com/Newer-Team/NewerSMBW
[newerSetup]: https://github.com/Newer-Team/NewerSMBW?tab=readme-ov-file#installation