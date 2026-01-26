#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bg_parts.h"
#include "d/d_s_play.h"
#include "d/d_camera.h"
#include "f_op/f_op_camera_mng.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRSolidHeap.h"

// NONMATCHING
void dBgp_c::material_c::draw() {
    material_c* material;
    do {
        drawSimple();
    } while ((material = getNext()) != NULL);
}

// NONMATCHING - regalloc
void dBgp_c::modelMaterial_c::drawSimple() {
    if (mpMaterial->getInvalid()) {
        OSReport_Warning("Share Material Nothing !!\n");
        return;
    }

    mpMaterial->loadSharedDL();
    mpModelData->syncJ3DSysFlags();
    j3dSys.setTexture(mpModelData->getTexture());

    J3DShape* shape = mpMaterial->getShape();
    shape->loadCurrentMtx();

    modelMaterial_c* modelMaterial = this;
    int spC = 0;
    void* vatCmd = NULL;

    if (getMaterial()->getShape()->checkFlag(1)) {
        modelMaterial = modelMaterial->getChild();
    }

    do {
        Mtx m;
        J3DShape* shape = modelMaterial->getMaterial()->getShape();
        J3DModelData* modelData = modelMaterial->getModelData();

        j3dSys.setVtxPos(modelData->getVtxPosArray());
        j3dSys.setVtxNrm(modelData->getVtxNrmArray());
        j3dSys.setVtxCol(modelData->getVtxColorArray(0));
        MTXConcat(j3dSys.getViewMtx(), modelMaterial->getMtx(), m);
        GXLoadPosMtxImm(m, 0);
        GXLoadNrmMtxImm(m, 0);

        if (vatCmd != shape->getVcdVatCmd()) {
            GXCallDisplayList(shape->getVcdVatCmd(), 0xC0);
            vatCmd = shape->getVcdVatCmd();
        }

        shape->loadVtxArray();

        u16 mtxNum = shape->getMtxGroupNum();
        for (u16 i = 0; i < mtxNum; i++) {
            if (shape->getShapeDraw(i) != NULL) {
                shape->getShapeDraw(i)->draw();
            }
        }

        modelMaterial = modelMaterial->getChild();
    } while (modelMaterial != NULL);
}

bool dBgp_c::modelMaterial_c::isSame(dBgp_c::material_c* i_material) {
    dBgp_c::modelMaterial_c* modelMaterial = (dBgp_c::modelMaterial_c*)i_material;
    return modelMaterial->getMaterialID() == mMaterialID;
}

void dBgp_c::modelMaterial_c::addChild(dBgp_c::material_c* i_material) {
    dBgp_c::modelMaterial_c* modelMaterial = (dBgp_c::modelMaterial_c*)i_material;
    modelMaterial->setChild(mpChild);
    mpChild = modelMaterial;
}

// NONMATCHING
void dBgp_c::modelMaterial_c::set(J3DModelData* i_modelData, J3DMaterial* i_material, Mtx i_mtx) {
    mpModelData = i_modelData;
    mpMaterial = i_material;
    mpMtx = i_mtx;
    mpChild = NULL;

    setNext(NULL);

    JUTNameTab* nametab = i_modelData->getMaterialName();
    u16* name = (u16*)nametab->getName(i_material->getIndex());
    mMaterialID = *name;
    JUT_ASSERT(157, mMaterialID != static_cast<u16>(~0));
    JUT_ASSERT(159, mMaterialID < (1<<(16-packet_c::MATERIAL_SORT_LEVEL)));

    const char* sortName = (const char*)name;
    ASSERTLINE(166, sortName[2] == 'X' || sortName[2] == '0' || sortName[2] == '1' || sortName[2] == '2' || sortName[2] == '3');

    u16 sp12 = 0;
    if (sortName[2] != 'X') {
        sp12 = (sortName[2] - 0x30) * 0x10;
    }

    mMaterialID = (mMaterialID & ~0x30) | ((mMaterialID & 0x30) << 10) | sp12;
}

