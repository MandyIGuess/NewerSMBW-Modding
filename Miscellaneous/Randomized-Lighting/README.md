# Randomized Lighting
This randomizes the model lighting for levels and on the World Map (retail maps only)!

![Static Badge](https://img.shields.io/badge/Version-1.0.0-default)
![Static Badge](https://img.shields.io/badge/Supports-NewerSMBW%20(1.3.0)-maroon)
![Static Badge](https://img.shields.io/badge/Supports-NSMBWer+-DCDC73)
![Static Badge](https://img.shields.io/badge/Modifies-Stages-25a86d)
![Static Badge](https://img.shields.io/badge/Modifies-World%20Maps-skyblue)
![Static Badge](https://img.shields.io/badge/Modifies-Lighting-FFFF00)

## Changes
The lighting will be randomly selected on the World Map, in levels, and in the main menu (players) every time they are reloaded. This applies to
changing worlds, changing the sub-map (like in W3), going through pipes, and so on.

This also allows stages to randomly select one of the four depth-of-field effects used by certain lighting themes, which will effect the background
of the stage.


Additionally, the game will now randomly choose a tileset lighting option (entirely unused in the retail game), which will cause ground tiles and
blocks to appear darker and have a slight tint to it!

If you want to disable the terrain lighting, go to line 3 of `randomLighting.cpp`, change the `true` to `false`, then recompile your code.

> [!NOTE]
> This can and will cause weird oddities with the way some models are lit, do not report these as bugs as they will not be fixed!

## Installation Instructions
Make sure you are able compile [NewerSMBW][newerGit]/[NSMBWer+][nsmbwerGit] before you add this feature. Both repositories have setup instructions
in their README files.

Add the following symbols below to your `kamek_pal.x` file. You do not need to add them to the other `.x` files.
```
GetFogMgr = 0x80164CC0;
LoadBfog = 0x80919900;
CurrentTerrainLightingValue = 0x8042A505;
getTerrainShadingForZone__9dCourse_cFUc = 0x8008E6E0;
returnToOriginalLoadLighting = 0x8092651C;
```

Take `randomLighting.yaml` and put it into your `/Kamek` folder. After that, take `randomLighting.cpp` as well as `randomLighting.S`, and put them
into your `/Kamek/src` folder.

Make sure to reference `randomLighting.yaml` in your project YAML so it gets compiled!

[newerGit]: https://github.com/Newer-Team/NewerSMBW
[nsmbwerGit]: https://github.com/Developers-Collective/NSMBWerPlus