#include "opening.h"

extern "C" u32 AssembleDefaultScWorldMapSettings();
extern void callOpeningState(dScOpening_c* handler, u32 state);

void *EGG__Heap__alloc(unsigned long size, int unk, void *heap);
void EGG__Heap__free(void *ptr, void *heap);

dScOpening_c *dScOpening_c::instance = 0;

dScOpening_c *dScOpening_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dScOpening_c));
	dScOpening_c *c = new(buffer) dScOpening_c;

	instance = c;
	return c;
}

dScOpening_c::dScOpening_c() {
	Actors aYesNoWindow = translateActorID(YES_NO_WINDOW);
	yesNoWindow = (dYesNoWindow_c*)CreateParentedObject(aYesNoWindow, this, 0, 0);

	Actors aSelectCursor = translateActorID(SELECT_CURSOR);
	CreateParentedObject(aSelectCursor, this, 0, 0);
}

TPLPalette *lastLeft, *lastRight, *currLeft, *currRight;

int dScOpening_c::onCreate() {
	*CurrentDrawFunc = OpeningDrawFunc;

	if (!layoutLoaded) {
		if (!layout.loadArc("opening.arc", false))
			return false;

		static const char *brlanNames[] = {
			"opening_artIdle.brlan",
			"opening_pageNext.brlan",
			"opening_pageBefore.brlan",
			"opening_textboxAppear.brlan",
			"opening_textboxIdle.brlan",
			"opening_textboxDisappear.brlan",
			"opening_buttonAppear.brlan",
			"opening_buttonIdle.brlan",
			"opening_buttonDisappear.brlan",
		};

		static const char *groupNames[] = {
			"A00_flipbook", "A00_flipbook", "A00_flipbook",
			"B00_textBox", "B00_textBox", "B00_textBox",
			"C00_button", "C00_button", "C00_button",
		};

		static const int groupIDs[] = {
			0, 1, 2,
			3, 4, 5,
			6, 7, 8,
		};

		layout.build("opening.brlyt");

		layout.loadAnimations(brlanNames, 9);
		layout.loadGroups(groupNames, groupIDs, 9);
		layout.disableAllAnimations();

		layoutLoaded = true;

		N_flipbook_00 = layout.findPaneByName("N_flipbook_00");
		P_rightPage_00 = layout.findPictureByName("P_rightPage_00");
		P_leftPage_00 = layout.findPictureByName("P_leftPage_00");
		P_orightPage_00 = layout.findPictureByName("P_orightPage_00");
		P_oleftPage_00 = layout.findPictureByName("P_oleftPage_00");
		P_nrightPage_00 = layout.findPictureByName("P_nrightPage_00");
		P_nleftPage_00 = layout.findPictureByName("P_nleftPage_00");
		T_DialogueTxt_00 = layout.findTextBoxByName("T_DialogueTxt_00");
		T_DialogueSha_00 = layout.findTextBoxByName("T_DialogueSha_00");

		if (IsWideScreen()) {
			N_flipbook_00->scale.x *= 1.372693726937269f;
		}

		this->loadIdlePic();
		layout.enableNonLoopAnim(0);
		layout.enableNonLoopAnim(6);
	}

	openingFile = (OpeningBinary*)LoadFile(&fileHandle, "/NewerRes/Opening.bin");

	return true;
}

int dScOpening_c::onDelete() {
	FreeFile(&fileHandle);
	FreeFile(&picHandle);

	// free memory
	EGG__Heap__free(currLeft, GameHeaps[2]);
	EGG__Heap__free(currRight, GameHeaps[2]);
	return layout.free();
}

