#ifndef J3DMODELDATA_H
#define J3DMODELDATA_H

#include "JSystem/J3DGraphAnimator/J3DJointTree.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAttach.h"
#include "JSystem/J3DGraphAnimator/J3DShapeTable.h"
#include "JSystem/J3DGraphBase/J3DSys.h"
#include "JSystem/J3DGraphBase/J3DVertex.h"

class JUTNameTab;

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DModelData {
public:
    void clear();
    J3DModelData();
    s32 newSharedDisplayList(u32);
    void indexToPtr();
    void makeSharedDL();
    void simpleCalcMaterial(u16, Mtx);
    void syncJ3DSysPointers() const;
    void syncJ3DSysFlags() const;

    virtual ~J3DModelData() {}

    void simpleCalcMaterial(Mtx mtx) { simpleCalcMaterial(0, mtx); }
    J3DMaterialTable& getMaterialTable() { return mMaterialTable; }
    JUTNameTab* getMaterialName() const { return mMaterialTable.getMaterialName(); }
    J3DVertexData& getVertexData() { return mVertexData; }
    u16 getShapeNum() const { return mShapeTable.getShapeNum(); }
    u16 getMaterialNum() const { return mMaterialTable.getMaterialNum(); }
    u16 getJointNum() const { return mJointTree.getJointNum(); }
    u16 getDrawMtxNum() const { return mJointTree.getDrawMtxNum(); }
    J3DMaterial* getMaterialNodePointer(u16 idx) const {
        return mMaterialTable.getMaterialNodePointer(idx);
    }
    u32 getVtxNum() const { return mVertexData.getVtxNum(); }
    u32 getNrmNum() const { return mVertexData.getNrmNum(); }
    u8 getDrawMtxFlag(u16 idx) const { return mJointTree.getDrawMtxFlag(idx); }
    u16 getDrawMtxIndex(u16 idx) const { return mJointTree.getDrawMtxIndex(idx); }
    J3DDrawMtxData* getDrawMtxData() { return mJointTree.getDrawMtxData(); }
    J3DShapeTable* getShapeTable() { return &mShapeTable; }
    J3DShape* getShapeNodePointer(u16 idx) const { return mShapeTable.getShapeNodePointer(idx); }
    J3DJoint* getJointNodePointer(u16 idx) const { return mJointTree.getJointNodePointer(idx); }
    J3DJointTree& getJointTree() { return mJointTree; }
    J3DModelHierarchy const* getHierarchy() { return mJointTree.getHierarchy(); }
    void setHierarchy(J3DModelHierarchy* hierarchy) { mJointTree.setHierarchy(hierarchy); }
    void setBasicMtxCalc(J3DMtxCalc* calc) { mJointTree.setBasicMtxCalc(calc); }
    JUTNameTab* getJointName() const { return mJointTree.getJointName(); }
    Mtx& getInvJointMtx(int idx) { return mJointTree.getInvJointMtx(idx); }
    J3DTexture* getTexture() const { return mMaterialTable.getTexture(); }
    JUTNameTab* getTextureName() const { return mMaterialTable.getTextureName(); }
    u16 getWEvlpMtxNum() const { return mJointTree.getWEvlpMtxNum(); }
    u16* getWEvlpMixMtxIndex() const { return mJointTree.getWEvlpMixMtxIndex(); }
    f32* getWEvlpMixWeight() const { return mJointTree.getWEvlpMixWeight(); }
    u8 getWEvlpMixMtxNum(u16 idx) const { return mJointTree.getWEvlpMixMtxNum(idx); }
    u16* getWEvlpImportantMtxIndex() const { return mJointTree.getWEvlpImportantMtxIndex(); }
    u32 getModelDataType() const { return mJointTree.getModelDataType(); }
    void setModelDataType(u32 type) { mJointTree.setModelDataType(type); }
    void* getVtxPosArray() const { return mVertexData.getVtxPosArray(); }
    void* getVtxNrmArray() const { return mVertexData.getVtxNrmArray(); }
    GXColor* getVtxColorArray(u8 idx) const { return mVertexData.getVtxColorArray(idx); }
    bool checkFlag(u32 flag) const { return (mFlags & flag) ? true : false; }
    u32 getFlag() const { return mFlags; }
    void const* getRawData() const { return mpRawData; }
    bool checkBumpFlag() const { return mbHasBumpArray == 1; }
    void setBumpFlag(u32 flag) { mbHasBumpArray = flag; }
    bool checkBBoardFlag() const { return mbHasBillboard == 1; }
    bool isLocked() { return mMaterialTable.isLocked(); }
    void entryTexMtxAnimator(J3DAnmTextureSRTKey* anm) { mMaterialTable.entryTexMtxAnimator(anm); }
    int entryTevRegAnimator(J3DAnmTevRegKey* anm) { return mMaterialTable.entryTevRegAnimator(anm); }
    void entryTexNoAnimator(J3DAnmTexPattern* anm) { mMaterialTable.entryTexNoAnimator(anm); }
    int removeTexNoAnimator(J3DAnmTexPattern* anm) {
        return mMaterialTable.removeTexNoAnimator(anm);
    }
    int removeTexMtxAnimator(J3DAnmTextureSRTKey* anm) {
        return mMaterialTable.removeTexMtxAnimator(anm);
    }
    int removeTevRegAnimator(J3DAnmTevRegKey* anm) {
        return mMaterialTable.removeTevRegAnimator(anm);
    }
    int removeMatColorAnimator(J3DAnmColor* anm) {
        return mMaterialTable.removeMatColorAnimator(anm);
    }
    void syncJ3DSys() {
        syncJ3DSysFlags();
        syncJ3DSysPointers();
    }
    void makeHierarchy(J3DJoint* joint, J3DModelHierarchy const** hierarchy) {
        mJointTree.makeHierarchy(joint, hierarchy, &mMaterialTable, &mShapeTable);
        mShapeTable.initShapeNodes(getDrawMtxData(), &getVertexData());
    }
    void show() {
        mShapeTable.show();
    }
    void hide() {
        mShapeTable.hide();
    }

private:
    friend class J3DModelLoader;

    /* 0x04 */ void const* mpRawData;
    /* 0x08 */ u32 mFlags;
    /* 0x0C */ u16 mbHasBumpArray;
    /* 0x0E */ u16 mbHasBillboard;
    /* 0x10 */ J3DJointTree mJointTree;
    /* 0x58 */ J3DMaterialTable mMaterialTable;
    /* 0x78 */ J3DShapeTable mShapeTable;
    /* 0x88 */ J3DVertexData mVertexData;
};  // Size: 0xE4

#endif /* J3DMODELDATA_H */
