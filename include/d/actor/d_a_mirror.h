#ifndef D_A_MIRROR_H
#define D_A_MIRROR_H

#include "f_op/f_op_actor_mng.h"

class dMirror_packet_c : public J3DPacket {
public:
    dMirror_packet_c();
    void reset();
    void calcMinMax();
    int entryModel(J3DModel*);
    void mirrorZdraw(f32*, f32*, f32, f32, f32, f32, f32, f32);
    void modelDraw(J3DModel*, f32 (*)[4]);
    void mainDraw();
    virtual void draw();
    virtual ~dMirror_packet_c();
    cXyz& getViewScale() { return mViewScale; }
    GXTexObj& getTexObj() { return mTexObj; }
    cXyz* getQuad() { return mQuad; }

    /* 0x010 */ GXTexObj mTexObj;
    /* 0x030 */ u8 mModelCount;
    /* 0x034 */ J3DModel* mModels[0x40];
    /* 0x134 */ cXyz mQuad[4];
    /* 0x164 */ cXyz mMinVal;
    /* 0x170 */ cXyz mMaxVal;
    /* 0x17C */ cXyz mViewScale;
};

/**
 * @ingroup actors-unsorted
 * @class daMirror_c
 * @brief Mirror
 *
 * @details Used in Link's House? Or Blizzeta's Room? Or Twilight Mirror?
 *
 */
class daMirror_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    daMirror_c();
    BOOL createHeap();
    void setModelMtx();
    inline int create();
    int execute();
    int entryModel(J3DModel*);
    static int entry(J3DModel*);
    static bool remove();

    inline int Delete();
    inline int draw();

    s32 getType() { return fopAcM_GetParamBit(this, 0, 8); }
    s32 getSw() { return fopAcM_GetParamBit(this, 8, 8); }
    static u32 getMirrorRoomPrm() { return 0xFF03; }

    typedef int (daMirror_c::*entryModelFunc)(J3DModel*);
    static entryModelFunc m_entryModel;
    static daMirror_c* m_myObj;

    /* 0x570 */ dMirror_packet_c mPacket;
    /* 0x6f8 */ J3DModel* mpModel;
};


#endif /* D_A_MIRROR_H */