int dScOpening_c::onExecute() {
	// OSReport("executing! %d %d %d\n", layout.isAnimOn(0), layout.isAnimOn(1), layout.isAnimOn(2));

	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	switch (yesNoStage) {
		case 1: // Opening
			if (!yesNoWindow->animationActive)
				yesNoStage = 2;
			return true;
		case 2: // Opened
			if (nowPressed & WPAD_LEFT)
				yesNoWindow->current = 1;
			else if (nowPressed & WPAD_RIGHT)
				yesNoWindow->current = 0;
			else if (Wiimote_TestButtons(GetActiveWiimote(), WPAD_A | WPAD_TWO)) {
				yesNoWindow->close = true;

				if (yesNoWindow->current != 1)
					yesNoWindow->hasBG = true;
				yesNoStage = 3;

			} else { // Cancel using B or 1
				if (CheckIfMenuShouldBeCancelledForSpecifiedWiimote(0)) {
					yesNoWindow->cancelled = true;
					yesNoWindow->current = 1;
					yesNoStage = 3;
				}
			}
			return true;
		case 3: // Closing
			if (!yesNoWindow->visible) {
				if (yesNoWindow->current == 1) {
					yesNoStage = 0;
					for (int i = 0; i < 40; i++)
						if (handles[i].Exists()) {
							handles[i].Pause(false, 3);
						}
				} else {
					yesNoStage = 4;
					this->finish();
				}
			}
			return true;
		case 4: // Waiting for exit transition
			return true;
	}

	if (yesNoWindow->layoutLoaded && (nowPressed & WPAD_PLUS)) {
		yesNoStage = 1;
		yesNoWindow->type = 20;
		yesNoWindow->visible = true;

		for (int i = 0; i < 40; i++)
			if (handles[i].Exists()) {
				handles[i].Pause(true, 3);
			}
		return true;
	}

	// Let's get our shit started
	callOpeningState(this, currentStateID);

	if (!layout.isAnimOn(0) && !layout.isAnimOn(1) && !layout.isAnimOn(2))
		layout.enableNonLoopAnim(0);

	layout.execAnimations();
	layout.update();

	return true;
}

int dScOpening_c::onDraw() {
	layout.scheduleForDrawing();
	return true;
}


void dScOpening_c::loadIdlePic() {
	char leftName[32], rightName[32];
	FileHandle handle;

	sprintf(leftName, "/OpeningPics/Page%dleft.tpl.LZ", this->currentPicture);
	sprintf(rightName, "/OpeningPics/Page%dright.tpl.LZ", this->currentPicture);

	//OSReport("Getting %s and %s\n", leftName, rightName);

	// setup the left texture
	void *leftBuf = LoadFile(&picHandle, leftName);
	u32 leftBufSize = CXGetUncompressedSize(leftBuf);
	EGG__Heap__free(lastLeft, GameHeaps[2]);
	lastLeft = currLeft;
	currLeft = (TPLPalette*)EGG__Heap__alloc(leftBufSize, 0x20, GameHeaps[2]);
	CXUncompressLZ(leftBuf, currLeft);

	P_leftPage_00->material->texMaps[0].ReplaceImage(currLeft, 0);
	FreeFile(&picHandle);

	// and now the right
	void *rightBuf = LoadFile(&picHandle, rightName);
	u32 rightBufSize = CXGetUncompressedSize(rightBuf);
	EGG__Heap__free(lastRight, GameHeaps[2]);
	lastRight = currRight;
	currRight = (TPLPalette*)EGG__Heap__alloc(rightBufSize, 0x20, GameHeaps[2]);
	CXUncompressLZ(rightBuf, currRight);

	P_rightPage_00->material->texMaps[0].ReplaceImage(currRight, 0);
	FreeFile(&picHandle);
}

void dScOpening_c::loadNextPic() {
	this->currentPicture++;
	loadIdlePic();
	P_oleftPage_00->material->texMaps[0].ReplaceImage(lastLeft, 0);
	P_orightPage_00->material->texMaps[0].ReplaceImage(lastRight, 0);
	P_nleftPage_00->material->texMaps[0].ReplaceImage(currLeft, 0);
}

