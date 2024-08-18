# Single-Row Number of Players
This rewrites the button functionality code for the Number of Players screen, allowing the 1 button to be on the same row as the other buttons.

![Static Badge](https://img.shields.io/badge/Version-1.0.0-default)
![Static Badge](https://img.shields.io/badge/Supports-NewerSMBW%20(1.3.0)-maroon)
![Static Badge](https://img.shields.io/badge/Supports-NSMBWer+-DCDC73)
![Static Badge](https://img.shields.io/badge/Modifies-User%20Interfaces-blue)

If you find any bugs, issues, or have any questions, make an issue in the Issues tab.

## Changes
The controls have been changed to where it will act as if the 1P button were on the same row as the 2, 3, and 4P buttons. Pressing right on 1
will go to 2, and vice versa. This change only applies to the button *navigation*, the layout has not been changed in any way.

This means you will need to edit the layout yourself, with a tool such as [Benzin][benzin]. The layout can be found here:
- In NSMBW: `/Layout/fileSelectPlayer/fileSelectPlayer.arc`
- In NewerSMBW: `/Others/fileSelectPlayer.arc`

## Installation Instructions
Download NewerSMBW's source code from the [GitHub repo][newerGit] (press `Code > Download ZIP`). Go through the [setup instructions][newerSetup], and make sure
you are able to **compile NewerSMBW.**

Add the three symbols below to your `kamek_pal.x` file. You do not need to add them to the other `.x` files.
```
StateID_ExitAnimeEndWait__15dSelectPlayer_c = 0x80995560;
StateID_StartMemberButtonAnime__15dSelectPlayer_c = 0x80995520;
StateID_ButtonChangeAnimeEndWait__15dSelectPlayer_c = 0x80995460;
```

Take `selectPlayer.yaml` and put it into your `/Kamek` folder. Then take `selectPlayer.cpp` and put it into your `/Kamek/src` folder.

Make sure to reference `selectPlayer.yaml` in your project YAML so it gets compiled!

---

Now, test it in-game. If you have any issues, make sure you set it up correctly.

If you cannot get it to work, make an issue on the Issues page, and I'll try to respond as soon as I can.

[benzin]: https://horizon.miraheze.org/wiki/Benzin
[newerGit]: https://github.com/Newer-Team/NewerSMBW
[newerSetup]: https://github.com/Newer-Team/NewerSMBW?tab=readme-ov-file#installation