void dBgp_c::model_c::create(J3DModelData* i_modelData, Mtx i_mtx) {
    const void* binary = i_modelData->getBinary();
    
    mId = *(u32*)((char*)binary + 0x1C);
    if (mId != 0xFFFF) {
        addShare(mId);
    }

    JUT_ASSERT(205, i_modelData->getJointNum() == 1);
    JUT_ASSERT(206, i_modelData->getMaterialNum() != 0);

    mMaterial = new modelMaterial_c[i_modelData->getMaterialNum()];
    JUT_ASSERT(212, mMaterial != NULL);

    J3DJoint* joint = i_modelData->getJointNodePointer(0);    
    cMtx_copy(i_mtx, mBaseTRMtx);

    J3DMaterial* material;
    for (material = joint->getMesh(); material != NULL; material = material->getNext()) {
        mMaterial[material->getIndex()].set(i_modelData, material, mBaseTRMtx);
    }
}

void dBgp_c::model_c::remove() {
    if (mMaterial != NULL) {
        cutShare(mId);
    }
}

void dBgp_c::packet_c::draw() {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    s8 roomNo = (s8)getRoomNo();
    dKy_tevstr_c* roomTevstr = dComIfGp_roomControl_getTevStr(roomNo);

    g_env_light.settingTevStruct(0x40, NULL, roomTevstr);
    dBgp_c::entryShare(this);

    GXColor ambcol;
    stage_pure_lightvec_info_class* lightvecInf = dComIfGp_roomControl_getStatusRoomDt(roomNo)->getLightVecInfo();
    dKy_setLight_nowroom(roomNo);

    ambcol.r = roomTevstr->AmbCol.r;
    ambcol.g = roomTevstr->AmbCol.g;
    ambcol.b = roomTevstr->AmbCol.b;
    ambcol.a = roomTevstr->AmbCol.a;
    GXSetChanAmbColor(GX_COLOR0A0, ambcol);
    GXSetChanMatColor(GX_COLOR0A0, g_whiteColor);

    dKy_setLight_again();
    dKy_GxFog_tevstr_set(roomTevstr);
    J3DShape::resetVcdVatCache();

    material_c** material = mpMaterials;
    for (int i = 64; i > 0; i--) {
        if (*material != NULL) {
            (*material)->draw();
        }
        material++;
    }
}

void dBgp_c::packet_c::reset() {
    material_c** material = mpMaterials;
    for (int i = 64; i > 0; i--) {
        *material = NULL;
        material++;
    }
}

void dBgp_c::packet_c::entryMaterial(dBgp_c::material_c* i_material, u32 i_id) {
    i_material->setNext(mpMaterials[i_id]);
    mpMaterials[i_id] = i_material;
}

void dBgp_c::packet_c::entryModel(dBgp_c::modelMaterial_c* i_modelMaterial) {
    u16 id = i_modelMaterial->getMaterialID() & 0x3F;
    if (mpMaterials[id] != NULL) {
        material_c* material = mpMaterials[id];
        do {
            if (material->isSame(i_modelMaterial)) {
                material->addChild(i_modelMaterial);
                return;
            }

            material = material->getNext();
        } while (material != NULL);
    }

    entryMaterial(i_modelMaterial, id);
    i_modelMaterial->setChild(NULL);
}

void dBgp_c::packet_c::entryShare(dBgp_c::modelMaterial_c* i_modelMaterial) {
    u16 id = i_modelMaterial->getMaterialID() & 0x3F;
    if (mpMaterials[id] != NULL) {
        material_c** ppmaterial = &mpMaterials[id];
        modelMaterial_c* modelMaterial = (modelMaterial_c*)*ppmaterial;
        do {
            if (modelMaterial->isSame(i_modelMaterial)) {
                i_modelMaterial->setChild(modelMaterial);
                i_modelMaterial->setNext(modelMaterial->getNext());
                modelMaterial->setNext(NULL);
                *ppmaterial = i_modelMaterial;
                return;
            }

            ppmaterial = modelMaterial->getNext_p();
            modelMaterial = (modelMaterial_c*)*ppmaterial;
        } while (modelMaterial != NULL);
    }
}

dBgp_c::share_c::share_c() {
    mStatus = -1;
    mCount = 0;
}

JKRSolidHeap* dBgp_c::mShareHeap;
dBgp_c::share_c* dBgp_c::mShare;

