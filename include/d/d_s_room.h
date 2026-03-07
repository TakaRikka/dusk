#ifndef D_S_D_S_ROOM_H
#define D_S_D_S_ROOM_H

#include "f_op/f_op_scene_mng.h"

class dStage_roomDt_c;

class room_of_scene_class : public scene_class {
public:
    /* 0x1C4 */ request_of_phase_process_class phase;
    /* 0x1CC */ void* roomInfo;
    /* 0x1D0 */ dStage_roomDt_c* roomDt;
    /* 0x1D4 */ s8 field_0x1d4;
    /* 0x1D5 */ u8 field_0x1d5;
    /* 0x1D6 */ u8 unk_0x1d6[0x1D8 - 0x1D6];
    /* 0x1D8 */ u8 field_0x1d8;
};

#endif /* D_S_D_S_ROOM_H */
