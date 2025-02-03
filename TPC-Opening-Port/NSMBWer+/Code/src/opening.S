.text
#include <asm_setup.S>
.align 4

.extern __ptmf_scall
.extern state0_whatever__12dScOpening_cFv, state1_whatever__12dScOpening_cFv, state2_whatever__12dScOpening_cFv
.extern state3_whatever__12dScOpening_cFv, state4_whatever__12dScOpening_cFv, state5_whatever__12dScOpening_cFv
.extern state6_whatever__12dScOpening_cFv, state7_whatever__12dScOpening_cFv, state8_whatever__12dScOpening_cFv

.global callOpeningState__FP12dScOpening_cUi
callOpeningState__FP12dScOpening_cUi:
	stwu sp, -0x10(sp)
	mflr r0
	stw r0, 0x14(sp)
	stw r31, 0xC(sp)

	lis r5, openingStates@h
	ori r5, r5, openingStates@l
	mulli r4, r4, 0xC
	add r12, r5, r4
	bl __ptmf_scall

	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 16
	blr

.data
.global openingStates
openingStates:
	.long 0, -1, state0_whatever__12dScOpening_cFv
	.long 0, -1, state1_whatever__12dScOpening_cFv
	.long 0, -1, state2_whatever__12dScOpening_cFv
	.long 0, -1, state3_whatever__12dScOpening_cFv
	.long 0, -1, state4_whatever__12dScOpening_cFv
	.long 0, -1, state5_whatever__12dScOpening_cFv
	.long 0, -1, state6_whatever__12dScOpening_cFv
	.long 0, -1, state7_whatever__12dScOpening_cFv
	.long 0, -1, state8_whatever__12dScOpening_cFv