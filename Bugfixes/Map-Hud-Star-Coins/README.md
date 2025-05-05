# (Retail) Map HUD Star Coin Fix
The retail World Map HUD doesn't hide the Star Coin outlines when showing the coin itself, which causes them to be visible
as the HUD becomes transparent during animations. This fixes that by hiding the outlines if the HUD is showing the coin!

> [!WARNING]
> This only works for NSMBWer+, as Newer doesn't use the retail HUD!!!

## Installation Instructions
Make sure you're able to compile NewerSMBW/NSMBWer+ before adding this to your code.


Take `wmaphud.yaml` and put it into your `/Kamek` folder. Then take `wmaphud.S` and put it into your `/Kamek/src` folder.

Make sure to reference `wmaphud.yaml` in your project YAML so it gets compiled!


Recompile your code, then put the files from the Build folder into the `NewerRes`/`Code` folder in the Riivolution patch.