void dScOpening_c::loadPrevPic() {
	this->currentPicture--;
	loadIdlePic();
	P_oleftPage_00->material->texMaps[0].ReplaceImage(lastLeft, 0);
	P_orightPage_00->material->texMaps[0].ReplaceImage(lastRight, 0);
	P_nrightPage_00->material->texMaps[0].ReplaceImage(currRight, 0);
}


void dScOpening_c::finish() {
	for (int i = 0; i < 40; i++) {
		if (handles[i].Exists())
			handles[i].Stop(5);
	}

	// So quick explanation, we make the save think we saw the W1 cutscene already and then write this save to memory,
	// and then edit it to make the cutscene play but do not write it to memory this time.

	// This way, the animation plays but when exiting and coming back, it doesn't replay
	// Clever Nintendo, very clever...

	SaveFile* save = GetSaveFile();
	SaveBlock* saveBlock = save->GetBlock(-1);

	// Stolen from 80922ED0
	saveBlock->OrR5AgaginstByteFromWorldAvailableArrayAt0x32ofWorldData(0, 1);
	save->SaveCurrentData_Maybe();
	save->create_hash_for_savefile();
	save->WriteSavefileToRegularBuffer();

	// Stolen from 80922F80
	saveBlock->ClearR5FromByteInWorldAvailableArrayAt0x32ofWorldData(0, 1);

	ActivateWipe(WIPE_FADE);
	DoSceneChange(WORLD_MAP, 0x80000000, 0);
}

wchar_t *dScOpening_c::getTextForPage(int page) {
	return (wchar_t *)((u32)this->openingFile + this->openingFile->openingInfo[page].textOffset);
}



// setting up the current page
void dScOpening_c::state0_whatever() {
	if (!layout.isAnimOn(1) && !layout.isAnimOn(2)) {
		u16 musicID = this->openingFile->openingInfo[currentPicture].musicID;
		if (this->currentMusicID != musicID) {
			if (handles[musicHandleID].Exists())
				handles[musicHandleID].Stop(1);

			musicHandleID = (musicHandleID == 1) ? 2 : 1;
			PlaySoundWithFunctionB4(SoundRelatedClass, &handles[musicHandleID], musicID, 1);
			this->currentMusicID = musicID;
		}

		u16 sfxID = this->openingFile->openingInfo[currentPicture].sfxID;
		if (sfxID != 0) {
			PlaySoundWithFunctionB4(SoundRelatedClass, &handles[3], sfxID, 1);
		}

		T_DialogueTxt_00->SetString(L"\u0000");
		T_DialogueSha_00->SetString(L"\u0000");

		layout.disableAllAnimations();
		layout.enableNonLoopAnim(3);

		currentStateID++;
	}
}

// writing text
void dScOpening_c::state1_whatever() {
	if (!layout.isAnimOn(3)) {
		int nowHeld = Remocon_GetButtons(GetActiveRemocon());
		int nowPressed = Remocon_GetPressed(GetActiveRemocon());
		if (nowPressed & (WPAD_TWO | WPAD_A)) this->textTimer = 0;
		if (this->textTimer == ((nowHeld & (WPAD_TWO | WPAD_A)) ? 2 : 5)) {
			this->currentChar++;

			wchar_t chars[256];
			wchar_t *txt = getTextForPage(this->currentPicture);

			while (txt[this->currentChar-1] == ' ' || txt[this->currentChar-1] == '\n') {
				this->currentChar++;
			}

			if ((txt[this->currentChar-1] == '.' /*|| txt[this->currentChar-1] == '!'*/ || txt[this->currentChar-1] == '?') && txt[this->currentChar] != '.') 
				this->waitingTime = 50;
			else if (txt[this->currentChar-1] == ',' || txt[this->currentChar-1] == ':')
				this->waitingTime = 20;

			u32 chrCount = 0;
			u32 lineCount = 1;
			for (int i = 0; i < this->currentChar; i++) {
				chars[i] = txt[i];
				if (txt[i] == '\n') {
					lineCount++;
				}
				chrCount++;
			}

			for (int i = lineCount; i < this->openingFile->openingInfo[this->currentPicture].lineCount; i++) {
				chars[chrCount] = '\n';
				chrCount++;
			}

			chars[chrCount] = 0;

			T_DialogueTxt_00->SetString(chars);
			T_DialogueSha_00->SetString(chars);

			this->textTimer = 0;

			if (this->currentChar == this->openingFile->openingInfo[this->currentPicture].chrCount) {
				this->currentChar = 0;
				this->waitingTime = 0;
				currentStateID++;
			}
		}

		if (this->waitingTime)
			this->waitingTime--;
		else
			this->textTimer++;
	}
}

