#ifndef D_D_ENVSE_H
#define D_D_ENVSE_H

#include "f_op/f_op_kankyo.h"

class dStage_SoundInfo_c;

class dEnvSe_c : public kankyo_class {
public:
    int execute_common(dStage_SoundInfo_c*, s8*, u8);
    int execute();

    /* 0x0F8 */ u8 field_0xf8[0x0FC - 0x0F8];
    /* 0x0FC */ s8 field_0xfc;
    /* 0x0FD */ s8 field_0xfd;
    /* 0x0FE */ s8 field_0xfe;
    /* 0x0FF */ s8 field_0xff;
    /* 0x100 */ u8 field_0x100[0x104 - 0x100];
    /* 0x104 */ int mRoomNo;
    /* 0x108 */ int field_0x108;
};

#endif /* D_D_ENVSE_H */
