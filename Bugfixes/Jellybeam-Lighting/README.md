# Jellybeam Lighting Fix
The Jellybeam sprite's lightmask model breaks in zone that don't use the darkness effect, showing as a large white square.

This fixes that by only drawing the model if the zone it's in has darkness lighting enabled.

## Installation Instructions
Make sure you're able to compile NewerSMBW/NSMBWer+ before adding this to your code.


Take `jellyfishFix.yaml` and put it into your `/Kamek` folder. Then take `jellyfishFix.cpp` and put it into your `/Kamek/src` folder.

Make sure to reference `jellyfishFix.yaml` in your project YAML so it gets compiled!


Recompile your code, then put the files from the Build folder into the `NewerRes` folder in the Riivolution patch.