void dBgp_c::share_c::set(u16 i_id) {
    JUT_ASSERT(441, mCount != static_cast<u16>(~0));

    if (mCount++ == 0) {
        mStatus = 1;
        mId = i_id;
        
        int rt = dComIfG_setObjectRes(getArcName(), 0, (JKRHeap*)NULL);
        JUT_ASSERT(446, rt != 0);
    }
}

void dBgp_c::share_c::reset() {
    JUT_ASSERT(460, mCount);

    if (--mCount == 0) {
        for (u16 i = 0; i < mModelData->getMaterialNum(); i++) {
            J3DMaterial* material = mModelData->getMaterialNodePointer(i);
            if (material->getMaterialAnm() != NULL) {
                delete material->getMaterialAnm();
            }
        }

        delete[] mMaterial;

        dComIfG_deleteObjectResMain(getArcName());
        mStatus = -1;
    }
}

const char* dBgp_c::share_c::getArcName() {
    static char arcName[8];
    sprintf(arcName, "@mt%04x", mId);
    return arcName;
}

static BOOL createMatAnm(J3DModelData* i_modelData, u16 i_materialId) {
    if (i_materialId != 0xFFFF) {
        J3DMaterial* material = i_modelData->getMaterialNodePointer(i_materialId);
        if (material->getMaterialAnm() == NULL) {
            J3DMaterialAnm* anm = new J3DMaterialAnm();
            if (anm == NULL) {
                return FALSE;
            }

            material->setMaterialAnm(anm);
        }
    }

    return TRUE;
}

int dBgp_c::share_c::execute() {
    if (mStatus != 0) {
        mStatus = dComIfG_syncObjectRes(getArcName());
        JUT_ASSERT(527, mStatus >= 0);
        if (mStatus > 0) {
            return 0;
        }

        mModelData = (J3DModelData*)dComIfG_getObjectRes(getArcName(), "model.bmdl");
        JUT_ASSERT(542, mModelData != NULL);

        JUT_ASSERT(544, mModelData->getMaterialNum() != 0);

        mMaterial = new modelMaterial_c[mModelData->getMaterialNum()];
        JUT_ASSERT(546, mMaterial != NULL);

        for (u16 i = 0; i < mModelData->getMaterialNum(); i++) {
            J3DMaterial* material = mModelData->getMaterialNodePointer(i);
            mMaterial[i].set(mModelData, material, cMtx_getIdentity());
            
            J3DShape* shape = material->getShape();
            shape->hide();

            JUTNameTab* nametab = mModelData->getMaterialName();
            char* name = (char*)nametab->getName(material->getIndex());
            name[0] = ' ';
            name[1] = ' ';
        }

        field_0x1 = 0;

        J3DAnmTextureSRTKey* btk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(getArcName(), "model.btk");
        if (btk != NULL) {
            int rt = mBtk.init(mModelData, btk, TRUE, 2, 1.0f, 0, -1);
            JUT_ASSERT(568, rt);

            for (u16 i = 0; i < btk->getUpdateMaterialNum(); i++) {
                rt = createMatAnm(mModelData, btk->getUpdateMaterialID(i));
                JUT_ASSERT(571, rt);
            }

            mBtk.entry(mModelData);
            field_0x1 |= (u8)1;
        }

        J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(getArcName(), "model.brk");
        if (brk != NULL) {
            int rt = mBrk.init(mModelData, brk, TRUE, 2, 1.0f, 0, -1);
            JUT_ASSERT(582, rt);

            for (u16 i = 0; i < brk->getCRegUpdateMaterialNum(); i++) {
                rt = createMatAnm(mModelData, brk->getCRegUpdateMaterialID(i));
                JUT_ASSERT(585, rt);
            }

            for (u16 i = 0; i < brk->getKRegUpdateMaterialNum(); i++) {
                rt = createMatAnm(mModelData, brk->getKRegUpdateMaterialID(i));
                JUT_ASSERT(589, rt);
            }

            mBrk.entry(mModelData);
            field_0x1 |= (u8)2;
        }

        if (mId == 0x8000) {
            setCallback(&share_c::cbConveyor);
        } else {
            setCallback(&share_c::cbNormal);
        }
    }

    (this->*mCallback)();
    return 1;
}

