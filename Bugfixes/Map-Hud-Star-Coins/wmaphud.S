#include <asm_setup.S>
.text

.global fixStarCoinOutlineBug
fixStarCoinOutlineBug:
    stb r0, 0xBB(r3) # replaced instruction

    lwz r3, 0x3E0(r29) # get star coin outline
    li  r0, 0 # new alpha value
    stb r0, 0xB8(r3) # put alpha into outline pane
    blr