#include "CaptureScreen.h"
#include "global.h"

CaptureScreen::CaptureScreen(const JFWDisplay* pDisplay) {
    mpDisplay = pDisplay;
    field_0x4 = 0;
}

int CaptureScreen::getAlignedWidthBytes(int param_0) {
    return ROUND((param_0 * 4) - param_0, 4);
}

int CaptureScreen::getBmpBufferSize(int param_0, int param_1) {
    return param_1 * getAlignedWidthBytes(param_0) + 0x36;
}

void CaptureScreen::makeBmpFromEfb(void* param_0, int param_1, int param_2, int param_3, int param_4, int param_5, int param_6) {
    // NONMATCHING
}

void CaptureScreen::openBmp(int param_0, int param_1) {
    // NONMATCHING
}

void CaptureScreen::closeBmp() {
    // NONMATCHING
}

void CaptureScreen::saveBmp(void* param_0, u32 param_1) {
    // NONMATCHING
}
