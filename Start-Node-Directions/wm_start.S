.text
#include <asm_setup.S>

.align 4

.extern newDirectionsForWmStart__FPvi
.extern returnToWmStartLoadModel
.extern fixCourseIconForStartNode__FPQ34nw4r3lyt7TextBox
.extern returnToGuideUpdateLevelDisplay

.global StartNodeDirections
StartNodeDirections:
    or r3, r29, r29
    lwz r4, 4(r29)
    bl newDirectionsForWmStart__FPvi

    b returnToWmStartLoadModel

.global FixStartNodeIcon
FixStartNodeIcon:
    lwz r3, 0x3B8(r30) # T_cSelect_pic
    bl fixCourseIconForStartNode__FPQ34nw4r3lyt7TextBox

    b returnToGuideUpdateLevelDisplay