void dBgp_c::share_c::draw() {
    if (mStatus == 0 && field_0x1 != 0) {
        mModelData->simpleCalcMaterial((MtxP)j3dDefaultMtx);
        j3dSys.setTexture(mModelData->getTexture());

        for (u16 i = 0; i < mModelData->getMaterialNum(); i++) {
            J3DMaterial* material = mModelData->getMaterialNodePointer(i);
            J3DDisplayListObj* dlobj = material->getSharedDisplayListObj();
            dlobj->beginDL();

            if (field_0x1 & 1) {
                material->getTexGenBlock()->patch();
            }

            if (field_0x1 & 2) {
                material->getTevBlock()->patch();
            }

            dlobj->endPatch();
        }
    }
}

void dBgp_c::share_c::entry(dBgp_c::packet_c* i_packet) {
    if (mStatus == 0) {
        modelMaterial_c* modelMaterial = mMaterial;
        for (int i = modelMaterial->getModelData()->getMaterialNum() - 1; i >= 0; i--) {
            i_packet->entryShare(modelMaterial);
            modelMaterial++;
        }
    }
}

void dBgp_c::share_c::cbNormal() {
    if (field_0x1 & 1) {
        mBtk.play();
        mBtk.entryFrame();
    }

    if (field_0x1 & 2) {
        mBrk.play();
        mBrk.entryFrame();
    }
}

void dBgp_c::share_c::cbConveyor() {
    if (IREG_S(0) == 0) {
        if (mBtk.getFrame() > 30.0f) {
            mBtk.setEndFrame(80.0f);
        } else {
            mBtk.setEndFrame(30.0f);
        }

        mBtk.setLoopFrame(0.0f);
    } else {
        mBtk.setEndFrame(70.0f);
        mBtk.setLoopFrame(40.0f);
    }

    mBtk.play();
    mBtk.entryFrame();
}

dBgp_c::dBgp_c() {
    mPointer = NULL;
    mHeap = NULL;
    mHeap2 = NULL;
    mBgWork = NULL;
    mModel = NULL;
}

dBgp_c::packet_c::~packet_c() {}

dBgp_c::packet_c::packet_c() {
    reset();
}

void dBgp_c::create(s8 i_roomNo, void* i_data) {
    mPointer = i_data;
    mPacket.setRoomNo(i_roomNo);
    strcpy(mArcName, dComIfG_getRoomArcName(i_roomNo));

    if (mPointer != NULL) {
        JKRExpHeap* block = dStage_roomControl_c::getMemoryBlock(i_roomNo);
        JKRExpHeap* parentHeap;
        if (block != NULL) {
            parentHeap = block;
        } else {
            parentHeap = mDoExt_getGameHeap();
        }

        mHeap = JKRCreateSolidHeap(0xFFFFFFFF, parentHeap, false);
        JUT_ASSERT(882, mHeap != NULL);

        JKRHeap* prevHeap = mDoExt_setCurrentHeap(mHeap);

        mModel = new model_c[((stage_map_unit_class*)mPointer)->num];
        JUT_ASSERT(886, mModel != NULL);

        unit_class* mapUnit = (unit_class*)((stage_map_unit_class*)mPointer)->entries;
        mBgWork = new dBgW[mapUnit->num];
        JUT_ASSERT(890, mBgWork != NULL);

        dBgW* bgw = mBgWork;
        unit_group_class* unitGroup = mapUnit->groups;
        for (int i = 0; i < mapUnit->num; i++) {
            char resName[16];
            sprintf(resName, "bp%04d.dzb", i);

            cBgD_t* dzb = (cBgD_t*)dComIfG_getStageRes(mArcName, resName);
            if (dzb != NULL) {
                bool rt;
                if (unitGroup->switchNo == 0xFF) {
                    rt = bgw->Set(dzb, 0x20, NULL);
                } else {
                    rt = bgw->Set(dzb, 1, (Mtx*)cMtx_getIdentity());
                }
                JUT_ASSERT(911, !rt);
                
                bgw->SetPriority(dBgW::PRIORITY_0);
                bgw->SetRoomId(mPacket.getRoomNo());
            }

            bgw++;
            unitGroup++;
        }

        unit_unk_data2_class* unitData = ((stage_map_unit_class*)mPointer)->entries->unk_0xc;
        for (int i = 0; i < ((stage_map_unit_class*)mPointer)->num; i++) {
            u16 id = getId(unitData->field_0x0);
            int rt = dComIfG_setObjectRes(getArcName(unitData->field_0x0), 0, (JKRHeap*)NULL);
            JUT_ASSERT(927, rt);
            unitData++;
        }

        field_0x130 = 0;
        mDoExt_setCurrentHeap(prevHeap);
        u32 heapSize = mHeap->adjustSize();
        printf("Room%02d Unit Memory <%d>\n", mPacket.getRoomNo(), heapSize);
    }
}

