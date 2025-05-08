#ifndef __KOOPATLAS_HUD_H
#define __KOOPATLAS_HUD_H

#include "koopatlas/core.h"

class dCyuukan_c {
public:
	void *vtable;
	int checkpoint_id;
	Vec player_spawn_pos;
	u32 direction;
	u8 cur_world, cur_level, cur_area, cur_entrance;
	bool isKinopioInChukan;
	u8 pad[3];
	u32 star_coin_status[3]; // 4 means uncollected
	u32 checkpoint_char_id[2];

	int isCyuukanStart(int related_to_some_player_ids, u8 world, u8 level);
};

class dInfo_c {
public:
	void *vtable;
	u32 _08;
	dCyuukan_c cyuukan;
};

extern "C" void GetScaleFromAspectRatio(Vec2 *in); // 0x800B5140

// W = pane white mat, B = pane black mat
extern bool DontResetPlayerColorsB; // 0x8042A22C
extern bool DontResetPlayerColorsW; // 0x8042A22D
extern u32 MarioPlayerColorB;	  // 0x80358D58
extern GXColor MarioPlayerColorW; // 0x80358D68
void assignPlayerColorsToWindow(nw4r::lyt::Pane *target, int playerID); // 0x800B3C50

class dWMHud_c : public dBase_c {
public:
	dWMHud_c();

	enum Animation {
		IN_GUIDE = 0, IN_LIVES, IN_COURSE,
		LOOP_GUIDE, LOOP_LIVES, LOOP_COURSE,
		OUT_GUIDE, OUT_LIVES, OUT_COURSE,
		IN_ARROW_R, IN_ARROW_L, IN_ARROW_U, IN_ARROW_D,
		LOOP_ARROW_R, LOOP_ARROW_L, LOOP_ARROW_U, LOOP_ARROW_D,
		OUT_ARROW_R, OUT_ARROW_L, OUT_ARROW_U, OUT_ARROW_D,
		IN_SHADOW,
		OUT_SHADOW,
	};

	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	bool layoutLoaded;
	m2d::EmbedLayout_c layout;

	static dWMHud_c *build();
	static dWMHud_c *instance;

	bool doneFirstShow;
	void loadInitially();

	void enteredNode(dKPNode_s *node = 0);
	void leftNode();

	void hideFooter();
	void showFooter();

	void updateLives();

	void hideAll();
	void unhideAll();

private:
	void playAnim(int id);

	dKPNode_s *nodeForCourse;
	void loadInfo();

	bool willShowGuide, guideVisible, tryToHideGuide;
	bool needsToShowCourse, courseHidden;
	bool courseHiddenForNewMap;

	int displayedControllerType;
	void checkExtensionController();

	nw4r::lyt::Pane *N_IconPosXP_00[4], *N_mapArrow_00, *N_cC_02;
	nw4r::lyt::Picture
		*P_cC_X_00[3], *P_cC_Xs_00[3],
		*P_flagSkull_00, *P_marioFace_00,
		*P_luigiFace_00, *P_BkinoFace_00,
		*P_YkinoFace_00, *P_bgShadow_00;
	nw4r::lyt::TextBox
		*T_worldNum_00, *T_cSelect_00, *T_cSelect_pic,
		*T_lifeNumber[4], *T_guideViewLS_00, *T_guideViewL_01;

	// newer features
	nw4r::lyt::Pane *W_world_00, *N_zanki_00;
	nw4r::lyt::Picture *P_bCenter_00, *P_flagSkull_01;
	nw4r::lyt::TextBox *T_levelName_00, *T_levelNameS_00, *T_worldName_00, *T_worldNameS_00;
};
#endif
