#include <game.h>

// fix M pane sizes and adjust positions of the GAME letters (replaces initializeState_FadeInEndWait)
void FixGameOverLetters(void *self) {
	m2d::EmbedLayout_c *layout = (m2d::EmbedLayout_c*)(((u8*)self)+0x70);

	// adjust M pane sizes
	layout->findTextBoxByName("T_tex_11")->size.x = 60.0f;
	layout->findTextBoxByName("T_tex_06")->size.x = 60.0f;

	// fix overlap issues
	layout->findPaneByName("N_tex_08")->trans.x = -219.0f; // G
	layout->findPaneByName("N_tex_07")->trans.x = -160.0f; // A
	layout->findPaneByName("N_tex_06")->trans.x = -102.0f; // M
}