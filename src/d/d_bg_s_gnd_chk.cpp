/**
 * d_bg_s_gnd_chk.cpp
 * BG Collision Ground Check
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bg_s_gnd_chk.h"

dBgS_GndChk::dBgS_GndChk() {
    SetPolyPassChk(GetPolyPassChkInfo());
    SetGrpPassChk(GetGrpPassChkInfo());
}

dBgS_GndChk::~dBgS_GndChk() {}

dBgS_ObjGndChk_Wtr::dBgS_ObjGndChk_Wtr() {
    OffNormalGrp();
    OnWaterGrp();
}

dBgS_ObjGndChk_Wtr::~dBgS_ObjGndChk_Wtr() {}

dBgS_ObjGndChk_Spl::dBgS_ObjGndChk_Spl() {
    OffNormalGrp();
    OnSpl();
}

dBgS_ObjGndChk_Spl::~dBgS_ObjGndChk_Spl() {}

dBgS_ObjGndChk_All::~dBgS_ObjGndChk_All() {}

dBgS_CamGndChk::~dBgS_CamGndChk() {}

dBgS_CamGndChk_Wtr::dBgS_CamGndChk_Wtr() {
    OffNormalGrp();
    OnWaterGrp();
}

dBgS_CamGndChk_Wtr::~dBgS_CamGndChk_Wtr() {}
