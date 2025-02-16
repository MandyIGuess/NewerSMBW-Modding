#include <game.h>
#include <profileid.h>

void newDirectionsForWmStart(void *self, int m_param) {
	m3d::anmTexPat_c *texpat = (m3d::anmTexPat_c*)((int)(self) + 0x1B8);
	int type = m_param >> 16 & 0xF; // nybble 8

	if (type < 4)
		texpat->setFrameForEntry(type, 0);
}

static const wchar_t *icons[] = {
	L"4", // right
	L"5", // up
	L"b", // left
	L"c", // down
};

void fixCourseIconForStartNode(nw4r::lyt::TextBox *T_cSelect_pic) {
	fBase_c *node = fBase_c::searchByProfileId(ProfileId::WM_START);
	int type = node->settings >> 16 & 0xF;

	if (type > 3)
		type = 0;
	T_cSelect_pic->SetString(icons[type]);
}