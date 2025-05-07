# Improved Level Nodes
This contains several modifications to the level nodes that make them behave similarly to the retail game.

![Static Badge](https://img.shields.io/badge/Version-1.1.0-default)
![Static Badge](https://img.shields.io/badge/Supports-NewerSMBW%20(1.3.0)-maroon)
![Static Badge](https://img.shields.io/badge/Modifies-World%20Maps%20(Koopatlas)-25a86d)

If you find any bugs, issues, or have any questions, make an issue in the Issues tab.

![Screenshot of the different colored level nodes next to each other.](/Improved-Level-Nodes/image.png)

For a video of this in-game, see [this YouTube video](https://youtu.be/JccIT9YbMeg)!

>[!IMPORTANT]
> Make sure you credit me (as `MandyIGuess`) if you use this for your own project!!!

## Changes
The nodes now use Color animations! The retail game applies CLR animations to the level nodes depending on if it's unlocked, cleared, or
cleared with help from Super Guide. Newer, on the other hand, uses separate models, each with a different texture for the inside of the node.

Not only is this really inefficient, as there's *nine* duplicates of the same model with the only difference being a differently-colored texture,
but it also looks bad because the colors are slightly off compared to the retail game, and, of course, the unlocked levels don't fade from red to black,
they're just a solid color.

This adds support for playing CLR animations, so now there's only a need for one model. The animations that come included with the node model are:
- `cobCourseClose`: Locked level nodes (solid black)
- `cobCourseOpen`: Unlocked (but not cleared) level nodes (fades from black to red)
- `cobCourseSecret`: Level nodes that only have one of it's two exits cleared (solid purple/magenta)
- `cobCourseClear`: Cleared levels (solid blue)

If you want to add your own colors, just make a new CLR animation, and add some kind of condition in `mapdata.cpp` so it appears in-game.

---
The nodes also use Texture Pattern animations now! These animation types allow for the inside of the course node to use different
textures, allowing for proper shop nodes, as well as a new introduction: start nodes!

Start nodes are special level nodes that use level slots `95` - `98`, and do nothing when trying to enter them. Each level slot spawns a node with
an arrow pointing in different direction: `95` for **Left**, `96` for **Up**, `97` for **Right**, and `98` for **Down**.

There are currently 6 different textures to choose from:
- The default course texture
- Course texture with a bag icon, used for shop nodes
- Four textures for start nodes

---
Additionally, the node model itself has been rotated, so the lighting on the node is in the same place it's at in the retail game.

This isn't done in the best way, as the model's main bone has been rotated, rather than it being done through the code, like the rest of the
node's rotation values are done. Trying to rotate it through the code resulted in it never looking quite 'right', so I chose to just do it through
the model.

## Installation Instructions
Take the `SpriteTex` folder (inside the `Resources` folder), and drop it into your NewerSMBW Riivolution patch. You should get a prompt
saying that a `cobCourse.arc.LZ` file already exists. Confirm you want to replace the file.

Download NewerSMBW's source code from the [GitHub repo][newerGit] (press `Code > Download ZIP`). Go through the [setup instructions][newerSetup], and make sure
you are able to **compile NewerSMBW.**

Now, take the `Kamek` folder (inside the `Code` folder), and drop it into your source code. You should get a prompt about there being three files with the same
names. Confirm you want to replace the files, then recompile NewerSMBW. Take the files in the `Build_NewerProject` folder, and put them in `NewerRes` in the
Riivolution patch.

Now, test it in-game. If you have any issues, make sure you set it up correctly.

If you cannot get it to work, make an issue on the Issues page, and I'll try to respond as soon as I can.

## Changelog

### v1.1.0 (May 07th, 2025)
- Level nodes now use Texture Pattern animations! This means shop nodes have the bag icon, and there's now support for Start Nodes!
- For Start Nodes:
  - Level slots XX-95 -> XX-98 cannot be entered anymore, and will display a Start Node instead of a regular node.
  - The HUD will not show a header for these level slots (to restore the header, don't add the modified `hud.cpp` file into your project).
- Newly unlocked levels will now only turn red once the unlock effect plays, like in the retail game.
- The course unlock sound will no longer play when unlocking paths leading to non-levels (such as pipes to other maps).
- Adjusted the position of the unlocking effect so it's properly aligned with the inside of the node.
- The "invalid" sound effect no longer plays when trying to enter a completed one-time stage.
  - This can be re-enabled by uncommenting line `1550` in `pathmanager.cpp` (remove the `//` at the beginning).

### v1.0.0 (June 22nd, 2024)
- Initial release

[newerGit]: https://github.com/Newer-Team/NewerSMBW
[newerSetup]: https://github.com/Newer-Team/NewerSMBW?tab=readme-ov-file#installation