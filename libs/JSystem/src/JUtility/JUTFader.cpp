/**
 * JUTFader.cpp
 * JUtility - Color Fader
 */

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTFader.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"

#ifdef TARGET_PC
#include <algorithm>
#endif

JUTFader::JUTFader(int x, int y, int width, int height, JUtility::TColor pColor)
    : mColor(pColor), mBox(x, y, x + width, y + height) {
    mStatus = None;
    mDuration = 0;
    mTimer = 0;
    mNextStatus = 0;
    mStatusTimer = -1;
}

void JUTFader::advance() {
    if (0 <= mStatusTimer && mStatusTimer-- == 0) {
        mStatus = mNextStatus;
    }

    if (mStatus == Wait) {
        return;
    }

    switch (mStatus) {
    case None:
        mColor.a = 0xFF;
        break;
    case FadeIn:
#if AVOID_UB
        if (mDuration == 0) {
            mStatus = Wait;
            break;
        }
#endif
        mColor.a = 0xFF - ((++mTimer * 0xFF) / mDuration);

        if (mTimer >= mDuration) {
            mStatus = Wait;
        }

        break;
    case FadeOut:
#if AVOID_UB
        if (mDuration == 0) {
            mStatus = None;
            break;
        }
#endif
        mColor.a = ((++mTimer * 0xFF) / mDuration);

        if (mTimer >= mDuration) {
            mStatus = None;
        }

        break;
    }
}

void JUTFader::control() {
    advance();
    draw();
}

void JUTFader::draw() {
    if (mColor.a != 0) {
#ifdef TARGET_PC
        if (dusk::frame_interp::is_enabled() && mDuration != 0) {
            const auto step = dusk::frame_interp::get_interpolation_step();
            const auto progress = static_cast<f32>(mTimer) / static_cast<f32>(mDuration);
            const auto timer = mTimer - 1 + step + progress;
            auto alpha = timer / mDuration;
            if (mStatus == FadeIn) {
                alpha = 1.0f - alpha;
            }
            alpha = std::clamp(alpha, 0.0f, 1.0f);
            mColor.a = static_cast<u8>(alpha * 255.0f);
        }
#endif
        J2DOrthoGraph orthograph;
        orthograph.setColor(mColor);
        orthograph.fillBox(mBox);
    }
}

bool JUTFader::startFadeIn(int duration) {
    bool statusCheck = mStatus == 0;

    if (statusCheck) {
        mStatus = FadeIn;
        mTimer = 0;
        mDuration = duration;
    }

    return statusCheck;
}

bool JUTFader::startFadeOut(int duration) {
    bool statusCheck = mStatus == 1;

    if (statusCheck) {
        mStatus = FadeOut;
        mTimer = 0;
        mDuration = duration;
    }

    return statusCheck;
}

void JUTFader::setStatus(JUTFader::EStatus i_status, int timer) {
    switch (i_status) {
    case None: 
        if (timer != 0) {
            mNextStatus = None;
            mStatusTimer = timer;
            break;
        }

        mStatus = None;
        mNextStatus = None;
        mStatusTimer = 0;
        break;
    case Wait:
        if (timer != 0) {
            mNextStatus = Wait;
            mStatusTimer = timer;
            break;
        }

        mStatus = Wait;
        mNextStatus = Wait;
        mStatusTimer = 0;
        break;
    }
}

JUTFader::~JUTFader() {}