// showing 2 button
void dScOpening_c::state2_whatever() {
	if (!layout.isAnimOn(3)) {
		layout.disableAllAnimations();
		layout.enableNonLoopAnim(6);
		currentStateID++;
	}
}

// button idle activation
void dScOpening_c::state3_whatever() {
	if (!layout.isAnimOn(6)) {
		layout.disableAllAnimations();
		layout.enableNonLoopAnim(7);
		currentStateID++;
	}
}

// waiting for player input
void dScOpening_c::state4_whatever() {
	if (!layout.isAnimOn(7))
		layout.enableNonLoopAnim(7);

	int nowPressed = Remocon_GetPressed(GetActiveRemocon());
	if (nowPressed & (WPAD_TWO | WPAD_A)) {
		layout.disableAllAnimations();
		layout.enableNonLoopAnim(8);

		if (this->currentPicture < this->openingFile->pagesCount-1) {
			PlaySoundWithFunctionB4(SoundRelatedClass, &handles[0], SFX_BUTTONPRESS, 1);
			currentStateID++;
		}
		else
			currentStateID += 4;
	} else if ((nowPressed & (WPAD_B | WPAD_ONE)) && this->currentPicture > 0) {
		layout.enableNonLoopAnim(5);
		currentStateID += 3;
	}
}

// hiding the textbox
void dScOpening_c::state5_whatever() {
	if (!layout.isAnimOn(8)) {
		layout.disableAllAnimations();
		layout.enableNonLoopAnim(5);
		currentStateID++;
	}
}

// flipping to the next page
void dScOpening_c::state6_whatever() {
	if (!layout.isAnimOn(5)) {
		this->loadNextPic();
		layout.disableAllAnimations();
		layout.enableNonLoopAnim(1);
		PlaySoundWithFunctionB4(SoundRelatedClass, &handles[0], SFX_PAGEFLIP, 1);
		currentStateID = 0;
	}
}

// flipping to the previous page
void dScOpening_c::state7_whatever() {
	if (!layout.isAnimOn(5)) {
		this->loadPrevPic();
		layout.disableAllAnimations();
		layout.enableNonLoopAnim(2);
		PlaySoundWithFunctionB4(SoundRelatedClass, &handles[0], SFX_PAGEFLIP, 1);
		currentStateID = 0;
	}
}

// finishing
void dScOpening_c::state8_whatever() {
	if (!layout.isAnimOn(8) && !layout.isAnimOn(5)) {
		layout.disableAllAnimations();
		layout.enableNonLoopAnim(5);
		this->finish();
	}
}


void OpeningDrawFunc() {
	Reset3DState();

	SetupLYTDrawing();
	DrawAllLayoutsBeforeX(0x81);
	RenderEffects(0, 3);
	RenderEffects(0, 2);
	GXDrawDone();
	RemoveAllFromScnRoot();
	Reset3DState();
	SetCurrentCameraID(1);
	DoSpecialDrawing1();

	SetCurrentCameraID(0);
	for (int i = 0; i < 4; i++)
		RenderEffects(0, 0xB+i);
	for (int i = 0; i < 4; i++)
		RenderEffects(0, 7+i);
	GXDrawDone();
	DrawAllLayoutsAfterX(0x80);
	ClearLayoutDrawList();
	SetCurrentCameraID(0);
}