#include <asm_setup.S>

.text

.extern AssignAnmScnToLightInfo
.extern LoadStageLighting__Fi
.extern returnToOriginalLoadLighting

.global LoadStageLighting
LoadStageLighting:
    bl AssignAnmScnToLightInfo

    or r3, r29, r29 # get sceneID
    bl LoadStageLighting__Fi

    b returnToOriginalLoadLighting