#ifndef J3DSHAPEMTX_H
#define J3DSHAPEMTX_H

#include "JSystem/J3DGraphBase/J3DShape.h"
#include "JSystem/J3DAssert.h"
#include "dolphin/mtx.h"

class J3DTexMtx;
class J3DTexGenBlock;

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DTexMtxObj {
public:
    Mtx& getMtx(u16 idx) {
        J3D_ASSERT_RANGE(275, idx < mTexMtxNum);
        return mpTexMtx[idx];
    }

    void setMtx(u16 idx, const Mtx mtx) {
        J3D_ASSERT_RANGE(288, idx < mTexMtxNum);
        MTXCopy(mtx, mpTexMtx[idx]);
    }

    Mtx44& getEffectMtx(u16 idx) {
        J3D_ASSERT_RANGE(293, idx < mTexMtxNum);
        return mpEffectMtx[idx];
    }

    u16 getNumTexMtx() const { return mTexMtxNum; }

    /* 0x00 */ Mtx* mpTexMtx;
    /* 0x04 */ Mtx44* mpEffectMtx;
    /* 0x08 */ u16 mTexMtxNum;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DDifferedTexMtx {
public:
    static void loadExecute(f32 const (*)[4]);

    static inline void load(const Mtx m) {
        if (sTexGenBlock != NULL)
            loadExecute(m);
    }

    static J3DTexGenBlock* sTexGenBlock;
    static J3DTexMtxObj* sTexMtxObj;
};

class J3DShapeMtxConcatView;
typedef void (J3DShapeMtxConcatView::*J3DShapeMtxConcatView_LoadFunc)(int, u16) const;

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DShapeMtxConcatView : public J3DShapeMtx {
public:
    J3DShapeMtxConcatView(u16 useMtxIndex)
        : J3DShapeMtx(useMtxIndex)
    {}

    virtual ~J3DShapeMtxConcatView() {}
    virtual u32 getType() const { return 'SMCV'; }
    virtual void load() const;
    virtual void loadNrmMtx(int, u16) const {}
    virtual void loadNrmMtx(int, u16, f32 (*)[4]) const;

    void loadMtxConcatView_PNGP(int, u16) const;
    void loadMtxConcatView_PCPU(int, u16) const;
    void loadMtxConcatView_NCPU(int, u16) const;
    void loadMtxConcatView_PNCPU(int, u16) const;
    void loadMtxConcatView_PNGP_LOD(int, u16) const;

    static J3DShapeMtxConcatView_LoadFunc sMtxLoadPipeline[4];
    static J3DShapeMtxConcatView_LoadFunc sMtxLoadLODPipeline[4];
    static MtxP sMtxPtrTbl[2];
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DShapeMtxYBBoardConcatView : public J3DShapeMtxConcatView {
public:
    J3DShapeMtxYBBoardConcatView(u16 useMtxIndex)
        : J3DShapeMtxConcatView(useMtxIndex)
    {}

    virtual ~J3DShapeMtxYBBoardConcatView() {}
    virtual u32 getType() const { return 'SMYB'; }
    virtual void load() const;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DShapeMtxBBoardConcatView : public J3DShapeMtxConcatView {
public:
    J3DShapeMtxBBoardConcatView(u16 useMtxIndex)
        : J3DShapeMtxConcatView(useMtxIndex)
    {}

    virtual ~J3DShapeMtxBBoardConcatView() {}
    virtual u32 getType() const { return 'SMBB'; }
    virtual void load() const;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DShapeMtxMulti : public J3DShapeMtx {
public:
    J3DShapeMtxMulti(u16 useMtxIndex, u16 useMtxNum, u16* useMtxIndexTable)
        : J3DShapeMtx(useMtxIndex)
        , mUseMtxNum(useMtxNum)
        , mUseMtxIndexTable(useMtxIndexTable)
    {}

    virtual ~J3DShapeMtxMulti() {}
    virtual u32 getType() const { return 'SMML'; }
    virtual u16 getUseMtxNum() const { return mUseMtxNum; }
    virtual u16 getUseMtxIndex(u16 no) const { return mUseMtxIndexTable[no]; }
    virtual void load() const;
    virtual void calcNBTScale(Vec const&, f32 (*)[3][3], f32 (*)[3][3]);

private:
    /* 0x8 */ u16 mUseMtxNum;
    /* 0xC */ u16* mUseMtxIndexTable;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DShapeMtxMultiConcatView : public J3DShapeMtxConcatView {
public:
    J3DShapeMtxMultiConcatView(u16 useMtxIndex, u16 useMtxNum, u16* useMtxIndexTable)
        : J3DShapeMtxConcatView(useMtxIndex)
        , mUseMtxNum(useMtxNum)
        , mUseMtxIndexTable(useMtxIndexTable)
    {}

    virtual ~J3DShapeMtxMultiConcatView() {}
    virtual u32 getType() const { return 'SMMC'; }
    virtual u16 getUseMtxNum() const { return mUseMtxNum; }
    virtual u16 getUseMtxIndex(u16 no) const { return mUseMtxIndexTable[no]; }
    virtual void load() const;
    virtual void loadNrmMtx(int, u16) const {}
    virtual void loadNrmMtx(int, u16, f32 (*)[4]) const;

private:
    /* 0x8 */ u16 mUseMtxNum;
    /* 0xC */ u16* mUseMtxIndexTable;
};

#endif /* J3DSHAPEMTX_H */
