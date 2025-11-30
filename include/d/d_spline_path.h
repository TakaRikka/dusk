#ifndef D_D_SPLINE_PATH_H
#define D_D_SPLINE_PATH_H

#include "SSystem/SComponent/c_xyz.h"

struct dPath;

struct dPathCurve {
    void Init(dPath*);
    cXyz bSpline2(f32);

    /* 0x00 */ int field_0x00;
    /* 0x00 */ int field_0x04;
    /* 0x08 */ dPath* mpPath;
};

class d2DBSplinePath {
public:
    /* 0x00 */ s32 field_0x00;
    /* 0x04 */ s32 field_0x04;
    /* 0x08 */ s32 field_0x08;
    /* 0x0C */ s32 field_0x0c;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 mCurveWeight[3];
    /* 0x20 */ int mCurveKey[3];
    /* 0x2C */ int field_0x2c;

    d2DBSplinePath() { Init(0, 0); }
    void Init(s32, s32);
    bool Step();
    f32 Calc(f32*);
    cXyz Calc(cXyz*);
    f32 Spot(f32*, f32);
    virtual ~d2DBSplinePath() {}

    bool Last() { return field_0x08 == 3; }
};

#endif /* D_D_SPLINE_PATH_H */