int dBgp_c::remove() {
    if (mPointer != NULL) {
        if (!executeShare()) {
            return 0;
        }

        if (mHeap != NULL) {
            if (field_0x130 < ((stage_map_unit_class*)mPointer)->num) {
                execute(true);
                return 0;
            }

            unit_class* mapUnit = (unit_class*)((stage_map_unit_class*)mPointer)->entries;
            for (int i = 0; i < mapUnit->num; i++) {
                releaseBg(i);
            }

            unit_unk_data2_class* unitData = mapUnit->unk_0xc;
            model_c* model = mModel;
            for (int i = 0; i < ((stage_map_unit_class*)mPointer)->num; i++) {
                model->remove();
                dComIfG_deleteObjectResMain(getArcName(unitData->field_0x0));

                unitData++;
                model++;
            }

            mDoExt_destroySolidHeap(mHeap);
            mHeap = NULL;
        }

        if (mHeap2 != NULL) {
            mDoExt_destroySolidHeap(mHeap2);
            mHeap2 = NULL;
        }
    }

    return 1;
}

const char* dBgp_c::getArcName(u16 i_id, u16 i_arg) {
    static char arcName[8];
    sprintf(arcName, "@%03x%03x", i_id, i_arg);
    return arcName;
}

BOOL dBgp_c::isSwitch(dBgp_c::unit_group_class* i_group) {
    if (i_group->switchNo == 0xFF) {
        return false;
    }

    if (!(i_group->unk_0x0 & 0x80)) {
        return dComIfGs_isSwitch(i_group->switchNo, mPacket.getRoomNo());
    }

    return !dComIfGs_isSwitch(i_group->switchNo, mPacket.getRoomNo());
}

void dBgp_c::registBg(fopAc_ac_c* i_actor) {
    if (mPointer != NULL) {
        unit_class* mapUnit = (unit_class*)((stage_map_unit_class*)mPointer)->entries;
        unit_group_class* unitGroup = mapUnit->groups;
        for (int i = 0; i < mapUnit->num; i++) {
            registBg(i, i_actor);
        }
    }
}

void dBgp_c::releaseBg() {
    if (mPointer != NULL) {
        unit_class* mapUnit = (unit_class*)((stage_map_unit_class*)mPointer)->entries;
        unit_group_class* unitGroup = mapUnit->groups;
        for (int i = 0; i < mapUnit->num; i++) {
            releaseBg(i);
        }
    }
}

int dBgp_c::registBg(int i_no, fopAc_ac_c* i_actor) {
    if (!mBgWork[i_no].ChkUsed() && mBgWork[i_no].GetBgd() != NULL) {
        if (dComIfG_Bgsp().Regist(&mBgWork[i_no], i_actor)) {
            return 0;
        }
    }

    return 1;
}

void dBgp_c::releaseBg(int i_no) {
    if (mBgWork[i_no].ChkUsed()) {
        dComIfG_Bgsp().Release(&mBgWork[i_no]);
    }
}

