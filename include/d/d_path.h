#ifndef D_D_PATH_H
#define D_D_PATH_H

#include "dolphin/types.h"
#include "dolphin/mtx.h"

class cBgS_PolyInfo;
struct cXyz;

struct dPnt {
    /* 0x0 */ u8 mArg1;
    /* 0x1 */ u8 mArg2;
    /* 0x2 */ u8 mArg3;
    /* 0x3 */ u8 mArg0;
    /* 0x4 */ Vec m_position;
};  // Size: 0x10

struct dPath {
    /* 0x0 */ u16 m_num;
    /* 0x2 */ u16 m_nextID;
    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ bool m_closed;
    /* 0x6 */ u8 field_0x6;
    /* 0x7 */ u8 field_0x7;
    /* 0x8 */ dPnt* m_points;
};

inline BOOL dPath_ChkClose(const dPath* i_path) { return i_path->m_closed & 1; }

dPath* dPath_GetRoomPath(int path_index, int room_no);
dPath* dPath_GetNextRoomPath(dPath const* i_path, int room_no);
dPnt* dPath_GetPnt(dPath const* i_path, int pnt_index);
#if !PLATFORM_GCN  // Fakematch due to differing return type on non-GCN platforms
int dPath_GetPolyRoomPathVec(cBgS_PolyInfo const& poly, cXyz* p_pathVec, int* param_2);
#else
u8 dPath_GetPolyRoomPathVec(cBgS_PolyInfo const& poly, cXyz* p_pathVec, int* param_2);
#endif

#endif /* D_D_PATH_H */
