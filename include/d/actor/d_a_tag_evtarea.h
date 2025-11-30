#ifndef D_A_TAG_EVTAREA_H
#define D_A_TAG_EVTAREA_H

#include "f_op/f_op_actor_mng.h"

class daTag_EvtArea_c : public fopAc_ac_c {
public:
    cPhs__Step create();
    BOOL Delete();
    BOOL Execute();
    BOOL Draw();
    BOOL isDelete();
    BOOL chkPointInArea(cXyz, cXyz);

    BOOL chkPointInArea(cXyz param_0) {
        cXyz local_28(0.0f, 0.0f, 0.0f);
        return chkPointInArea(param_0, local_28);
    }

    virtual ~daTag_EvtArea_c() {}

    int getType() {
        u16 type = shape_angle.z & 0xFF;
        return type == 0xFF ? 0 : type;
    }

    s32 getNo() {
        u16 no = (shape_angle.z & 0xff00) >> 8;
        return no == 0xff ? 0 : no;
    }

    u32 getOnEvtBit() {
        u32 ret = fopAcM_GetParam(this) & 0xfff;
        return ret == 0xfff ? -1 : ret;
    }

    u32 getOffEvtBit() {
        u32 ret = (fopAcM_GetParam(this) & 0xfff000) >> 12;
        return ret == 0xfff ? -1 : ret;
    }

    u8 getBitSW() { return home.angle.x & 0xff; }

    u8 getBitSW2() { return (home.angle.x & 0xff00) >> 8; }

    void noEffect() { field_0x56c = 1; }

private:
    /* 0x568 vtable */
    /* 0x56C */ u8 field_0x56c;
};

STATIC_ASSERT(sizeof(daTag_EvtArea_c) == 0x570);

#endif /* D_A_TAG_EVTAREA_H */
