#include "koopatlas/hud.h"
#include <newer.h>

// Use these to show/hide the View Map and World Select prompts
#define SHOW_MAP_VIEW	  true
#define SHOW_WORLD_SELECT true

// Use this to enable functionality for Newer-specific HUD features
#define NEWER_FEATURES  false
// Tweaks the way some things behave depending on the layout being used
// 0 is the more retail-based HUD, while 1 is the more NSMBU-like HUD (see the README for more info)
#define LYT_USED 0

// Enabling this makes the first path node in the first map display a Start Node arrow instead of a dot
#define FIRST_NODE_IS_START_NODE false

dWMHud_c *dWMHud_c::instance = 0;

dWMHud_c *dWMHud_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dWMHud_c));
	dWMHud_c *c = new(buffer) dWMHud_c;

	instance = c;
	return c;
}

dWMHud_c::dWMHud_c() {
	layoutLoaded = false;
	displayedControllerType = -1;
}

int dWMHud_c::onCreate() {
	if (!layoutLoaded) {
		bool gotFile = layout.loadArc("corseSelectUIGuide/corseSelectUIGuide.arc", false);
		if (!gotFile)
			return false;

		layout.build("corseSelectUIGuide_37.brlyt");
		layout.drawOrder = 0;

		if (!IsWideScreen()) {
			layout.clipX = 0.0f;
			layout.clipY = 52.0f;
			layout.clipWidth = 640.0f;
			layout.clipHeight = 352.0f;
			layout.clippingEnabled = true;
		}

		// fix scale for 4:3 mode
		Vec2 newScale;
		GetScaleFromAspectRatio(&newScale);
		layout.findPaneByName("N_proportionL_00")->scale = newScale;
		layout.findPaneByName("N_proportionR_00")->scale = newScale;
		layout.findPaneByName("N_proportionC_00")->scale = newScale;

		static const char *brlanNames[] = {
			"corseSelectUIGuide_37_inInfo.brlan",
			"corseSelectUIGuide_37_loopInfo.brlan",
			"corseSelectUIGuide_37_outInfo.brlan",
			"corseSelectUIGuide_37_inMapArrow.brlan",
			"corseSelectUIGuide_37_loopArrow.brlan",
			"corseSelectUIGuide_37_outMapArrow.brlan",
			"corseSelectUIGuide_37_inShadow.brlan",
			"corseSelectUIGuide_37_outShadow.brlan"
		};
		static const char *groupNames[] = {
			"A00_underGuide", "A01_zanki", "A02_courseInfo",
			"A00_underGuide", "A01_zanki", "A02_courseInfo",
			"A00_underGuide", "A01_zanki", "A02_courseInfo",
			"B00_ArrowRight", "B01_ArrowLeft", "B02_ArrowUp", "B03_ArrowUnder",
			"B00_ArrowRight", "B01_ArrowLeft", "B02_ArrowUp", "B03_ArrowUnder",
			"B00_ArrowRight", "B01_ArrowLeft", "B02_ArrowUp", "B03_ArrowUnder",
			"C00_shadow",
			"C00_shadow"
		};
		static const int groupIDs[] = {
			0, 0, 0,
			1, 1, 1,
			2, 2, 2,
			3, 3, 3, 3,
			4, 4, 4, 4,
			5, 5, 5, 5,
			6,
			7
		};
		layout.loadAnimations(brlanNames, 8);
		layout.loadGroups(groupNames, groupIDs, 23);
		layout.disableAllAnimations();

		layout.resetAnim(IN_GUIDE);
		layout.resetAnim(IN_LIVES);
		layout.resetAnim(IN_COURSE);

		layout.findPaneByName("N_guideViewC_00")->SetVisible(SHOW_WORLD_SELECT);
		layout.findPaneByName("N_guideViewL_00")->SetVisible(SHOW_MAP_VIEW);

		static const char *paneNames[] = {
			"N_IconPos1P_00", "N_IconPos2P_00", "N_IconPos3P_00",
			"N_IconPos4P_00", "N_mapArrow_00", "N_cC_02"
		};
		layout.getPanes(paneNames, &N_IconPosXP_00[0], 6);

		static const char *pictureNames[] = {
			"P_cC_1_00", "P_cC_2_00", "P_cC_3_00",
			"P_cC_1s_00", "P_cC_2s_00", "P_cC_3s_00",
			"P_flagSkull_00", "P_marioFace_00", "P_luigiFace_00",
			"P_BkinoFace_00", "P_YkinoFace_00", "P_bgShadow_00"
		};
		layout.getPictures(pictureNames, &P_cC_X_00[0], 12);

		static const char *textBoxNames[] = {
			"T_worldNum_00", "T_cSelect_00", "T_cSelect_pic",
			"T_lifeNumber_00", "T_lifeNumber_01", "T_lifeNumber_02",
			"T_lifeNumber_03", "T_guideViewLS_00", "T_guideViewL_01"
		};
		layout.getTextBoxes(textBoxNames, &T_worldNum_00, 9);

		static const char *bmgNames[] = {"T_guideViewC_00", "T_guideViewR_00", "T_world_00"};
		static const int bmgMsgIDs[] = {0xB, 0xC, 0x3C};
		layout.setLangStrings(bmgNames, bmgMsgIDs, 4, 3);

		updateLives();

		if (NEWER_FEATURES) {
			P_bCenter_00 = layout.findPictureByName("P_bCenter_00");
			P_flagSkull_01 = layout.findPictureByName("P_flagSkull_01");
			P_bCenter_00->alpha = 210;

			W_world_00 = layout.findPaneByName("W_world_00");
			N_zanki_00 = layout.findPaneByName("N_zanki_00");

			static const char *newerTextBoxNames[] = {"T_levelName_00", "T_levelNameS_00", "T_worldName_00", "T_worldNameS_00"};
			layout.getTextBoxes(newerTextBoxNames, &T_levelName_00, 4);
		}

		layoutLoaded = true;
	}
	return true;
}

