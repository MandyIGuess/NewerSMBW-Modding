#include <game.h>
#include <sfx.h>

class dSelectPlayer_c : public dBase_c {
public:
    dStateWrapper_c<dSelectPlayer_c> state;
    m2d::EmbedLayout_c layout;

    nw4r::lyt::Picture
        *P_1_00, *P_2_01, *P_3_01, *P_4_01,
        *P_2_02, *P_3_02, *P_4_02;
    
    nw4r::lyt::Pane *N_playerNum_00, *N_playerMulti_00;

    bool layoutLoaded;
    u8 doActuallyRun;
    u8 startShowing;
    u8 field_26B;
    int currentChoice;
    int whatAnimationToExitWith; // this also decides the *behavior*, not just the animation
    int field_274;
    int field_278;
    int choiceOnBottomRow;

    void newButtonFunctionality(); // new function

    USING_STATES(dSelectPlayer_c);
    REF_NINTENDO_STATE(ExitAnimeEndWait);
    REF_NINTENDO_STATE(StartMemberButtonAnime);
    REF_NINTENDO_STATE(ButtonChangeAnimeEndWait);
};

// only effects main game buttons
void dSelectPlayer_c::newButtonFunctionality() {
    int nowPressed = Remocon_GetPressed(GetActiveRemocon());

    // return to file selection
    if (nowPressed & WPAD_ONE) {
        nw4r::snd::SoundHandle handle;
        PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_SYS_BACK, 0);
        whatAnimationToExitWith = 0xffffffff; // behavior: returns to file select
        state.setState(&StateID_ExitAnimeEndWait);
    }

    // press our current button
    if (nowPressed & WPAD_TWO) {
        whatAnimationToExitWith = 1;

        // play mario's sound on the 1 button
        if (currentChoice == 0) {
            nw4r::snd::SoundHandle handle;
            PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_VOC_MA_PLAYER_DECIDE, 0);
        }
        state.setState(&StateID_StartMemberButtonAnime);
    }

    // go to prior button (unless we're on 1P)
    if (currentChoice != 0) {
        if (nowPressed & WPAD_LEFT) {
            currentChoice--;
            state.setState(&StateID_ButtonChangeAnimeEndWait);
        }
    }

    // go to next button (unless we're on 4P)
    if (currentChoice != 3) {
        if (nowPressed & WPAD_RIGHT) {
            currentChoice++;
            state.setState(&StateID_ButtonChangeAnimeEndWait);
        }
    }
}