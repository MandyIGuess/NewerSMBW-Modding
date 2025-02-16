# Start Node Directions
This allows the Start Nodes to point up, right, left, or down! The retail game only allows for up or right.

![Static Badge](https://img.shields.io/badge/Version-1.0.0-default)
![Static Badge](https://img.shields.io/badge/Supports-NSMBWer+-DCDC73)
![Static Badge](https://img.shields.io/badge/Modifies-World%20Maps-skyblue)

For a video of this in-game, see [this YouTube video](https://youtu.be/ScL3NnMCN8c)!

## Changes
This adds new textures to the Start Node model that point left and down. These can be displayed in-game by changing the last character of the 
Start Node's bone name (`WxSy`) to a value from 0-3:
- 0: Right
- 1: Up
- 2: Left
- 3: Down

Additionally, the World Map HUD can show left/down arrows, this is handled by checking what direction the Start Node points, so you don't 
have to do anything to make this work.

Since the HUD needs icons for the new nodes, an edited version of the retail PictureFont is included. The icons replace the lowercase `b` and `c` 
characters, to ensure compatibility with edited fonts (as most fonts likely won't replace that many characters).

## Installation Instructions
Make sure you are able to compile [NSMBWer+][nsmbwerGit] before you add this feature. Setup instructions can be found in the README file.

Add the following symbols below to your `kamek_pal.x` file. You do not need to add them to the other `.x` files.
```
returnToWmStartLoadModel = 0x808E81D4;
returnToGuideUpdateLevelDisplay = 0x80010D8C;
```

Take `wm_start.yaml` and put it into your `/Kamek` folder.<br>
Then, take `wm_start.cpp` as well as `wm_start.S`, and put them into your `/Kamek/src` folder.

Make sure to reference `wm_start.yaml` in your project YAML so it gets compiled!

### LevelInfo Utils Only
If you're using Ryguy's [LevelInfo utilities][levelinfo] for your mod, then the Start Node icon is controlled by an entry in `LevelInfo.bin`.
To make it work for that, go into `utils.cpp`, and at the bottom of the `levelNumbers` array, add the following two lines:
```cpp
	L"b", //43, left arrow
	L"c", //44, down arrow
```
If you've added icons to this list before, then the numbers in the comment will be incorrect, you can change them if you wish.

After that, you can change the level display number to 43 or 44 for worlds with leftward or downward facing Start Nodes.



In your game files, add `mj2d00_PictureFont_32_RGBA8.brfnt` to your `/Font` folder (or similar). Then, add `cobStart.arc` to your `WorldMap` folder.

[levelinfo]: https://github.com/Ryguy0777/nsmbw-utils/tree/main/levelinfo%20utils
[nsmbwerGit]: https://github.com/Developers-Collective/NSMBWerPlus