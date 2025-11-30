#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_simple_model.h"
#include "d/d_com_inf_game.h"

static void dSmplMdl_modelUpdateDL(J3DModel* i_model) {
    JUT_ASSERT(61, i_model != NULL);
    J3DModelData* modelData = i_model->getModelData();
    if (modelData->getMaterialNodePointer(0)->getMaterialAnm() != NULL) {
        OS_REPORT(
            "\x1b[43;30mシンプルモデル描画：モデルは\'bmdg\'フォルダに置いていますか？\n\x1b[m");
    }
    i_model->calc();
    i_model->entry();
    i_model->viewCalc();
}

dSmplMdl_draw_c::dSmplMdl_draw_c() {
    for (int i = 0; i < 8; i++) {
        mModel[i].init();
    }

    for (int i = 0; i < 500; i++) {
        mList[i].init();
    }

    mListCount = 0;
}

diff_model_c::diff_model_c() {
    mpHeap = NULL;
    mpModel = NULL;
    mCreateNum = 0;
    mpList = NULL;
    mRoomNo = 63;
}

dSmplMdl_draw_c::~dSmplMdl_draw_c() {
    for (int i = 0; i < 8; i++) {
        if (mModel[i].getModel() != NULL) {
            OS_REPORT("\x1b[43;30m差分ＤＬ用ダミーモデル削除忘れがあります！！\n");
            OS_REPORT("差分ＤＬモデル領域を強制的に削除します。\n", "\x1b[m");
            mModel[i].remove(1);
        }
    }
}

void dSmplMdl_draw_c::draw() {
    for (int i = 0; i < 8; i++) {
        if (mModel[i].getModel() != NULL) {
            mModel[i].draw();
        }
    }
    mListCount = 0;

    for (int i = 0; i < 500; i++) {
        mList[i].init();
    }

    for (int i = 0; i < 8; i++) {
        mModel[i].insert(NULL);
    }
}

void dSmplMdl_draw_c::entry(J3DModel* i_model, int roomNo) {
    if (i_model != NULL && mListCount < 500) {
        mList[mListCount].set(i_model);
        for (int i = 0; i < 8; i++) {
            if (mModel[i].isSame(i_model->getModelData(), roomNo)) {
                mModel[i].insert(&mList[mListCount]);
                break;
            }
        }
        mListCount++;
    } else {
        OS_REPORT("\x1b[43;30mシンプルモデル描画エントリ数オーバー！！(%s, %d)\n\x1b[m",
                  __FILE__, 278);
    }
}

BOOL dSmplMdl_draw_c::addModel(J3DModelData* i_modelData, int roomNo, u8 drawBG) {
    JUT_ASSERT(307, i_modelData != NULL);
    if (roomNo == -1) {
        OS_REPORT("\x1b[43;30mシンプルモデル描画差分ＤＬモデル登録：部屋番号指定がありません！！！\n\x1b[m");
    }
    for (int i = 0; i < 8; i++) {
        if (mModel[i].isSame(i_modelData, roomNo)) {
            if (mModel[i].create(i_modelData, roomNo, drawBG)) {
                return TRUE;
            }
            return FALSE;
        }
    }

    for (int i = 0; i < 8; i++) {
        if (mModel[i].getModelData() == NULL) {
            if (mModel[i].create(i_modelData, roomNo, drawBG)) {
                return TRUE;
            }
            return FALSE;
        }
    }

    OS_REPORT(
        "\x1b[43;30m差分ＤＬ用ダミーモデル空きエントリが見つかりませんでした！(%s, %d)\n\x1b[m",
        __FILE__, 349);

    return 0;
}

int dSmplMdl_draw_c::removeModel(J3DModelData* i_modelData, int roomNo) {
    if (i_modelData == NULL) {
        return 0;
    }

    for (int i = 0; i < 8; i++) {
        if (mModel[i].isSame(i_modelData, roomNo)) {
            mModel[i].remove(0);
            return 0;
        }
    }
    
    OS_REPORT_ERROR("シンプルモデルを登録してないのに削除に！\n");
    return 0;
}

BOOL diff_model_c::create(J3DModelData* i_modelData, int roomNo, u8 drawBG) {
    JUT_ASSERT(397, i_modelData != NULL);
    if (mpHeap == NULL) {
        mpHeap = mDoExt_createSolidHeapFromGameToCurrent(0x2000, 0x20);

        if (mpHeap != NULL) {
            mpModel = mDoExt_J3DModel__create(i_modelData, 0x80000, 0x11000084);

            if (mpModel == NULL) {
                OS_REPORT("差分ＤＬ用モデル モデル生成失敗！(%s, %d)\n", __FILE__, 413);
                remove(1);
                mDoExt_restoreCurrentHeap();
            } else {
                cXyz scale(0.0f, 0.0f, 0.0f);
                mpModel->setBaseScale(scale);
                mpModel->setBaseTRMtx(mDoMtx_getIdentity());
                mCreateNum++;
                int result = mDoExt_adjustSolidHeapToSystem(mpHeap);
                mRoomNo = roomNo;
                mDrawBG = drawBG;
                #if VERSION == VERSION_SHIELD_DEBUG
                field_0x12 = 0;
                #endif
                return 1;
            }
        }
    } else {
        mCreateNum++;
        return 1;
    }

    return 0;
}

J3DModelData* diff_model_c::getModelData() {
    if (mpModel != NULL) {
        return mpModel->getModelData();
    }
    return NULL;
}

void diff_model_c::remove(int param_0) {
    mCreateNum--;

    if (mCreateNum == 0 || param_0 != 0) {
        if (mpHeap != NULL) {
            #if VERSION == VERSION_SHIELD_DEBUG
            if (field_0x12 != 0) {
                JUT_WARN(510, "%s", "Simple Model Denger Remove !!\n");
            }
            #endif

            mDoExt_destroySolidHeap(mpHeap);
            init();
        }
    }
}

void diff_model_c::draw() {
    modelList_c* list = mpList;
    if (list != NULL && dComIfGp_roomControl_checkStatusFlag(getRoomNo(), 0x10)) {
        g_env_light.setLightTevColorType_MAJI(mpModel, dComIfGp_roomControl_getTevStr(getRoomNo()));

        if (mDrawBG == 1) {
            dComIfGd_setListBG();
        }

        for (; list != NULL; list = list->field_0x4) {
            dSmplMdl_modelUpdateDL(list->mpModel);
        }
        mDoExt_modelUpdateDL(mpModel);
        dComIfGd_setList();
        #if VERSION == VERSION_SHIELD_DEBUG
        field_0x12 = 1;
        #endif
    } else {
        #if VERSION == VERSION_SHIELD_DEBUG
        field_0x12 = 0;
        #endif
    }
}

void diff_model_c::init() {
    mpModel = NULL;
    mpHeap = NULL;
    mpList = NULL;
    mCreateNum = 0;
    mRoomNo = 63;
}

BOOL diff_model_c::isSame(J3DModelData* i_modelData, int roomNo) {
    if (mpModel != NULL) {
        return getModelData() == i_modelData && roomNo == getRoomNo();
    }

    return FALSE;
}

s32 diff_model_c::getRoomNo() {
    return mRoomNo;
}

void diff_model_c::insert(modelList_c* list) {
    modelList_c* prev = mpList;
    mpList = list;

    if (list == NULL) {
        return;
    }

    list->field_0x4 = prev;
}
