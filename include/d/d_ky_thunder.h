#ifndef D_D_KY_THUNDER_H
#define D_D_KY_THUNDER_H

#include "f_op/f_op_kankyo.h"
#include "m_Do/m_Do_ext.h"

class JKRSolidHeap;

class dThunder_modelInfo_c {
public:
    /* 0x00 */ J3DModel* mpModel;
    /* 0x04 */ mDoExt_invisibleModel mInvisModel;
    /* 0x0C */ mDoExt_brkAnm mBrk;
    /* 0x24 */ mDoExt_btpAnm mBtp;
};  // Size: 0x3C

class dThunder_c : public kankyo_class {
public:
    int createHeap();
    void adjustHeap();
    int create();
    inline int draw();
    inline int execute(dThunder_c* i_thunder);
    inline int thunder_delete(dThunder_c* i_thunder);

    ~dThunder_c() { mDoExt_destroySolidHeap(mpHeap); }

private:
    /* 0x0F8 */ JKRSolidHeap* mpHeap;
    /* 0x0FC */ dThunder_modelInfo_c mModelInfo;
    /* 0x138 */ cXyz field_0x138;
    /* 0x144 */ cXyz mThunderPos;
    /* 0x150 */ cXyz field_0x150;
    /* 0x15C */ f32 field_0x15c;
    /* 0x160 */ u8 unk_0x160[4];
    /* 0x164 */ f32 mBrkSpeed;
    /* 0x168 */ s16 field_0x168;
    /* 0x16A */ s16 field_0x16a;
    /* 0x16C */ s16 field_0x16c;
};

#endif /* D_D_KY_THUNDER_H */
