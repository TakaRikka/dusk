#ifndef HOMEBUTTON_FRAME_CONTROLLER_H
#define HOMEBUTTON_FRAME_CONTROLLER_H

#include <revolution/types.h>

namespace homebutton {

    enum {
        /* 0 */ ANIM_TYPE_FORWARD = 0,
        /* 1 */ ANIM_TYPE_BACKWARD,
        /* 2 */ ANIM_TYPE_LOOP,
        /* 3 */ ANIM_TYPE_ALTERNATE
    };

    enum {
        /* 0 */ ANIM_STATE_STOP = 0,
        /* 1 */ ANIM_STATE_PLAY,
        /* 2 */ ANIM_STATE_STOP_REQ,
    };

    class FrameController {
    public:
        FrameController() {}

        /* 0x08 */ virtual ~FrameController() {}
        /* 0x0C */ virtual void calc();

        void init(int type, f32 maxFrame, f32 minFrame, f32 delta);
        void initFrame();

        void setMaxFrame(f32 value) { mMaxFrame = value; }
        f32 getMaxFrame() const { return mMaxFrame; }

        f32 getLastFrame() const { return mMaxFrame - 1.0f; }

        void setMinFrame(f32 value) { mMinFrame = value; }
        f32 getMinFrame() const { return mMinFrame; }

        void setCurrentFrame(f32 value) { mFrame = value; }
        f32 getCurrentFrame() const { return mFrame; }

        void setDelta(f32 value) { mDelta = value; }
        f32 getDelta() const { return mDelta; }

        void setState(int value) { mState = value; }
        int getState() const { return mState; }

        void setAnimType(int value) { mAnmType = value; }
        int getAnimType() const { return mAnmType; }

        bool isPlaying() const { return mState == ANIM_STATE_PLAY; }

        void start() {
            initFrame();
            restart();
        }

        void restart() { mState = ANIM_STATE_PLAY; }
        void stop() { mState = ANIM_STATE_STOP; }

    protected:
        /* 0x00 (vtable) */
        /* 0x04 */ f32 mMaxFrame;
        /* 0x08 */ f32 mMinFrame;
        /* 0x0C */ f32 mFrame;
        /* 0x10 */ f32 mDelta;
        /* 0x14 */ int mState;
        /* 0x18 */ int mAnmType;

    private:
        /* 0x1C */ bool mbAlternateBack;
    };

}  // namespace homebutton

#endif
