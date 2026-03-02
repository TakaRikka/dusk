
#include "HBMFrameController.h"

namespace homebutton {

    void FrameController::init(int anm_type, f32 max_frame, f32 min_frame, f32 delta) {
        mAnmType = anm_type;
        mMaxFrame = max_frame;
        mMinFrame = min_frame;

        mDelta = delta;
        mState = ANIM_STATE_STOP;
        mbAlternateBack = false;

        initFrame();
    }

    void FrameController::initFrame() {
        mFrame = mAnmType == ANIM_TYPE_BACKWARD ? mMaxFrame : mMinFrame;
    }

    void FrameController::calc() {
        if (mState != ANIM_STATE_PLAY) {
            return;
        }

        switch (mAnmType) {
        case ANIM_TYPE_FORWARD:
            if ((mFrame += mDelta) >= getLastFrame()) {
                mFrame = getLastFrame();
                stop();
            }

            break;

        case ANIM_TYPE_BACKWARD:
            if ((mFrame -= mDelta) <= mMinFrame) {
                mFrame = mMinFrame;
                stop();
            }

            break;

        case ANIM_TYPE_LOOP:
            if ((mFrame += mDelta) >= mMaxFrame) {
                mFrame -= mMaxFrame - mMinFrame;
            }

            break;

        case ANIM_TYPE_ALTERNATE:
            if (!mbAlternateBack) {
                if ((mFrame += mDelta) >= getLastFrame()) {
                    mFrame = getLastFrame();
                    mbAlternateBack = true;
                }
            } else {
                if ((mFrame -= mDelta) <= mMinFrame) {
                    mFrame = mMinFrame;
                    mbAlternateBack = false;
                }
            }

            break;
        }
    }

}  // namespace homebutton
