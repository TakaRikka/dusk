#ifndef D_A_TAG_SPPATH_H
#define D_A_TAG_SPPATH_H

#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_path.h"

class daTagSppath_c : public fopAc_ac_c {
public:
    int create();
    ~daTagSppath_c();
    int getNearPathPos(cXyz*, dPath*);
    int execute();

    dPath* getKeepPath() { return mpBestPath; }
    u16 getKeepPointNum() const { return mBestNo; }
    bool checkDirectionFront() const { return field_0x6e8 == 1; }
    bool checkDirectionNone() const { return field_0x6e8 == 0; }

    /* 0x568 */ dPath* mpInitPath;
    /* 0x56C */ dPath* mpBestPath;
    /* 0x570 */ dCcD_Cyl mCyl;
    /* 0x6AC */ dCcD_Stts mStts;
    /* 0x6E8 */ u8 field_0x6e8;
    /* 0x6E9 */ u8 mSwNo1;
    /* 0x6EA */ u8 mSwNo2;
    /* 0x6EC */ u16 mBestNo;
    /* 0x6F0 */ f32 mHeight;
};

#endif /* D_A_TAG_SPPATH_H */
