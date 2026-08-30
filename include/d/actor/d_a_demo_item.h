#ifndef D_A_DEMO_ITEM_H
#define D_A_DEMO_ITEM_H

#include "f_op/f_op_actor_mng.h"
#include "d/actor/d_a_itembase.h"
#include "d/d_particle.h"

/**
 * @ingroup actors-unsorted
 * @class daDitem_c
 * @brief Cutscene Item
 *
 * @details
 *
 */
class daDitem_c : public daItemBase_c {
public:
    enum Action_e {
        ACTION_START_e,
        ACTION_EVENT_e,
        ACTION_WAIT_LIGHT_END_e,
        ACTION_END_e,
    };

    int CreateInit();
    void action();
    void actionStart();
    void actionEvent();
    void actionWaitLightEnd();
    void actionEnd();
    void setInsectEffect();
    void followInsectEffect();
    void endInsectEffect();
    void onEventReg(int, int);
    void set_pos();
    void anim_control();
    void initEffectLight();
    void settingEffectLight();
    void set_mtx();
    void draw_WOOD_STICK();

    virtual void setListStart();
    virtual void setTevStr();
    virtual int __CreateHeap();

    inline int Delete();
    inline int create();
    inline int execute();
    inline int draw();

    void setAction(u8 i_action) { mAction = i_action; }

    u8 chkArgFlag(u8 i_flag) { return field_0x93d & i_flag; }

    void setOffsetPos(cXyz i_pos) { mOffsetPos = i_pos; }
    void setMaxScale(f32 i_scale) { mMaxScale = i_scale; }

private:
    /* 0x92C */ cXyz mOffsetPos;
    /* 0x938 */ f32 mMaxScale;
    /* 0x93C */ u8 field_0x93c;
    /* 0x93D */ u8 field_0x93d;
    /* 0x93E */ u8 field_0x93e;
    /* 0x93F */ u8 mSetLightEff;
    /* 0x940 */ f32 mLightStrength;
    /* 0x944 */ u8 field_0x944[0x948 - 0x944];
    /* 0x948 */ LIGHT_INFLUENCE mLight;
    /* 0x968 */ u8 mAction;
    /* 0x969 */ u8 field_0x969;
    /* 0x96A */ u8 mParticleAlpha;
    /* 0x96C */ dPa_followEcallBack field_0x96c;
    /* 0x980 */ dPa_followEcallBack field_0x980;
    /* 0x994 */ JPABaseEmitter* field_0x994;
    /* 0x998 */ JPABaseEmitter* field_0x998;
    /* 0x99C */ cXyz field_0x99c;
    /* 0x9A8 */ Z2SoundObjSimple mSound;
};

STATIC_ASSERT(sizeof(daDitem_c) == 0x9c8);

namespace daDitem_prm {
inline u8 getFlag(daDitem_c* i_this) {
    return (fopAcM_GetParam(i_this) >> 0x10) & 0xFF;
}

inline u8 getNo(daDitem_c* i_this) {
    return fopAcM_GetParam(i_this) & 0xFF;
}
};

#endif /* D_A_DEMO_ITEM_H */
