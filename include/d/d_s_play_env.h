#include "JSystem/JHostIO/JORReflexible.h"
#include "SSystem/SComponent/c_xyz.h"

class dScnPly_env_otherHIO_c : public JORReflexible {
public:
    dScnPly_env_otherHIO_c();

    virtual ~dScnPly_env_otherHIO_c() {}
    void genMessage(JORMContext*);

    #if DEBUG
    /* 0x04 */ s8 field_0x04;
    #endif
    
    /* 0x08 */ f32 mShadowDensity;

    #if DEBUG
    /* 0x0C */ f32 mCullFarValue;
    /* 0x10 */ f32 mRailSize;
    /* 0x14 */ s16 mRailColorR;
    /* 0x16 */ s16 mRailColorG;
    /* 0x18 */ s16 mRailColorB;
    /* 0x1A */ s16 field_0x1a;
    /* 0x1C */ s16 field_0x1c[20];
    #endif

    /* 0x44 */ u8 mAdjustLODBias;
    /* 0x45 */ u8 field_0x45;
    /* 0x46 */ u8 mDisplayTransparentCyl;

    #if DEBUG
    /* 0x47 */ u8 mDisplayShadows;
    /* 0x48 */ u8 mDisplayShadowImage;
    /* 0x49 */ u8 mDisplayShadowPoly;
    /* 0x4A */ u8 mDisplayCullBox;
    /* 0x4B */ u8 mLoadAllRooms;
    /* 0x4C */ u8 mDepthOfField;
    /* 0x4D */ u8 mDisplayParticleInfo;
    /* 0x4E */ u8 field_0x4e;
    /* 0x4F */ u8 field_0x4f;
    /* 0x50 */ u8 mAdjustCullFar;
    #endif
};

class dScnPly_env_debugHIO_c : public JORReflexible {
public:
    dScnPly_env_debugHIO_c();

    virtual ~dScnPly_env_debugHIO_c() {}
    void genMessage(JORMContext*);

    /* 0x04 */ u8 field_0x4;
    /* 0x08 */ cXyz mBoxCullMinSize;
    /* 0x14 */ cXyz mBoxCullMaxSize;
    /* 0x20 */ cXyz mSphereCullCenter;
    /* 0x2C */ f32 mSphereCullRadius;
};
