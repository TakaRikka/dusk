/**
 * JUTFader.cpp
 * JUtility - Color Fader
 */

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTFader.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"

JUTFader::JUTFader(int x, int y, int width, int height, JUtility::TColor pColor)
    : mColor(pColor), mBox(x, y, x + width, y + height) {
    mStatus = 0;
    field_0x8 = 0;
    field_0xa = 0;
    field_0x24 = 0;
    mEStatus = UNKSTATUS_M1;
}

void JUTFader::control() {
    if (0 <= mEStatus && mEStatus-- == 0) {
		mStatus = field_0x24;
	}

	if (mStatus == 1) {
		return;
	}
    
	switch (mStatus) {
    case 0:
        mColor.a = 0xFF;
        break;
    case 2:
        mColor.a = 0xFF - ((++field_0xa * 0xFF) / field_0x8);

        if (field_0xa >= field_0x8) {
            mStatus = 1;
        }

        break;
    case 3:
        mColor.a = ((++field_0xa * 0xFF) / field_0x8);

        if (field_0xa >= field_0x8) {
            mStatus = 0;
        }

        break;
	}
	draw();
}

void JUTFader::draw() {
    if (mColor.a == 0) {
		return;
	}

	J2DOrthoGraph orthograph;
	orthograph.setColor(mColor);
	orthograph.fillBox(mBox);
}

bool JUTFader::startFadeIn(int param_0) {
    bool statusCheck = mStatus == 0;

    if (statusCheck) {
        mStatus = 2;
        field_0xa = 0;
        field_0x8 = param_0;
    }

    return statusCheck;
}

bool JUTFader::startFadeOut(int param_0) {
    bool statusCheck = mStatus == 1;

    if (statusCheck) {
        mStatus = 3;
        field_0xa = 0;
        field_0x8 = param_0;
    }

    return statusCheck;
}

void JUTFader::setStatus(JUTFader::EStatus i_status, int param_1) {
    switch (i_status) {
    case 0: 
        if (param_1 != 0) {
            field_0x24 = 0;
            mEStatus = param_1;
            break;
        }

        mStatus = 0;
        field_0x24 = 0;
        mEStatus = 0;
        break;
    case 1: 
        if (param_1 != 0) {
            field_0x24 = 1;
            mEStatus = param_1;
            break;
        }

        mStatus = 1;
        field_0x24 = 1;
        mEStatus = 0;
        break;
    }
}

JUTFader::~JUTFader() {}
