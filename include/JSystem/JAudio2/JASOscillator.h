#ifndef JASOSCILLATOR_H
#define JASOSCILLATOR_H

#include "dolphin/types.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASOscillator {
	struct Point {
		/* 0x0 */ s16 _0;
		/* 0x2 */ s16 _2;
		/* 0x4 */ s16 _4;
	};

    struct EffectParams {
		EffectParams() {
			mVolume = 1.0f;
			mPitch = 1.0f;
			mPan = 0.5f;
			mFxMix = 0.0f;
			mDolby = 0.0f;
			_14 = 1.0f;
			_18 = 1.0f;
		}

		/* 0x00 */ f32 mVolume;
		/* 0x04 */ f32 mPitch;
		/* 0x08 */ f32 mPan;
		/* 0x0C */ f32 mFxMix;
		/* 0x10 */ f32 mDolby;
		/* 0x14 */ f32 _14;
		/* 0x18 */ f32 _18;
	};

	struct Data {
		/* 0x00 */ u32 mTarget;
		/* 0x04 */ f32 _04;
		/* 0x08 */ const Point* mTable;
		/* 0x0C */ const Point* rel_table;
		/* 0x10 */ f32 mScale;
		/* 0x14 */ f32 _14;
	};

	enum Target {
		/* 0 */ TARGET_VOLUME,
		/* 1 */ TARGET_PITCH,
		/* 2 */ TARGET_PAN,
		/* 3 */ TARGET_FXMIX,
		/* 4 */ TARGET_DOLBY,
		/* 5 */ TARGET_5,
		/* 6 */ TARGET_6,
	};

	JASOscillator();

	void initStart(const Data*);
	void incCounter(f32 param_0);
	f32 getValue() const;
	void release();
    void update();
    void updateCurrentValue(f32 param_0);

    void setDirectRelease(u16 param_0) { mDirectRelease = param_0; }
    void stop() { _1C = 0; }
	bool isValid() const { return mData != NULL; }
	bool isStop() const { return _1C == 0; }
	bool isRelease() const { return _1C == 3 || _1C == 4; }
	u32 getTarget() const { return mData->mTarget; }

	/* 0x00 */ const Data* mData;
	/* 0x04 */ f32 _04;
	/* 0x08 */ f32 _08;
	/* 0x0C */ f32 _0C;
	/* 0x10 */ f32 _10;
	/* 0x14 */ u16 _14;
	/* 0x16 */ u16 mDirectRelease;
	/* 0x18 */ u8 _18;
	/* 0x1A */ u16 _1A;
	/* 0x1C */ int _1C;

    static const f32 sCurveTableLinear[17];
    static const f32 sCurveTableSampleCell[17];
	static const f32 sCurveTableSqRoot[17];
	static const f32 sCurveTableSquare[17];
};

#endif /* JASOSCILLATOR_H */
