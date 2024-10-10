# Game Over Fix (for Modern MarioFont)
This simple code hack fixes the spacing issues that appear on the Game Over layout when using the modern MarioFont.

![Static Badge](https://img.shields.io/badge/Version-1.0.0-default)
![Static Badge](https://img.shields.io/badge/Supports-NewerSMBW%20(1.3.0)-maroon)
![Static Badge](https://img.shields.io/badge/Supports-NSMBWer+-DCDC73)
![Static Badge](https://img.shields.io/badge/Modifies-User%20Interfaces-blue)

If you find any bugs, issues, or have any questions, make an issue in the Issues tab.

![Comparison of the differences between each version.](/Game-Over-Fix-For-Modern-MarioFont/image.png)

## Changes
The most notable issue is that the *M* is so large that it line-breaks and appears above the other letters. This was easily fixed by resizing the
textboxes. After that, the overlap issue with the entire *GAME* word was fixed. The *G*, *A*, and *M* letters were moved to the left slightly, so the
letters no longer overlap.

## Installation Instructions
Download NewerSMBW's source code from the [GitHub repo][newerGit] (press `Code > Download ZIP`). Go through the [setup instructions][newerSetup], and make sure
you are able to **compile NewerSMBW.**

Take `gameOver.yaml` and put it into your `/Kamek` folder. Then take `gameOver.cpp` and put it into your `/Kamek/src` folder.

Make sure to reference `gameOver.yaml` in your project YAML so it gets compiled!

---

Now, test it in-game. If you have any issues, make sure you set it up correctly.

If you cannot get it to work, make an issue on the Issues page, and I'll try to respond as soon as I can.

[newerGit]: https://github.com/Newer-Team/NewerSMBW
[newerSetup]: https://github.com/Newer-Team/NewerSMBW?tab=readme-ov-file#installation