int dWMHud_c::onExecute() {
	if (!layoutLoaded)
		return true;

	if (tryToHideGuide && !layout.isAnimOn(IN_GUIDE)) {
		tryToHideGuide = false;
		playAnim(OUT_GUIDE);
		playAnim(OUT_LIVES);
	}

	if (willShowGuide && !layout.isAnimOn(OUT_GUIDE) && !layout.isAnimOn(IN_GUIDE)) {
		loadInfo();
		if (!dScKoopatlas_c::instance->pathManager.isMoving) {
			willShowGuide = false;
			playAnim(IN_GUIDE);
			playAnim(IN_LIVES);
			guideVisible = true;
		}
	}

	dKPNode_s *node = dScKoopatlas_c::instance->pathManager.currentNode;
	if (node->type == dKPNode_s::CHANGE) {
		courseHiddenForNewMap = true;
	}

	updateLives();
	checkExtensionController();

	layout.execAnimations();
	layout.update();
	return true;
}

int dWMHud_c::onDraw() {
	if (!layoutLoaded)
		return true;
	layout.scheduleForDrawing();
	return true;
}

int dWMHud_c::onDelete() {
	dWMHud_c::instance = 0;
	if (!layoutLoaded)
		return true;
	return layout.free();
}


void dWMHud_c::hideAll() {
	for (int i = 0; i < 3; i++) {
		if (layout.isAnimOn(i))
			layout.grpHandlers[i].frameCtrl.flags = 3; // flags: NO_LOOP | REVERSE
		else
			layout.enableNonLoopAnim(OUT_GUIDE + i);
	}
}
void dWMHud_c::unhideAll() {
	for (int i = 0; i < 3; i++) {
		if (layout.isAnimOn(OUT_GUIDE + i))
			layout.grpHandlers[OUT_GUIDE + i].frameCtrl.flags = 3; // flags: NO_LOOP | REVERSE
		else
			layout.enableNonLoopAnim(i);
	}
}

