#ifndef JSUIOSBASE_H_
#define JSUIOSBASE_H_

#include <dolphin/types.h>

enum JSUStreamSeekFrom {
    JSUStreamSeekFrom_SET = 0,  // absolute
    JSUStreamSeekFrom_CUR = 1,  // relative
    JSUStreamSeekFrom_END = 2,  // relative to end
};

/**
* @ingroup jsystem-jsupport
* 
*/
class JSUIosBase {
public:
    enum EIoState {
        IOS_STATE_1 = 1,
        IOS_STATE_2 = 2,
    };

    JSUIosBase() { mState = false; }

    virtual ~JSUIosBase() {}

    bool isGood() const { return mState == 0; }
    void clrState(EIoState state) { mState &= ~state; }
    void setState(EIoState state) { mState |= state; }

private:
    bool mState;
};  // Size = 0x8

#endif
