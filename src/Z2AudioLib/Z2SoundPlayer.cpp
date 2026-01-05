#include "Z2AudioLib/Z2SoundPlayer.h"
#include "Z2AudioLib/Z2SeqMgr.h"

Z2SoundPlayer::Z2SoundPlayer(const char* param_1) : JAWWindow("Player for JAudio2", 500, 140), field_0x48c(field_0x46c, 8) {
    field_0x3ed = 0;
    field_0x3ee = 0;
    field_0x3ef = 0;
    field_0x3f0 = 0;
    field_0x3f4 = param_1;
    field_0x3f8 = 0;
    field_0x3fc = 0;
    field_0x3fa = 0;
    field_0x3fe = 0;
    field_0x400 = 0;
    field_0x402 = 0;
    field_0x444 = 0;
    field_0x446 = 0;
    field_0x404 = "CMD_PORT";
    field_0x408 = "END_PORT";
    field_0x40c = "STA_PORT";
    field_0x410 = "WAIT_PORT";
    field_0x414 = "NUM_PORT";
    field_0x418 = "PORT_5";
    field_0x41c = "MAP_PORT";
    field_0x420 = "NOTE_PORT";
    field_0x424 = "SE_SELECT_PORT";
    field_0x428 = "BGM_STATUS_PORT";
    field_0x42c = "BGM_PORT2";
    field_0x430 = "BGM_PORT3";
    field_0x434 = "PORT12";
    field_0x438 = "PORT13";
    field_0x43c = "FILTER_PORT";
    field_0x440 = "FX_PORT";
    u8 r30 = 0;
    field_0x448[r30++] = &field_0x3f8;
    field_0x448[r30++] = &field_0x3fa;
    field_0x448[r30++] = &field_0x3fc;
    field_0x448[r30++] = &field_0x3fe;
    field_0x448[r30++] = &field_0x400;
    field_0x448[r30++] = &field_0x402;
    field_0x448[r30] = &field_0x444;
    field_0x464 = 0;
    field_0x468 = 7;
    field_0x4a0 = 0;
    field_0x4a4 = 1.0f;
    field_0x4a8 = 0.5f;
    field_0x4ac = 1.0f;
    field_0x4b0 = 0.0f;
    field_0x4b4 = 0.0f;
    field_0x4b8 = 1.0f;
    field_0x498 = Z2GetSeqMgr()->getSubBgmHandle();
}

void Z2SoundPlayer::onDraw(JAWGraphContext*) {

}

void Z2SoundPlayer::onTrigA(const JUTGamePad&) {

}

void Z2SoundPlayer::onTrigB(const JUTGamePad&) {

}

void Z2SoundPlayer::frameWork() {

}

void Z2SoundPlayer::onPadProc(const JUTGamePad&) {

}

void Z2SoundPlayer::onTrigX(const JUTGamePad&) {

}

void Z2SoundPlayer::onTrigY(const JUTGamePad&) {


}
void Z2SoundPlayer::onTrigZ(const JUTGamePad&) {


}
void Z2SoundPlayer::onTrigL(const JUTGamePad&) {

}

void Z2SoundPlayer::onKeyLeft(const JUTGamePad&) {

}

void Z2SoundPlayer::onKeyRight(const JUTGamePad&) {

}

void Z2SoundPlayer::onKeyUp(const JUTGamePad&) {

}

void Z2SoundPlayer::onKeyDown(const JUTGamePad&) {

}

void Z2SoundPlayer::onKeyMenu(const JUTGamePad&) {

}

u32 Z2SoundPlayer::getCursorMoveMax(const JUTGamePad&) {

}