void dWMHud_c::hideFooter() {}
void dWMHud_c::showFooter() {}

void dWMHud_c::playAnim(int id) {
	if (layout.isAnimOn(id))
		layout.grpHandlers[id].frameCtrl.flags = 3; // NO_LOOP | REVERSE
	else
		layout.enableNonLoopAnim(id);
}


void dWMHud_c::loadInitially() {
	if (doneFirstShow)
		return;

	doneFirstShow = true;
	needsToShowCourse = true;

	if (!dScKoopatlas_c::instance->pathManager.isMoving)
		enteredNode();
}

void dWMHud_c::loadInfo() {
	bool isLevelNode = nodeForCourse->type == dKPNode_s::LEVEL;
	N_cC_02->SetVisible(isLevelNode);
	P_flagSkull_00->SetVisible(isLevelNode);
	if (LYT_USED) {
		N_zanki_00->trans.y = (isLevelNode) ? 13.0f : 74.0f;
	}

	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	int worldNum = save->newerWorldID;
	WriteNumberToTextBox(&worldNum, T_worldNum_00, false);
	int levelNum = 0;

	if (NEWER_FEATURES) {
		for (int i = 0; i < 2; i++) {
			P_bCenter_00->colours[i] = save->fsHintColours[1];
			P_bCenter_00->colours[i+2] = save->fsHintColours[0];
		}

		// hack of the century: we're using the function below to recolor the window pane
		// this is normally used for coloring wnds based on the player it's for (ex. pause menu)
		// however we're gonna temporarily repurpose it

		DontResetPlayerColorsW = true; // force it to NOT update the color values
		DontResetPlayerColorsB = true;
		MarioPlayerColorW = save->fsHintColours[0]; // set mario's white color to the world color,
		MarioPlayerColorB = 0xFFFFFF00; // and black color to the pane's black mat color
		assignPlayerColorsToWindow(W_world_00, 0); // call it for 0 (mario) since that's the data we replaced
		DontResetPlayerColorsW = false; // allow them to be reset the next time that function runs
		DontResetPlayerColorsB = false;

		T_levelName_00->SetVisible(isLevelNode);
		T_levelNameS_00->SetVisible(isLevelNode);
	}

	if (isLevelNode) {
		u8 worldID = nodeForCourse->levelNumber[0]-1;
		u8 levelID = nodeForCourse->levelNumber[1]-1;
		dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(worldID, levelID);
		if (level) {
			wchar_t worldStr[4], levelStr[4];
			getWorldString(level->displayWorld, worldStr);
			getLevelString(level->displayLevel, levelStr);
			levelNum = level->displayLevel;

			T_worldNum_00->SetString(worldStr);
			
			if (level->displayLevel > 20) {
				T_cSelect_00->SetVisible(false);
				T_cSelect_pic->SetVisible(true);
				T_cSelect_pic->SetString(levelStr);
			} else {
				T_cSelect_pic->SetVisible(false);
				T_cSelect_00->SetVisible(true);
				T_cSelect_00->SetString(levelStr);
			}

			// CHECKPOINT/TEMPORARY COLLECTION
			// in the retail game, if you get the checkpoint and die, a flag is
			// shown on the map HUD and the Star Coins you've collected so far
			// are displayed, but are slightly transparent
			dInfo_c *info = (dInfo_c*)GameMgrP;
			u8 tempWorld = info->cyuukan.cur_world, tempLevel = info->cyuukan.cur_level;

			// handle the checkpoint flag first
			if (!NEWER_FEATURES) {
				P_flagSkull_00->SetVisible(false);
				for (int i = 0; i < 2; i++) {
					u32 hasCheckpoint = info->cyuukan.isCyuukanStart(i, tempWorld, tempLevel);
					if (hasCheckpoint != 0 && ((tempWorld == worldID) && (tempLevel == levelID)))
						P_flagSkull_00->SetVisible(true);
				}
			}

			// handle coin visibility
			bool hasCoins = (level->flags & 2);
			N_cC_02->SetVisible(hasCoins);
			if (LYT_USED) {
				N_zanki_00->trans.y = (hasCoins) ? 13.0f : 46.0f;
			}

			if (hasCoins) {
				for (int coin = 0; coin < 3; coin++) {
					// set everything to normal
					P_cC_X_00[coin]->SetVisible(true);
					P_cC_Xs_00[coin]->alpha = 0xFF;
					P_cC_X_00[coin]->alpha = 0xFF;

					// do we have that coin saved for this level? if not, do the shit
					u32 isCollect = save->CheckIfCoinCollected(worldID, levelID, coin);
					if ((isCollect & 0xFF) == 0) {
						// make sure we're at the level with the temp info, and the coin is not uncollected
						if ((worldID == tempWorld) && (levelID == tempLevel) && (info->cyuukan.star_coin_status[coin] != 4)) {
							// we have the temp coin, hide the outline and show the real one with transparency
							P_cC_Xs_00[coin]->alpha = 0;
							
							P_cC_X_00[coin]->SetVisible(true);
							P_cC_X_00[coin]->alpha = 0x46;
						} else {
							// we don't have it, hide the main one
							P_cC_X_00[coin]->SetVisible(false);
						}
					} else {
						// it's already saved to the file, just show it without any transparency
						P_cC_X_00[coin]->SetVisible(true);
						P_cC_Xs_00[coin]->alpha = 0; // mandy 1 - nintendo 0
					}
				}
			}

			if (NEWER_FEATURES) {
				const char *lname = dLevelInfo_c::s_info.getNameForLevel(level);
				wchar_t levelName[64], worldName[64];
				for (int i = 0; i < 64; i++) {
					levelName[i] = (unsigned short)lname[i];
				}
				T_levelName_00->SetString(levelName);
				T_levelNameS_00->SetString(levelName);

				// handle clear flags
				u32 conds = save->GetLevelCondition(worldID, levelID);
				int normalState = 0, secretState = 0; // 0 = invisible, 1 = visible, 2 = faded

				if ((conds & COND_BOTH_EXITS) && (level->flags & 0x30) == 0x30) {
					// If this level has two exits and one of them is already collected, then show the faded flags
					normalState = 2;
					secretState = 2;
				}

				if ((conds & COND_NORMAL) && (level->flags & 0x10))
					normalState = 1;
				if ((conds & COND_SECRET) && (level->flags & 0x20))
					secretState = 1;

				P_flagSkull_00->alpha = (normalState == 2) ? 80 : 255;
				P_flagSkull_00->SetVisible(normalState > 0);

				P_flagSkull_01->alpha = (secretState == 2) ? 80 : 255;
				P_flagSkull_01->SetVisible(secretState > 0);

				// adjust position if star coins aren't being shown
				if (LYT_USED) {
					P_flagSkull_00->trans.x = (hasCoins) ? 126.0f : 27.0f;
				} else {
					P_flagSkull_00->trans.x = (hasCoins) ? 190.0f : 91.0f;
				}
			}
		} else {
			T_worldNum_00->SetString(L"?");
			T_cSelect_00->SetString(L"?");
			T_cSelect_00->SetVisible(true);
			T_cSelect_pic->SetVisible(false);
			N_cC_02->SetVisible(false);
			P_flagSkull_00->SetVisible(false);
			if (NEWER_FEATURES) {
				T_levelName_00->SetString(L"Unknown Level Name!");
				T_levelNameS_00->SetString(L"Unknown Level Name!");
			}
		}
	} else {
		// STOP node, just show a dot and we're finished
		T_cSelect_00->SetVisible(false);
		T_cSelect_pic->SetVisible(true);
		T_cSelect_pic->SetString(L"6");
		levelNum = 43; // dot

		if (FIRST_NODE_IS_START_NODE) { // start node in Yoshi's Island
			if ((worldNum == 1) && (save->current_path_node == 0)) {
				T_cSelect_pic->SetString(L"4");
				levelNum = 44; // start right
			}
		}
	}

	// do our world name
	if (NEWER_FEATURES) {
		const char *wname = save->newerWorldName;
		wchar_t worldName[64];
		for (int i = 0; i < 64; i++) {
			worldName[i] = (unsigned short)wname[i];
		}

		// don't append level num in the warp zone
		if (!dScKoopatlas_c::instance->warpZoneHacks && LYT_USED) {
			int wnamelen = strlen(wname);
			getWorldNameAndLevelNumString(levelNum, &worldName[wnamelen]);
		}
		T_worldName_00->SetString(worldName);

		// makes pictureFont icons shadowy
		if (LYT_USED) {
			int sidx = 0;
			while (worldName[sidx]) {
				if (worldName[sidx] == 11) {
					worldName[sidx+1] = 0x200 | (worldName[sidx+1] & 0xFF);
					sidx += 2;
				}
				sidx++;
			}
		}
		T_worldNameS_00->SetString(worldName);

		T_worldName_00->colour1 = save->hudTextColours[0];
		T_worldName_00->colour2 = save->hudTextColours[1];
	}

	// only show course info once we're at our node and have our info
	if (needsToShowCourse) {
		layout.enableNonLoopAnim(IN_COURSE);
		needsToShowCourse = false;
		courseHidden = false;
	}
}