int dBgp_c::execute(bool param_0) {
    if (mPointer != NULL) {
        if (field_0x130 < ((stage_map_unit_class*)mPointer)->num) {
            unit_unk_data2_class* unitData = &((stage_map_unit_class*)mPointer)->entries->unk_0xc[field_0x130];
            do {
                u16 id = getId(unitData->field_0x0);
                u16 arg = getArg0(unitData->field_0x0);
                const char* arcName = getArcName(id, arg);
                int rt = dComIfG_syncObjectRes(arcName);
                JUT_ASSERT(1117, rt >= 0);
                if (rt > 0) {
                    return 0;
                }

                unitData++;
            } while (++field_0x130 < ((stage_map_unit_class*)mPointer)->num);

            if (param_0) {
                return 1;
            }

            s8 roomNo = mPacket.getRoomNo();
            JKRExpHeap* block = dStage_roomControl_c::getMemoryBlock(roomNo);
            JKRExpHeap* parentHeap;
            if (block != NULL) {
                parentHeap = block;
            } else {
                parentHeap = mDoExt_getGameHeap();
            }

            mHeap2 = JKRCreateSolidHeap(0xFFFFFFFF, parentHeap, false);
            JUT_ASSERT(1145, mHeap2 != NULL);

            JKRHeap* prevHeap = mDoExt_setCurrentHeap(mHeap2);

            unit_class* mapUnit = ((stage_map_unit_class*)mPointer)->entries;
            model_c* model = mModel;
            unitData = mapUnit->unk_0xc;
            for (int i = 0; i < ((stage_map_unit_class*)mPointer)->num; i++) {
                J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(getArcName(unitData->field_0x0), "model.bmdl");
                if (modelData != NULL) {
                    mDoMtx_stack_c::transS(unitData->pos.x, unitData->pos.y, unitData->pos.z);
                    mDoMtx_stack_c::YrotM(unitData->angle);
                    model->create(modelData, mDoMtx_stack_c::get());
                }

                unitData++;
                model++;
            }

            mDoExt_setCurrentHeap(prevHeap);
            u32 heapSize = mHeap2->adjustSize();
            printf("Room%02d Unit Memory <%d>\n", mPacket.getRoomNo(), heapSize);
        }

        if (!param_0) {
            unit_unk_data2_class* unitData = ((stage_map_unit_class*)mPointer)->entries->unk_0xc;
            for (int i = 0; i < ((stage_map_unit_class*)mPointer)->num; i++) {
                u16 id = getId(unitData->field_0x0);
                u16 arg = getArg0(unitData->field_0x0);
                const char* arcName = getArcName(id, arg);
                if (dComIfG_getObjectResInfo(arcName)->getResNum() != 6) {
                    OS_REPORT("特殊オブジェクト<%s>セット！！\n", arcName);
                    cXyz pos(unitData->pos);
                    csXyz angle(0, unitData->angle, 0);
                    cXyz size(1.0f, 1.0f, 1.0f);

                    u32 parameters = id << 12 | arg;
                    parameters |= unitData->field_0x15 << 21;
                    parameters |= 0x80000000;

                    angle.x = (unitData->field_0x14 << 8) | unitData->field_0x5;
                    angle.z = (unitData->field_0x4 << 8) | unitData->field_0x3;

                    fopAcM_create(PROC_SET_BG_OBJ, parameters, &pos, mPacket.getRoomNo(), &angle, &size, -1);
                }
                unitData++;
            }
        }
    }

    return 1;
}