void dWMHud_c::enteredNode(dKPNode_s *node) {
	if (node == 0)
		node = dScKoopatlas_c::instance->pathManager.currentNode;

	if ((node->type == dKPNode_s::LEVEL || node->type == dKPNode_s::STOP || (node->getAvailableExitCount() == 1)) && doneFirstShow) {
		willShowGuide = true;
		nodeForCourse = node;
	}
}

void dWMHud_c::leftNode() {
	dKPNode_s *node = dScKoopatlas_c::instance->pathManager.currentNode;

	if (guideVisible) {
		guideVisible = false;
		tryToHideGuide = true;
	}

	// hide the course info box if we're exiting a world
	if (node->type == dKPNode_s::WORLD_CHANGE && !courseHidden) {
		if (!courseHiddenForNewMap)
			layout.enableNonLoopAnim(OUT_COURSE);
		courseHiddenForNewMap = false;
		courseHidden = true; // don't repeat for the inevitable 2nd world change node
		needsToShowCourse = true; // show it once we stop at our next node
	}
}


void dWMHud_c::updateLives() {
	static const int LogicalPlayerIDs[] = {0,1,3,2};

	P_marioFace_00->SetVisible(false);
	P_luigiFace_00->SetVisible(false);
	P_BkinoFace_00->SetVisible(false);
	P_YkinoFace_00->SetVisible(false);

	int playerCount = 0;

	for (int i = 0; i < 4; i++) {
		// The part in setupLives()
		int playerID = LogicalPlayerIDs[i];
		int slotID = SearchForIndexOfPlayerID(playerID);
		int lives = Player_Lives[slotID];
		int length = 2;

		WriteNumberToTextBox(&lives, &length, T_lifeNumber[slotID], true);

		// The part in setupIconThings()
		if (QueryPlayerAvailability(slotID)) {
			playerCount++;

			nw4r::lyt::Pane *facePane = (&P_marioFace_00)[playerID];
			facePane->trans = N_IconPosXP_00[playerCount - 1]->trans;
			facePane->SetVisible(true);
		}
	}

	for (int i = 0; i < 4; i++)
		N_IconPosXP_00[i]->SetVisible(false);
	N_IconPosXP_00[playerCount - 1]->SetVisible(true);
}

void dWMHud_c::checkExtensionController() {
	int type = RemoconMng->controllers[0]->controllerType;

	if (type != displayedControllerType) {
		displayedControllerType = type;
		GameMgrP->currentControllerType = type != 0;

		WriteBMGToTextBox(T_guideViewLS_00, GetBMG(), 0x4, 0xA, 0);
		WriteBMGToTextBox(T_guideViewL_01,  GetBMG(), 0x4, 0xF, 0);
	}
}