void dBgp_c::draw(fopAc_ac_c* i_actor) {
    if (mPointer == NULL || field_0x130 != ((stage_map_unit_class*)mPointer)->num) {
        return;
    }

    mPacket.reset();
    mDoLib_clipper::changeFar(100000.0f);

    camera_class* sp30 = dComIfGp_getCamera(0);
    dCamera_c* camera = &sp30->mCamera;
    JUT_ASSERT(1287, camera != NULL)

    static u16 l_dispBitTable[] = {
        0x4000,
        0xC000,
        0x8000,
        0x9000,
        0x1000,
        0x3000,
        0x2000,
        0x6000,
    };

    u16 spE = 0;
    if (camera->HideBGPartsOk()) {
        u16 spC = dCam_getAngleY(sp30) + 0x1000;
        spE = l_dispBitTable[spC >> 13];
    }

    unit_class* mapUnit = ((stage_map_unit_class*)mPointer)->entries;
    unit_group_class* unitGroup = mapUnit->groups;

    for (int i = 0; i < mapUnit->num; i++) {
        if (isSwitch(unitGroup)) {
            releaseBg(i);
        } else {
            registBg(i, i_actor);
            
            unit_unk_data_class* unitData = &mapUnit->unk_0x8[unitGroup->unk_0x2];
            for (int j = (unitGroup->unk_0x0 & ~0x80); j > 0; j--) {
                if ((unitData->field_0x0 & spE) == 0) {
                    model_c* model = &mModel[unitData->field_0x2];
                    for (int k = (unitData->field_0x0 & ~0xF000); k > 0; k--) {
                        modelMaterial_c* modelMaterial = model->getMaterial();
                        if (modelMaterial != NULL) {
                            J3DModelData* modelData = modelMaterial->getModelData();
                            J3DJoint* joint = modelData->getJointNodePointer(0);

                            Mtx m;
                            cMtx_concat(j3dSys.getViewMtx(), model->getBaseTRMtx(), m);

                            if (!mDoLib_clipper::clip(m, joint->getMax(), joint->getMin())) {
                                entryModel(model);
                            }
                        }

                        model++;
                    }
                }

                unitData++;
            }
        }

        unitGroup++;
    }

    mDoLib_clipper::resetFar();
    entryPacket();
}

void dBgp_c::setPointer(void* i_pointer) {
    stage_map_unit_class* var_r29 = (stage_map_unit_class*)i_pointer;
    if (var_r29 != NULL) {
        unit_class* mapUnit = var_r29->entries;
        if ((uintptr_t)mapUnit->groups < 0x80000000) {
            mapUnit->groups = (unit_group_class*)((uintptr_t)mapUnit + (uintptr_t)mapUnit->groups);
            mapUnit->unk_0x8 = (unit_unk_data_class*)((uintptr_t)mapUnit + (uintptr_t)mapUnit->unk_0x8);
            mapUnit->unk_0xc = (unit_unk_data2_class*)((uintptr_t)mapUnit + (uintptr_t)mapUnit->unk_0xc);
        }
    }
}

void dBgp_c::createShare() {
    if (mShareHeap == NULL) {
        mShareHeap = mDoExt_createSolidHeapFromGameToCurrent(0, 0x20);
        JUT_ASSERT(1409, mShareHeap != NULL);

        mShare = new share_c[16];
        JUT_ASSERT(1411, mShare != NULL);

        u32 heapSize = mDoExt_adjustSolidHeapToSystem(mShareHeap);
        printf("Share Memory <%d>\n", heapSize);
    }
}

void dBgp_c::removeShare() {
    if (mShareHeap != NULL) {
        cutShare(0);
        mDoExt_destroySolidHeap(mShareHeap);
        mShareHeap = NULL;
    }
}

void dBgp_c::addShare(u16 i_id) {
    JUT_ASSERT(1455, mShare != NULL);

    share_c* share = mShare;
    share_c* freeShare = NULL;

    for (int i = 0; i < 16; i++) {
        if (share->getCount() == 0) {
            freeShare = share;
        } else if (i_id == share->getId()) {
            share->set(i_id);
            return;
        }

        share++;
    }

    JUT_ASSERT(1469, freeShare != NULL);
    freeShare->set(i_id);
}

void dBgp_c::cutShare(u16 i_id) {
    share_c* share = mShare;
    for (int i = 0; i < 16; i++) {
        if (share->getCount() != 0 && i_id == share->getId()) {
            share->reset();
        }

        share++;
    }
}

bool dBgp_c::executeShare() {
    bool rt = true;
    share_c* share = mShare;
    for (int i = 0; i < 16; i++) {
        if (share->isUsed() && !share->execute()) {
            rt = false;
        }

        share++;
    }

    return rt;
}

void dBgp_c::drawShare() {
    share_c* share = mShare;
    for (int i = 0; i < 16; i++) {
        if (share->isDraw()) {
            share->draw();
        }

        share++;
    }
}

void dBgp_c::entryShare(dBgp_c::packet_c* i_packet) {
    share_c* share = mShare;
    for (int i = 0; i < 16; i++) {
        share->entry(i_packet);
        share++;
    }
}
