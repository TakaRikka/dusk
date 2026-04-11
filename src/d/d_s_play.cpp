/**
 * d_s_play.cpp
 * Main Gameplay Scene
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_s_play.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "SSystem/SComponent/c_counter.h"
#include "c/c_dylink.h"
#include "d/actor/d_a_player.h"
#include "d/d_demo.h"
#include "d/d_eye_hl.h"
#include "d/d_item.h"
#include "d/d_model.h"
#include "d/d_map_path_dmap.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "d/d_save_HIO.h"
#include "f_op/f_op_draw_iter.h"
#include "f_op/f_op_msg_mng.h"
#include "f_op/f_op_overlap_mng.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_machine.h"
#include "d/actor/d_a_suspend.h"
#include "d/actor/d_a_ykgr.h"
#include "JSystem/JHostIO/JORFile.h"
#include "JSystem/JHostIO/JORServer.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JFramework/JFWSystem.h"
#include "f_ap/f_ap_game.h"
#include "d/d_bg_parts.h"
#include "f_op/f_op_kankyo_mng.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_midna.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRAramArchive.h"

#if TARGET_PC
#include "dusk/memory.h"
#include <dusk/config.hpp>
#endif

#if DEBUG
#include "d/d_s_menu.h"
#include "d/d_debug_pad.h"
#include "d/d_jpreviewer.h"
#include "d/d_jcam_editor.h"
#include "d/d_a_obj.h"
#include "d/d_debug_viewer.h"
#endif
#include <cstring>

#if PLATFORM_WII
#include "d/d_cursor_mng.h"
#endif

static int dScnPly_Create(scene_class*);
static int dScnPly_Delete(dScnPly_c*);
static int dScnPly_IsDelete(dScnPly_c);
static int dScnPly_Execute(dScnPly_c*);
static int dScnPly_Draw(dScnPly_c*);
static int phase_00(dScnPly_c*);
static int phase_00(dScnPly_c*);
static int phase_1(dScnPly_c*);
static int phase_1_0(dScnPly_c*);
static int phase_01(dScnPly_c*);
static int phase_0(dScnPly_c*);
static int phase_2(dScnPly_c*);
static int phase_3(dScnPly_c*);
static int phase_4(dScnPly_c*);
static int phase_5(dScnPly_c*);
static int phase_6(dScnPly_c*);
static int phase_compleate(void*);

static request_of_phase_process_class resPhase[1];

static request_of_phase_process_class dylPhase[1];

#if DEBUG
static OSTime dylPreLoadTime0;
#endif

static OSTime dylPreLoadTime1;

static OSTime resPreLoadTime0;

static OSTime resPreLoadTime1;

static dScnPly_preLoad_HIO_c g_preLoadHIO;

s8 dScnPly_c::pauseTimer;

s8 dScnPly_c::nextPauseTimer;

#if DEBUG
u8 dScnPly_c::debugPause;
#endif

static const s16 T_JOINT_dylKeyTbl[1] = {
    fpcNm_COW_e,
};

static const char* T_JOINT_resName[1] = {"Always"};

struct PreLoadInfo {
    const char* stageName;
    const s16* profNameTbl;
    const char** resNameTbl;
    u8 dylKeyTblNum;
    u8 resNameNum;
};

static PreLoadInfo const PreLoadInfoT[1] = {
    {
        "T_JOINT",
        T_JOINT_dylKeyTbl,
        T_JOINT_resName,
        ARRAY_SIZE(T_JOINT_dylKeyTbl),
        ARRAY_SIZE(T_JOINT_resName),
    },
};

static s8 preLoadNo = -1;

static u8 doPreLoad = 1;

#if DEBUG
void dScnPly_preLoad_HIO_c::genMessage(JORMContext* mctx) {
    mctx->startComboBox("プリロード", &doPreLoad);
    mctx->genComboBoxItem("しない", 0);
    mctx->genComboBoxItem("する", 1);
    mctx->endComboBox();
}
#endif

s8 dScnPly_c::calcPauseTimer() {
    if (nextPauseTimer != 0) {
        pauseTimer = nextPauseTimer;
        nextPauseTimer = 0;
        return pauseTimer;
    } else {
        return cLib_calcTimer<s8>(&pauseTimer);
    }
}

#if DEBUG
dScnPly_reg_childHIO_c::dScnPly_reg_childHIO_c() {
    for (int i = 0; i < 30; i++) {
        mFloatReg[i] = 0.0f;
    }

    for (int i = 0; i < 10; i++) {
        mShortReg[i] = 0;
    }
}

void dScnPly_reg_childHIO_c::genMessage(JORMContext* mctx) {
    char textbuf[8];

    for (int i = 0; i < 20; i++) {
        sprintf(textbuf, " F(%02d)", i);
        mctx->genSlider(textbuf, &mFloatReg[i], -100000.0f, 100000.0f);
    }

    for (int i = 20; i < 25; i++) {
        sprintf(textbuf, " F(%02d)", i);
        mctx->genSlider(textbuf, &mFloatReg[i], 0.0f, 1.0f);
    }

    for (int i = 25; i < 30; i++) {
        sprintf(textbuf, " F(%02d)", i);
        mctx->genSlider(textbuf, &mFloatReg[i], -1.0f, 1.0f);
    }

    for (int i = 0; i < 10; i++) {
        sprintf(textbuf, " S(%02d)", i);
        mctx->genSlider(textbuf, &mShortReg[i], -0x8000, 0x7FFF);
    }
}

void dScnPly_reg_HIO_c::genMessage(JORMContext* mctx) {
    static const char l_nodeName[][20] = {
        "森田(T)",
        "まつたに(D)",
        "イワワキ(I)",
        "ささ(S)",
        "うめみや(U)",
        "おがわ(O)",
        "坂口(Y)",
        "さかい(K)",
        "ハラ(B)",
        "すみよし(M)",
        "(空)-たけした(A)",
        "吉田茂樹(P)",
        "(空)-のま(N)",
        "(空)-にしわき(W)",
        "(空)-榊原政郎(J)",
        "はやかわ(H)",
        "(空)-定本(V)",
        "(空)-西川(X)",
        "(空)-鈴木（ゆ）(Z)",
        "なかにし(n)",
        "くわじま(k)",
        "おかじま(o)",
        "山崎(y)",
        "芦田(a)",
        "岡田(h)",
        "高橋(t)",
    };

    // "Example usage"
    mctx->genLabel("使い方例", 0);
    mctx->genLabel("R_HS(00-09) ... はやかわレジスタ -32768 - +32767(short)", 0);
    mctx->genLabel("R_HF(00-19) ... はやかわレジスタ -100000.0 - +100000.0", 0);
    mctx->genLabel("R_HF(20-24) ... はやかわレジスタ  0.0f - 1.0f", 0);
    mctx->genLabel("R_HF(25-29) ... はやかわレジスタ -1.0f - 1.0f", 0);

    for (int i = 0; i < 26; i++) {
        mctx->genNode(l_nodeName[i], &mChildReg[i], 0, 0);
    }
}

dScnPly_preset_HIO_c::dScnPly_preset_HIO_c() {
    field_0x5 = 0;
    memset(mPresetData, 0, sizeof(mPresetData));
    memset(filename_buf, 0, sizeof(filename_buf));
    field_0x2716 = 0;
    field_0x2717 = 0;
}

void dScnPly_preset_HIO_c::exePreset() {
    JORFile file;

    if (g_presetHIO.filename_buf[0] != 0) {
        if (file.open((char*)filename_buf, 1, "", NULL, NULL, NULL)) {
            memset(mPresetData, 0, sizeof(mPresetData));
            field_0x2716 = 0;
            field_0x2717 = 1;

            file.readData(mPresetData, 0);
            file.close();

            if (dSm_read_stageset(mPresetData)) {
                field_0x2717 = 2;
            }
        } else {
            // "\nLoading error loop"
            OS_REPORT("\n読み込みしっぱいのまきまき");
        }

        g_presetHIO.filename_buf[0] = 0;
    }
}

void dScnPly_preset_HIO_c::genMessage(JORMContext* mctx) {
    // "Load situation file"
    mctx->genLabel("【状況ファイルを読みこみ】", 0x80000001);
    // "Items and Status can be set in a file"
    mctx->genLabel("　アイテムやステータスの状態をファイルで指定することが出来ます。", 0x80000001);
    mctx->genLabel("", 0x80000001);

    // "Click here to select a file to load!"
    mctx->genButton("ここを押して読込ファイルを指定して下さい！", 0x40000001);
    mctx->genLabel("", 0x80000001);

    // "- Loading during gameplay will automatically return you to the debug menu."
    mctx->genLabel("・ゲーム中に読み込むと自動的にデバッグメニューへ戻ります。", 0x80000001);
    // "- When loading a file from the debug menu,"
    mctx->genLabel("・デバッグメニューでファイルを読み込んだ際", 0x80000001);
    // "  if a stage command exists the game will temporarily load then return to the menu."
    mctx->genLabel("　stage命令がある場合は一旦ゲームに入りメニューへと戻ります。", 0x80000001);
    // "  (To position the cursor)"
    mctx->genLabel("　（カーソルを合わせる為）", 0x80000001);
    mctx->genLabel("", 0x80000001);

    // "Reset state to before loading situation file"
    mctx->genButton("状況ファイルを読み込んでいない状態に戻す", 0x40000003);
    mctx->genLabel("", 0x80000001);
    mctx->genLabel("", 0x80000001);

    // "Click here for the situation file location!"
    mctx->genButton("状況ファイルの格納場所はコチラを押下！", 0x40000005);
    mctx->genLabel("\\\\10.121.2.9\\zelda$\\JMAP_FOLDER\\situation", 0x80000001);
    // "For details about the situation file contents, please refer to Nakamura-san's website"
    mctx->genLabel("ファイルの内容については中村さんのＨＰをご参照ください", 0x80000001);
    // "Click here to launch browser and view webpage."
    mctx->genButton("ここを押すとブラウザが起動し表示されます。", 0x40000004);
    mctx->genLabel("http://www-ead/~nakamuh/gc-zelda_2/misc/spec-situation_file.html", 0x80000001);
}

void dScnPly_preset_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    JORReflexible::listenPropertyEvent(event);
    JORFile file;

    switch ((int)event->id) {
    case 0x40000001:
        if (file.open(1, "すべてのファイル(*.*)\0*.*", NULL, NULL, NULL)) {
            memset(mPresetData, 0, sizeof(mPresetData));
            field_0x2716 = 0;
            field_0x2717 = 1;

            file.readData(mPresetData, 0);
            file.close();

            if (dSm_read_stageset(mPresetData)) {
                field_0x2717 = 2;
            }
        }
        break;
    case 0x40000003:
        memset(mPresetData, 0, sizeof(mPresetData));
        break;
    case 0x40000004:
        JORShellExecute(NULL, "http://www-ead/~nakamuh/gc-zelda_2/misc/spec-situation_file.html", NULL, NULL, 1);
        break;
    case 0x40000005:
        JORShellExecute(NULL, "\\\\10.121.2.9\\zelda$\\ZELDA2_JMAP_FOLDER\\situation", NULL, NULL, 1);
        break;
    }
}
#endif

dScnPly_env_otherHIO_c::dScnPly_env_otherHIO_c() {
    mShadowDensity = 255.0f;
    mAdjustLODBias = TRUE;
    mDisplayTransparentCyl = FALSE;

    #if DEBUG
    mAdjustCullFar = FALSE;
    mCullFarValue = 4000.0f;
    mDisplayShadows = TRUE;
    mDisplayShadowImage = FALSE;
    mDisplayShadowPoly = FALSE;
    mDisplayCullBox = FALSE;
    mLoadAllRooms = FALSE;
    mDepthOfField = TRUE;

    mRailSize = 5.0f;
    mRailColorR = 0x50;
    mRailColorG = 0x50;
    mRailColorB = 0x50;

    field_0x1a = 0xFF;
    mDisplayParticleInfo = 0;
    field_0x4e = 0;
    field_0x4f = 0;

    for (int i = 0; i < 20; i++) {
        field_0x1c[i] = -1;
    }
    #endif
}

void dScnPly_env_otherHIO_c::genMessage(JORMContext* mctx) {
    #if DEBUG
    mctx->genCheckBox("影表示", &mDisplayShadows, 1);
    mctx->genSlider("影濃さ", &mShadowDensity, 0.0f, 255.0f);
    mctx->genCheckBox("被写界深度", &mDepthOfField, 1);
    mctx->genCheckBox("ＬＯＤバイアス変更", &mAdjustLODBias, 1);
    mctx->genCheckBox("カリングＦａｒ変更", &mAdjustCullFar, 1);
    mctx->genSlider("カリングＦａｒ値", &mCullFarValue, 0.0f, 50000.0f);
    mctx->genCheckBox("影イメージ表示", &mDisplayShadowImage, 1);
    mctx->genCheckBox("影ポリゴン表示", &mDisplayShadowPoly, 1);
    mctx->genCheckBox("透明円柱表示", &mDisplayTransparentCyl, 1);
    mctx->genCheckBox("カリングボックス表示", &mDisplayCullBox, 1);
    mctx->genCheckBox("パーティクル情報表示", &mDisplayParticleInfo, 1);
    mctx->genButton("パーティクル履歴", 0x4000003);
    mctx->genCheckBox("全部屋読み込み", &mLoadAllRooms, 1);
    mctx->genButton("部屋メモリ", 0x4000001);
    mctx->genButton("草、花、低木本数", 0x4000002);
    mctx->genSlider("レールサイズ", &mRailSize, 0.0f, 100.0f);
    mctx->genSlider("レールカラー(R)", &mRailColorR, 0, 0xff);
    mctx->genSlider("レールカラー(G)", &mRailColorG, 0, 0xff);
    mctx->genSlider("レールカラー(B)", &mRailColorB, 0, 0xff);
    #endif
}

#if DEBUG
void dScnPly_env_otherHIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    JORReflexible::listenPropertyEvent(event);
    JORFile file;

    switch ((int)event->id) {
    case 0x4000001:
        OS_REPORT("#############################################\n");
        break;
    case 0x4000002:
        break;
    case 0x4000003:
        g_envHIO.mOther.field_0x4e = 1;
        break;
    }
}

void dScnPly_env_otherHIO_c::printParticle() {
    dDbVw_Report(20, 100, "Emitter  Num <%d> X=<%d>", dComIfGp_particle_getEmitterNum(), dComIfGp_particle_getHeapSize());
    dDbVw_Report(20, 120, "Particle Num <%d> Y=<%d>", dComIfGp_particle_getParticleNum(), dComIfGp_particle_getSceneHeapSize() - 0x180000);

    int var_r26 = 0xA0;
    int var_r28 = field_0x4f;
    if (field_0x4e != 0) {
        OS_REPORT("Particle Error !!\n");

        for (int i = 0; i < 20; i++) {
            var_r28--;
            if (var_r28 < 0) {
                var_r28= 19;
            }

            if (field_0x1c[var_r28] != 0xFFFF) {
                OS_REPORT("              <ID=%d : %s>\n", field_0x1c[var_r28], dPa_name::getName(field_0x1c[var_r28]));
            }

            var_r26 += 20;
        }

        field_0x4e = 0;
    }
}
#endif

dScnPly_env_debugHIO_c::dScnPly_env_debugHIO_c() {
    mBoxCullMinSize.set(-100.0f, -100.0f, -100.0f);
    mBoxCullMaxSize.set(100.0f, 100.0f, 100.0f);
    mSphereCullCenter.set(0.0f, 0.0f, 0.0f);
    mSphereCullRadius = 100.0f;
}

void dScnPly_env_debugHIO_c::genMessage(JORMContext* mctx) {
    #if DEBUG
    mctx->genLabel("【ボックスカリング(CULLSIZE_DEBUG)サイズ】", 0x80000001);
    mctx->genSlider("最小Ｘ", &mBoxCullMinSize.x, -10000.0f, 10000.0f);
    mctx->genSlider("最小Ｙ", &mBoxCullMinSize.y, -10000.0f, 10000.0f);
    mctx->genSlider("最小Ｚ", &mBoxCullMinSize.z, -10000.0f, 10000.0f);
    mctx->genSlider("最大Ｘ", &mBoxCullMaxSize.x, -10000.0f, 10000.0f);
    mctx->genSlider("最大Ｙ", &mBoxCullMaxSize.y, -10000.0f, 10000.0f);
    mctx->genSlider("最大Ｚ", &mBoxCullMaxSize.z, -10000.0f, 10000.0f);

    mctx->genLabel("【球カリング(CULLSIZE_Q_DEBUG)サイズ】", 0x80000001);
    mctx->genSlider("中心Ｘ", &mSphereCullCenter.x, -10000.0f, 10000.0f);
    mctx->genSlider("中心Ｙ", &mSphereCullCenter.y, -10000.0f, 10000.0f);
    mctx->genSlider("中心Ｚ", &mSphereCullCenter.z, -10000.0f, 10000.0f);
    mctx->genSlider("半径", &mSphereCullRadius, -10000.0f, 10000.0f);
    #endif
}

#if DEBUG
void dScnPly_env_debugHIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    JORReflexible::listenPropertyEvent(event);

    l_cullSizeBox[fopAc_CULLBOX_14_e].min = mBoxCullMinSize;
    l_cullSizeBox[fopAc_CULLBOX_14_e].max = mBoxCullMaxSize;
    l_cullSizeSphere[fopAc_CULLSPHERE_8_e - fopAc_CULLSPHERE_0_e].center = mSphereCullCenter;
    l_cullSizeSphere[fopAc_CULLSPHERE_8_e - fopAc_CULLSPHERE_0_e].radius = mSphereCullRadius;
}

void dScnPly_env_HIO_c::genMessage(JORMContext* mctx) {
    mctx->genNode("デバッグ用", &mDebug, 0, 0);
    mctx->genNode("その他", &mOther, 0, 0);
}

static u32 l_usedMemoryBlockSize = 0xFFFFFFFF;

static int checkObjectSize(void* i_process, void* i_data) {
    s16 profname = fpcM_GetProfName(i_process);
    if (profname == fpcNm_BG_e) {
        fopAc_ac_c* actor = (fopAc_ac_c*)i_process;
        *((u32*)i_data) += ((l_usedMemoryBlockSize != 0 && actor->heap == NULL) ? 0 : actor->heap->getHeapSize()) + actor->base.base.profile->process_size;
    } else if (profname == fpcNm_PLAY_SCENE_e || profname == fpcNm_ROOM_SCENE_e) {
        *((u32*)i_data) += ((base_process_class*)i_process)->profile->process_size;
    } else if (fopKyM_IsKy(i_process)) {
        *((u32*)i_data + 1) += ((base_process_class*)i_process)->profile->process_size;
    } else if (fopAcM_IsActor(i_process)) {
        fopAc_ac_c* actor = (fopAc_ac_c*)i_process;
        *((u32*)i_data + 1) += (actor->heap == NULL ? 0 : actor->heap->getHeapSize()) + actor->base.base.profile->process_size;

        if (profname == fpcNm_ALINK_e) {
            daAlink_c* alink = (daAlink_c*)actor;
            *((u32*)i_data + 1) += alink->getOtherHeapSize();
        } else if (profname == fpcNm_MIDNA_e) {
            daMidna_c* midna = (daMidna_c*)actor;
            *((u32*)i_data + 1) += midna->getOtherHeapSize();
        }
    }

    return 1;
}

static void initUsedHeapSize() {
    l_usedMemoryBlockSize = 0;
    for (int i = 0; i < 19; i++) {
        JKRHeap* heap = dStage_roomControl_c::getMemoryBlockHeap(i);
        if (heap != NULL) {
            l_usedMemoryBlockSize += heap->getHeapSize();
        }
    }
}

static bool lbl_8074CADB;

static void drawUsedHeapSize() {
    if (mDoCPd_c::getHoldL(PAD_3) && mDoCPd_c::getHoldR(PAD_3) && mDoCPd_c::getTrigZ(PAD_3)) {
        lbl_8074CADB = !lbl_8074CADB;
    }

    if (lbl_8074CADB) {
        int gameSize = mDoExt_getSafeGameHeapSize() - mDoExt_getGameHeap()->getTotalFreeSize();
        int zeldaSize = mDoExt_getSafeZeldaHeapSize() - mDoExt_getZeldaHeap()->getTotalFreeSize();
        int archiveSize = mDoExt_getSafeArchiveHeapSize() - mDoExt_getArchiveHeap()->getTotalFreeSize();
        int j2dSize = mDoExt_getSafeJ2dHeapSize() - mDoExt_getJ2dHeap()->getTotalFreeSize();

        u32 sizes[2];
        sizes[0] = dComIfG_getStageAllSize();
        sizes[1] = dComIfG_getObjectAllSize() - (dComIfG_getObjectSize("CamParam")
                                                + dComIfG_getObjectSize("Event")
                                                + dComIfG_getObjectSize("Always")
                                                + dComIfG_getObjectSize("Alink")
                                                + dComIfG_getObjectSize("Midna"));

        fpcLyIt_All(checkObjectSize, sizes);
        if (l_usedMemoryBlockSize != 0) {
            sizes[0] = l_usedMemoryBlockSize + dComIfG_getStageSize("Stg_00") + dComIfG_getStageSize("Xtg_00");
        }

        int var_r25 = JKRGetAramFreeSize() / 1024;
        JUTReport(380, 300, "Stage    : %d(K)", (sizes[0] + 0x3FF) / 1024);
        JUTReport(380, 316, "Obj      : %d(K)", (sizes[1] + 0x3FF) / 1024);
        JUTReport(380, 332, "C-ARAM   : %d(K)", var_r25);
        JUTReport(380, 348, ">Game    : %d(K)", (gameSize + 0x3FF) / 1024);
        JUTReport(380, 364, ">Zelda   : %d(K)", (zeldaSize + 0x3FF) / 1024);
        JUTReport(380, 380, ">Archive : %d(K)", (archiveSize + 0x3FF) / 1024);
        JUTReport(380, 380, ">J2d     : %d(K)", (j2dSize + 0x3FF) / 1024);
    }
}
#endif

static int dScnPly_Draw(dScnPly_c* i_this) {
    static s16 l_wipeType[] = {
        0, 0, 17, 2, 2, 1, 3, 1, 4, 4, 5, 5, 6, 7, 0, 0, 2, 2, 2, 2, 2, 8, 8,
    };

    #if DEBUG
    fapGm_HIO_c::startCpuTimer();
    fpc_ProcID id = fpcM_GetID(i_this);
    drawUsedHeapSize();
    dStage_DebugDisp();
    #endif

    dComIfG_Ccsp()->Move();
    dComIfG_Bgsp().ClrMoveFlag();

    if (!fopOvlpM_IsPeek() && !dComIfG_resetToOpening(i_this)) {
        if (dComIfGp_isEnableNextStage()
            #if DEBUG
            && !dScnMenu_c::isAutoSelect()
            #endif
        ) {
            JUT_ASSERT(1019, dComIfGp_getNextStageWipe() < ARRAY_SIZEU(l_wipeType));

            u8 wipe = dComIfGp_getNextStageWipe();
            #if DEBUG
            if (g_kankyoHIO.navy.wipe_test_ON != 0xFF) {
                wipe = g_kankyoHIO.navy.wipe_test_ON;
            }
            #endif

            int rt = fopScnM_ChangeReq(i_this, fpcNm_PLAY_SCENE_e, l_wipeType[wipe], 5);

            int hour = dKy_getdaytime_hour();
            BOOL isDaytime = (hour >= 6 && hour < 18) ? FALSE : TRUE;

            if (wipe == 1 || wipe == 2 || wipe == 7 || wipe == 17 || wipe == 21 ||
                ((wipe == 8 || wipe == 10 || wipe == 18) && isDaytime) ||
                ((wipe == 9 || wipe == 11 || wipe == 19) && !isDaytime))
            {
                mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&g_saftyWhiteColor);
            } else if (wipe == 14 || wipe == 20) {
                GXColor color = {0x2A, 0x1E, 0x46, 0xFF};
                if (dKy_darkworld_check()) {
                    color.r = 0xFF;
                    color.g = 0xCF;
                    color.b = 0xB4;
                }
                mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&color);
            } else {
                mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&g_blackColor);
            }
        }
    }

    dMdl_mng_c::reset();

    if (!dComIfGp_isPauseFlag() && !dScnPly_c::isPause()
        #if DEBUG
        && !fapGm_HIO_c::isCaptureScreen()
        #endif
    ) {
        if (fpcM_GetName(i_this) == fpcNm_PLAY_SCENE_e) {
            dComIfGp_getVibration().Run();
        }
        daSus_c::execute();
        dComIfG_Bgsp().Move();
        #if VERSION == VERSION_SHIELD_DEBUG
        dPath_Move();
        #endif
        dComIfGp_particle_calc3D();
        dComIfGp_particle_calc2D();
        cCt_execCounter();
    } else {
        dPa_control_c::onStatus(1);

        if (!dScnPly_c::isPause()) {
            dPa_control_c::onStatus(2);
        }

        if (dScnPly_c::pauseTimer == 0) {
            dComIfGp_getVibration().Pause();
        }
    }

    #if DEBUG
    if (fopOvlpM_IsDoingReq() != TRUE) {
        if (dScnMenu_c::isAutoSelect() || dComIfG_isSceneResetButton() || g_presetHIO.field_0x2717 != 0) {
            if (g_presetHIO.field_0x2717 != 0) {
                dSm_read_stageset(g_presetHIO.mPresetData);
                g_presetHIO.field_0x2717 = 0;
            }

            s16 spA = 0;
            if (dDemo_c::getMode() == 1) {
                dDemo_c::end();
                spA = 7;
            }

            fopScnM_ChangeReq(i_this, fpcNm_MENU_SCENE_e, spA, 5);
            mDoAud_bgmStop(30);

            if (fpcM_GetName(i_this) != fpcNm_PLAY_SCENE_e) {
                dComIfG_playerStatusD();
            }
        }
    }

    dDebugPad.Update();

    fapGm_HIO_c::printCpuTimer("");
    fapGm_HIO_c::stopCpuTimer("ゲーム管理（計算処理２）");
    fapGm_HIO_c::printCpuTimer("");
    #endif

    for (create_tag_class* i = fopDwIt_Begin(); i != NULL; i = fopDwIt_Next(i)) {
        void* process = i->mpTagData;
        fpcM_Draw(process);
    }

    #if DEBUG
    fapGm_HIO_c::startCpuTimer();
    #endif

    if (!dComIfGp_isPauseFlag()) {
        dEyeHL_mng_c::update();

        #if VERSION == VERSION_SHIELD_DEBUG
        dBgp_c::drawShare();
        #endif
        #if DEBUG
        daSus_c::draw();
        #endif

        dComIfG_Ccsp()->Draw();
        dComIfG_Bgsp().Draw();

        #if DEBUG
        dPath_Draw();
        #endif

        dAttention_c* attention = dComIfGp_getAttention();
        attention->Draw();
    }

    #if DEBUG
    if (g_envHIO.mOther.mDisplayParticleInfo) {
        g_envHIO.mOther.printParticle();
    }

    fapGm_HIO_c::printCpuTimer("");
    fapGm_HIO_c::stopCpuTimer("ゲーム管理（描画処理）");
    #endif

    return 1;
}

#if DEBUG
static BOOL l_pause;
static f32 l_pauseFrame;
static u8 lbl_8074CAE4;
static u32 l_sceneChangeStartTick;
#endif

#if TARGET_PC
BOOL firstTime = FALSE;
#endif

static int dScnPly_Execute(dScnPly_c* i_this) {
    #if DEBUG
    fapGm_HIO_c::startCpuTimer();
    #endif

    i_this->offReset();

    #if DEBUG
    if (lbl_8074CAE4) {
        u32 var_r27 = OSGetTick() - l_sceneChangeStartTick;
        OS_REPORT("\x1b[33m");
        // "Scene transition time: %f seconds\n"
        OS_REPORT("シーン切り替え時間　%f秒\n\x1B[m", (f32)OSTicksToMicroseconds(var_r27) / 1000000.0f);
        lbl_8074CAE4 = 0;

        initUsedHeapSize();
        OS_REPORT("Game Heap <%d,%d,%d>\n", mDoExt_getGameHeap()->getTotalUsedSize(), mDoExt_getGameHeap()->getTotalFreeSize(), mDoExt_getGameHeap()->getFreeSize());
        OS_REPORT("Zelda Heap <%d,%d,%d>\n", mDoExt_getZeldaHeap()->getTotalUsedSize(), mDoExt_getZeldaHeap()->getTotalFreeSize(), mDoExt_getZeldaHeap()->getFreeSize());
        OS_REPORT("Archive Heap <%d,%d,%d>\n", mDoExt_getArchiveHeap()->getTotalUsedSize(), mDoExt_getArchiveHeap()->getTotalFreeSize(), mDoExt_getArchiveHeap()->getFreeSize());
        OS_REPORT("最終残り　%d(%fM)\n", JFWSystem::getRootHeap()->getTotalFreeSize(), (f32)JFWSystem::getRootHeap()->getTotalFreeSize() / (f32)0x100000);
        OS_REPORT("拡張最終残り　%d(%fM)\n", JKRGetRootHeap2()->getTotalFreeSize(), (f32)JKRGetRootHeap2()->getTotalFreeSize() / (f32)0x100000);
        OS_REPORT("\x1b[m");
    }
    #endif

    BOOL comboTrig = FALSE;

    dStage_roomControl_c::offNoChangeRoom();
    dStage_roomControl_c::setRoomReadId(0xFF);

    if (!fopOvlpM_IsPeek()) {
        if (mDoAud_zelAudio_c::isBgmSet()) {
            mDoAud_sceneBgmStart();
            mDoAud_load2ndDynamicWave();
            mDoAud_zelAudio_c::offBgmSet();
        }

        if (i_this->calcPauseTimer() != 0) {
            return 1;
        }
    }

    dKy_itudemo_se();

    #if DEBUG
    if (mDoCPd_c::isConnect(PAD_3)) {
        if (mDoCPd_c::getTrigStart(PAD_1) && !fapGmHIO_get2Ddraw()) {
            comboTrig = TRUE;
        } else if (mDoCPd_c::getHoldR(PAD_2) && mDoCPd_c::getTrigStart(PAD_2)) {
            comboTrig = TRUE;
        }

        if (comboTrig == TRUE) {
            if (l_pause) {
                l_pause = FALSE;
            } else {
                l_pause = TRUE;
                l_pauseFrame = 0.0f;
            }
        }

        if (l_pause) {
            l_pauseFrame += mDoCPd_c::getAnalogR(PAD_2);
            if (!mDoCPd_c::getTrigZ(PAD_2) && l_pauseFrame < 1.0f) {
                i_this->onDebugPause();
                return 1;
            }

            l_pauseFrame -= 1.0f;
        }

        i_this->offDebugPause();
    }
    #endif

    if (!dComIfGp_isPauseFlag()) {
        #if DEBUG
        dJprev_c::get()->update();
        #endif

        dDemo_c::update();

        #if DEBUG
        dJcame_c::get()->update();
        #endif

        #if VERSION == VERSION_SHIELD_DEBUG
        dBgp_c::executeShare();
        #endif

        dComIfGp_getEvent()->Step();
        dComIfGp_getAttention()->Run();
    }

    #if DEBUG
    fapGm_HIO_c::printCpuTimer("");
    fapGm_HIO_c::stopCpuTimer("ゲーム管理（計算処理１）");
    fapGm_HIO_c::printCpuTimer("");
    #endif

    return 1;
}

static int dScnPly_IsDelete(dScnPly_c i_this) {
    dComIfGp_particle_cleanup();
    return 1;
}

static int dScnPly_Delete(dScnPly_c* i_this) {
    UNUSED(i_this);

    #if VERSION == VERSION_SHIELD_DEBUG
    for (int i = 0; i < 32; i++) {
        char* bank = dStage_roomControl_c::getArcBank(i);
        if (strcmp(bank, "") != 0) {
            int rt = dComIfG_syncObjectRes(bank);
            if (rt > 0) {
                return 0;
            }
        }
    }
    #endif

    daSus_c::reset();
    dMpath_c::remove();
    dTres_c::remove();

    dAttention_c* attention = dComIfGp_getAttention();
    attention->~dAttention_c();
    dComIfGp_getVibration().Remove();

    dComIfG_Bgsp().Dt();
    dComIfG_Ccsp()->Dt();
    #if VERSION == VERSION_SHIELD_DEBUG
    dPath_Dt();
    #endif
    dStage_Delete();

    dComIfGp_event_remove();

    dComIfGp_particle_removeScene(0);

    dComIfGp_getMsgDtArchive(1)->removeResourceAll();
    JKRUnmountArchive(dComIfGp_getMsgDtArchive(1));

    #if DEBUG
    dJcame_c::remove();
    dJprev_c::remove();
    #endif

    dDemo_c::remove();

    fopMsgM_destroyExpHeap(dComIfGp_getExpHeap2D());
    dComIfGp_setExpHeap2D(NULL);
    fopMsgM_destroyExpHeap(dComIfGp_getMsgExpHeap());
    dComIfGp_setMsgExpHeap(NULL);

    dComIfGp_deleteSimpleModel();
    dMdl_mng_c::remove();

    mDoGph_gInf_c::getBloom()->remove();

    #if VERSION == VERSION_SHIELD_DEBUG
    dBgp_c::removeShare();
    #endif

    dComIfGs_offPlayerFieldLastStayFieldDataExistFlag();

    #if DEBUG
    daObj::HioObj_c::clean();
    mDoHIO_deleteChild(g_envHIO.field_0x4);
    mDoHIO_deleteChild(g_save_bit_HIO.field_0x4);
    g_preLoadHIO.removeHIO();
    #endif

    dComIfGp_setWindowNum(0);
    dComIfGd_setView(NULL);

    if (preLoadNo >= 0) {
        OS_REPORT("\x1b[32mプリロード解放\n\x1b[m");

        const char** resname_table = PreLoadInfoT[preLoadNo].resNameTbl;
        int res_num = PreLoadInfoT[preLoadNo].resNameNum;
        if (resname_table != NULL && (*resname_table != NULL)) {
            for (int i = 0; i < res_num; i++) {
                dComIfG_resDelete(&resPhase[i], resname_table[i]);
            }
        }

        const s16* profname_table = PreLoadInfoT[preLoadNo].profNameTbl;
        int prof_num = PreLoadInfoT[preLoadNo].dylKeyTblNum;
        if (profname_table != NULL && *profname_table != 0) {
            for (int i = 0; i < prof_num; i++) {
                cDylPhs::Unlink(&dylPhase[i], profname_table[i]);
            }
        }
    }

    dComIfGp_init();

    #if PLATFORM_WII
    data_8053a730 = 0;
    #endif

    JUTAssertion::setMessageCount(0);
    return 1;
}

#if DEBUG
static u32 getArchiveHeapSize(JKRHeap* i_heap, dRes_info_c* i_resInfo) {
    if (i_resInfo == NULL) {
        return 0;
    }

    u32 size = 0;
    int var_r29;
    JKRArchive* archive = i_resInfo->getArchive();
    if (archive != NULL) {
        var_r29 = i_heap->getSize(archive);
        if (var_r29 > 0) {
            size += var_r29 + 0x10;
        }

        var_r29 = i_heap->getSize(((JKRAramArchive*)archive)->mBlock);
        if (var_r29 > 0) {
            size += var_r29 + 0x10;
        }
    }

    JKRSolidHeap* dataHeap = i_resInfo->getDataHeap();
    if (dataHeap != NULL) {
        int var_r26 = i_heap->getSize(dataHeap);
        if (var_r26 > 0) {
            size += var_r26 + 0x10;
        }
    }

    return size;
}

static u32 getArchiveBankHeapSize(JKRHeap* i_heap) {
    u32 size = 0;
    for (int i = 0; i < 32; i++) {
        char* bank = dStage_roomControl_c::getArcBank(i);
        if (strcmp(bank, "") != 0) {
            size += getArchiveHeapSize(i_heap, dComIfG_getObjectResInfo(bank));
        }
    }

    return size;
}

static u32 getStageHeapSize(JKRHeap* i_heap) {
    u32 size = 0;
    for (int i = 0; i < 19; i++) {
        JKRHeap* heap = dStage_roomControl_c::getMemoryBlockHeap(i);
        if (heap != NULL) {
            int var_r26 = i_heap->getSize(heap);
            if (var_r26 > 0) {
                size += var_r26 + 0x10;
            }
        }
    }

    if (size == 0) {
        for (int i = 0; i < 64; i++) {
            size += getArchiveHeapSize(i_heap, dComIfG_getStageResInfo(dComIfG_getRoomArcName(i)));
        }
    }

    size += getArchiveHeapSize(i_heap, dComIfG_getStageResInfo("Stg_00")) + getArchiveHeapSize(i_heap, dComIfG_getStageResInfo("Xtg_00"));
    return size;
}

static BOOL heapSizeCheck(const char* i_label, JKRExpHeap* i_heap, s32 i_base, BOOL param_3) {
    s32 free = i_heap->getFreeSize();
    s32 total = i_heap->getTotalUsedSize();

    if (param_3) {
        total -= getArchiveBankHeapSize(i_heap);
        total -= getStageHeapSize(i_heap);
    }

    s32 archiveHeapSize = mDoExt_getSafeArchiveHeapSize();
    f32 totalRatio = (f32)total / (f32)archiveHeapSize;
    f32 freeRatio = (f32)free / (f32)total;
    OS_REPORT("%s Free<%d> Total<%d> Base<%d> TotalRatio<%f> FreeRatio<%f>\n",
        i_label,
        free,
        total,
        i_base,
        totalRatio,
        freeRatio
    );

    int var_r30 = std::abs(total - i_base);
    if (var_r30 > 64) {
        // "%d (0x%x) byte difference!\n"
        OS_REPORT("%d (0x%x) バイトの差があります！\n", var_r30, var_r30);
        i_heap->dump();
        return FALSE;
    }

    return TRUE;
}

static BOOL heapSizeCheck() {
    return heapSizeCheck("ArchiveHeap", mDoExt_getArchiveHeap(), mDoExt_getSafeArchiveHeapSize(), TRUE)
            && heapSizeCheck("J2dHeap", mDoExt_getJ2dHeap(), mDoExt_getSafeJ2dHeapSize(), FALSE)
            && heapSizeCheck("GameHeap", mDoExt_getGameHeap(), mDoExt_getSafeGameHeapSize(), TRUE)
            && heapSizeCheck("ZeldaHeap", mDoExt_getZeldaHeap(), mDoExt_getSafeZeldaHeapSize(), FALSE)
            && heapSizeCheck("CommandHeap", mDoExt_getCommandHeap(), mDoExt_getSafeCommandHeapSize(), FALSE);
}
#endif

bool dScnPly_c::resetGame() {
    if (fpcM_GetName(this) == fpcNm_OPENING_SCENE_e) {
        if (!dStage_roomControl_c::resetArchiveBank(0)) {
            return false;
        }

        if (!mDoAud_resetRecover()) {
            return false;
        }

        if (mDoRst::isReset()) {
            field_0x1d4 = 1;
        }
    } else {
        #if VERSION != VERSION_SHIELD_DEBUG
        if (dComIfGp_getNextStagePoint() == -4 || (dComIfGs_getLastSceneMode() & 0xF) == 0xC ||
            !strcmp(dComIfGp_getNextStageName(), "F_SP109") ||
            (!strcmp(dComIfGp_getNextStageName(), "F_SP116") &&
             dComIfGp_getNextStageRoomNo() == 1 && dComIfGp_getNextStageLayer() == 9))
        {
            if (!dStage_roomControl_c::resetArchiveBank(0)) {
                return false;
            }
        }
        #endif
    }

    dComIfG_setBrightness(255);
    mDoGph_gInf_c::offFade();
    dDlst_list_c::offWipe();
    return true;
}

void dScnPly_c::offReset() {
    if (field_0x1d4 != 0 && !fopOvlpM_IsPeek()) {
        mDoRst::offReset();
        mDoRst::offResetPrepare();
        #if VERSION != VERSION_SHIELD_DEBUG
        JUTGamePad::C3ButtonReset::sResetOccurred = false;
        JUTGamePad::setResetCallback(mDoRst_resetCallBack, NULL);
        #endif
        field_0x1d4 = 0;
    }
}

static int phase_00(dScnPly_c* i_this) {
    if (!i_this->resetGame()) {
        return cPhs_INIT_e;
    }

    #if PLATFORM_WII
    data_8053a730 = 1;
    #endif

    #if DEBUG
    l_sceneChangeStartTick = OSGetTick();
    lbl_8074CAE4 = 1;

    mDoExt_setSafeGameHeapSize();
    mDoExt_setSafeZeldaHeapSize();
    mDoExt_setSafeCommandHeapSize();
    mDoExt_setSafeArchiveHeapSize();
    mDoExt_setSafeJ2dHeapSize();
    #endif

    mDoGph_gInf_c::offBlure();

    #if DEBUG
    if (!heapSizeCheck()) {
        BOOL isDispWarning = mDoCPd_c::isConnect(PAD_3);
        if (isDispWarning) {
            JUT_WARN(2046, "%s", "Memory Danger !!");
        }
    }
    #endif

    return cPhs_NEXT_e;
}

static int phase_01(dScnPly_c* i_this) {
    mDoAud_setHour(dKy_getdaytime_hour());
    mDoAud_setMinute(dKy_getdaytime_minute());
    mDoAud_setWeekday(dKy_get_dayofweek());

    if (dKy_darkworld_check()) {
        mDoAud_setInDarkness(true);
    } else {
        mDoAud_setInDarkness(false);
    }


    mDoAud_setSceneName(dComIfGp_getStartStageName(),
                        dComIfGp_getStartStageRoomNo(),
                        dComIfG_play_c::getLayerNo_common(dComIfGp_getStartStageName(), dComIfGp_getStartStageRoomNo(),
                                                                 dComIfGp_getStartStageLayer()));

    if (!mDoAud_load1stDynamicWave()) {
        return cPhs_INIT_e;
    } else {
        return cPhs_NEXT_e;
    }
}

static int phase_0(dScnPly_c* i_this) {
    return cPhs_NEXT_e;
}

static int phase_1(dScnPly_c* i_this) {
    dStage_roomControl_c::setProcID(fopScnM_GetID(i_this));

    dComIfGp_setStartStage(dComIfGp_getNextStartStage());

    if (dComIfGp_getStartStageLayer() < 0 && daPy_py_c::checkRoomRestartStart()) {
        int layer = dComIfG_play_c::getLayerNo_common_common(dComIfGp_getStartStageName(), dComIfGp_getStartStageRoomNo(), dComIfGp_getStartStageLayer());
        if (layer < 0) {
            dComIfGp_setStartStageLayer(dComIfGp_getLayerOld());
        }
    }

    dComIfGp_offEnableNextStage();

    // Stage: Faron Woods, Room: Faron Spring
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP108") && dComIfGp_getStartStageRoomNo() == 1 &&
        dComIfGp_getStartStagePoint() == 3)
    {
        dComIfGs_onDarkClearLV(0);
        execItemGet(dItemNo_WEAR_KOKIRI_e);
    }
    // Stage: Kakariko Village, Room: Kakariko Village
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP109") &&
             dComIfGp_getStartStageRoomNo() == 0 && dComIfGp_getStartStagePoint() == 30)
    {
        dComIfGs_onDarkClearLV(1);
    }
    // Stage: Lake Hylia, Room: Fountain
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP115") &&
             dComIfGp_getStartStageRoomNo() == 1 && dComIfGp_getStartStagePoint() == 20)
    {
        dComIfGs_onDarkClearLV(2);
    }
    // Stage: Sacred Grove, Room: Lost Woods
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP117") &&
             dComIfGp_getStartStageRoomNo() == 1 && dComIfGp_getStartStagePoint() == 99)
    {
        dComIfGs_onDarkClearLV(3);
    }

    // Stage: Ordon Spring, Room: Ordon Spring
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP104") && dComIfGp_getStartStageRoomNo() == 1 &&
        dComIfGp_getStartStagePoint() == 23 && dComIfGp_getStartStageLayer() == 12)
    {
        dComIfGs_onItemFirstBit(dItemNo_HORSE_FLUTE_e);
        dComIfGs_setItem(SLOT_21, dItemNo_HORSE_FLUTE_e);
    }

    if ((u8)dKy_darkworld_stage_check(dComIfGp_getStartStageName(),
                                      dComIfGp_getStartStageRoomNo()) == true)
    {
        dComIfGp_world_dark_set(1);
    } else if ((u8)dKy_darkworld_spot_check(dComIfGp_getStartStageName(),
                                            dComIfGp_getStartStageRoomNo()) == true)
    {
        dComIfGp_world_dark_set(2);
    } else {
        dComIfGp_world_dark_set(0);
    }

    #if DEBUG
    if (dKy_darkworld_check()) {
        if (dComIfGp_getStartStageLayer() != -1 && dComIfGp_getStartStageLayer() < 10 && dComIfG_play_c::getLayerNo(0) < 10) {
            if (strcmp(dComIfGp_getStartStageName(), "D_MN08") != 0) {
                // "Twilight layer spec error! sasaki"
                OSReport_Error("\nトワイライトのレイヤー指定異常です！ sasaki");
                // "(If loaded from the debug menu, you can directly specify layers, so I can't tell if it's invalid.)"
                OSReport_Error("\n(デバグメニューからの場合はレイヤー直指定できるので危険なのかは予測つきません。）");

                OSReport_Error("\nstage[%s]", dComIfGp_getStartStageName());
                OSReport_Error("\nroom[%d]", dComIfGp_getStartStageRoomNo());
                OSReport_Error("\nlayer[%d]", dComIfG_play_c::getLayerNo(0));
                OSReport_Error("\nTW1[%d]", dComIfGs_isDarkClearLV(0));
                OSReport_Error("\nTW2[%d]", dComIfGs_isDarkClearLV(1));
                OSReport_Error("\nTW3[%d]", dComIfGs_isDarkClearLV(2));
                OSReport_Error("\nTW4[%d]", dComIfGs_isDarkClearLV(3));
            }
        }
    }
    #endif

    // Stage: Lake Hylia, Room: Fountain
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP115") && dComIfGp_getStartStageRoomNo() == 1 &&
        dComIfGp_getStartStageLayer() < 0)
    {
        if (dComIfG_play_c::getLayerNo_common(dComIfGp_getStartStageName(), dComIfGp_getStartStageRoomNo(), dComIfGp_getStartStageLayer()) == 9) {
            dComIfGp_setStartStageLayer(9);
        }
    }

    // Stage: Faron Woods, Room: South Faron
    // Stage: Faron Woods Cave
    // Stage: Kakariko Village, Room: Kakariko Village
    // Stage: Lake Hylia, Room: Fountain
    if ((!strcmp(dComIfGp_getStartStageName(), "F_SP108") && dComIfGp_getStartStageRoomNo() == 0 &&
         dComIfGp_getStartStageLayer() == 7) ||
        (!strcmp(dComIfGp_getStartStageName(), "D_SB10") && dComIfG_play_c::getLayerNo(0) == 14) ||
        (!strcmp(dComIfGp_getStartStageName(), "F_SP109") && dComIfGp_getStartStageRoomNo() == 0 &&
         dComIfGp_getStartStageLayer() == 8) ||
        (!strcmp(dComIfGp_getStartStageName(), "F_SP115") && dComIfGp_getStartStageRoomNo() == 1 &&
         dComIfGp_getStartStageLayer() == 8) ||
        (!strcmp(dComIfGp_getStartStageName(), "F_SP115") && dComIfGp_getStartStageRoomNo() == 1 &&
         dComIfGp_getStartStageLayer() == 9))
    {
        dComIfGp_world_dark_set(1);
    }

    // Stage: Zant Boss Fight, Room: Throne Room
    if (!strcmp(dComIfGp_getStartStageName(), "D_MN08D") && dComIfGp_getStartStageRoomNo() == 50 &&
        dComIfGp_getStartStagePoint() == 20)
    {
        dComIfGs_onSaveDunSwitch(30);
    }

    #if DEBUG
    if (!strcmp(dComIfGp_getStartStageName(), "NPC_GND") && dComIfGp_getStartStageRoomNo() == 0 &&
        dComIfGp_getStartStageLayer() >= 12)
    {
        dComIfGp_world_dark_set(1);
    }
    #endif

    dKy_darkworld_Area_set(dComIfGp_getStartStageName(), dComIfGp_getStartStageRoomNo());

    // Stage: Hyrule Castle Sewers, Room: Prison Cell
    if (!strcmp(dComIfGp_getStartStageName(), "R_SP107") && dComIfGp_getStartStageRoomNo() == 0 &&
        (dComIfGp_getStartStagePoint() == 24 || dComIfGp_getStartStagePoint() == 0))
    {
        dComIfGs_onTransformLV(0);
    }
    // Stage: Hyrule Field, Room: Eldin Gorge south entrance
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP121") &&
             dComIfGp_getStartStageRoomNo() == 2 && dComIfGp_getStartStagePoint() == 10)
    {
        dComIfGs_onTransformLV(1);
    }
    // Stage: Hyrule Field, Room: Lanayru Field north entrance
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP121") &&
             dComIfGp_getStartStageRoomNo() == 9 && dComIfGp_getStartStagePoint() == 10)
    {
        dComIfGs_onTransformLV(2);
    }
    // Stage: Hyrule Field, Room: Lanayru Field
    else if (!strcmp(dComIfGp_getStartStageName(), "F_SP121") &&
             dComIfGp_getStartStageRoomNo() == 10 &&
             (dComIfGp_getStartStagePoint() == 23 || dComIfGp_getStartStagePoint() == 20))
    {
        dComIfGs_onTransformLV(3);
    }

    // Stage: Fishing Pond, Room: Fishing Pond
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP127") && dComIfGp_getStartStageRoomNo() == 0 &&
        dComIfGp_getStartStagePoint() == 2)
    {
        g_env_light.fishing_hole_season++;

        if (g_env_light.fishing_hole_season > 4) {
            g_env_light.fishing_hole_season = 1;
        }
    }

    dComIfGs_BossLife_public_Set(0xFF);

    #if DEBUG
    if (g_env_light.light_mask_type & 0xF0) {
        g_env_light.light_mask_type = g_env_light.light_mask_type & 0xF;
    } else {
        g_env_light.light_mask_type = 0;
    }
    #else
    g_env_light.light_mask_type = 0;
    #endif

    JUTReportConsole_f("Start StageName:RoomNo [%s:%d]\n", dComIfGp_getStartStageName(),
                       dComIfGp_getStartStageRoomNo());
    dComIfGp_setStatus(0);

    if (dComIfG_syncStageRes("Stg_00") < 0) {
        int rt = dComIfG_setStageRes("Stg_00", NULL);
        JUT_ASSERT(2442, rt == 1);
    }

    return cPhs_NEXT_e;
}

static int phase_1_0(dScnPly_c* i_this) {
    static char camparamarc[10] = "CamParam";

    int rt = dComIfG_syncStageRes("Stg_00");
    JUT_ASSERT(2469, rt >= 0);

    if (rt != 0) {
        return cPhs_INIT_e;
    } else {
        dStage_infoCreate();
        dComIfG_setObjectRes("Event", (u8)0, NULL);
        dComIfGp_setCameraParamFileName(0, camparamarc);
        dComIfG_setObjectRes("CamParam", (u8)0, NULL);
        return cPhs_NEXT_e;
    }
}

static int phase_2(dScnPly_c* i_this) {
    int rt = dComIfG_syncAllObjectRes();
    if (rt >= 0 && rt != 0) {
        return cPhs_INIT_e;
    }

    u8 particle_no = dStage_stagInfo_GetParticleNo(dComIfGp_getStage()->getStagInfo(),
                                                   dComIfG_play_c::getLayerNo(0));
    if (particle_no == 255) {
        particle_no = dStage_stagInfo_GetParticleNo(dComIfGp_getStage()->getStagInfo());
    }

    #if DEBUG
    if (fapGm_HIO_c::mParticle254Fix) {
        particle_no = 0xFE;
    }
    #endif

    OS_REPORT("============== sceneParticleNo=%d\n", particle_no);
    dComIfGp_particle_readScene(particle_no, &i_this->sceneCommand);
    dMsgObject_readMessageGroup(&i_this->field_0x1d0);
    return cPhs_NEXT_e;
}

static int phase_3(dScnPly_c* i_this) {
    if ((i_this->sceneCommand != NULL && !i_this->sceneCommand->sync()) || mDoAud_check1stDynamicWave()) {
        return cPhs_INIT_e;
    }

    if (i_this->field_0x1d0 != NULL && !i_this->field_0x1d0->sync()) {
        return cPhs_INIT_e;
    }

    #if VERSION == VERSION_SHIELD_DEBUG
    dBgp_c::createShare();
    #endif

    return cPhs_NEXT_e;
}

dScnPly_reg_HIO_c g_regHIO;

dScnPly_env_HIO_c g_envHIO;

#if DEBUG
dScnPly_preset_HIO_c g_presetHIO;
#endif

static int phase_4(dScnPly_c* i_this) {
    #if VERSION == VERSION_SHIELD_DEBUG
    if (!dBgp_c::executeShare()) {
        return cPhs_INIT_e;
    }
    #endif

    if (i_this->sceneCommand) {
        JUT_ASSERT(2610, i_this->sceneCommand->getMemAddress() != NULL);
        dComIfGp_particle_createScene(i_this->sceneCommand->getMemAddress());
        JKR_DELETE(i_this->sceneCommand);
    } else {
        dComIfGp_particle_createScene(NULL);
    }

    if (i_this->field_0x1d0 != NULL) {
        dComIfGp_setMsgDtArchive(1, i_this->field_0x1d0->getArchive());
        i_this->field_0x1d0->destroy();
    }

    dComIfGp_calcNowRegion();
    #if DEBUG
    dComIfG_initStopwatch();
    #endif
    dComIfG_Bgsp().Ct();
    fopAcM_lc_c::getLineCheck()->ClearPi();
    fopAcM_gc_c::getGroundCheck()->ClearPi();
    fopAcM_rc_c::getRoofCheck()->ClearPi();
    fopAcM_wt_c::getWaterCheck()->ClearPi();
    dComIfG_Ccsp()->Ct();
    #if VERSION == VERSION_SHIELD_DEBUG
    dPath_Ct();
    #endif
    dDemo_c::create();
    dEyeHL_mng_c::create();

    #if DEBUG
    dJcame_c::create(dDemo_c::getSystem(), 0.03333334f, *mDoCPd_c::getGamePad(PAD_4));
    dJprev_c::create((JStudio::TControl*)dDemo_c::getControl(), *mDoCPd_c::getGamePad(PAD_4));
    #endif

    dComIfGp_setPlayerInfo(0, NULL, 0);
    for (int i = 0; i < 2; i++) {
        dComIfGp_setPlayerPtr(i, NULL);
    }

#if TARGET_PC
    dComIfGp_setWindow(0, 0.0f, 0.0f, mDoGph_gInf_c::getWidth(), mDoGph_gInf_c::getHeight(), 0.0f,
                       1.0f, 0, 2);
#else
    dComIfGp_setWindow(0, 0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, 0.0f, 1.0f, 0, 2);
#endif
    dComIfGp_setCameraInfo(0, NULL, 0, 0, -1);
    dComIfGd_setWindow(NULL);
    dComIfGd_setViewport(NULL);
    dComIfGd_setView(NULL);

    JKRExpHeap* heap = fopMsgM_createExpHeap(HEAP_SIZE(0xBB800, 0x177000), NULL);
#if TARGET_PC
    heap->setName("Scene2DHeap");
#endif

    JUT_ASSERT(2704, heap != NULL);
    dComIfGp_setExpHeap2D(heap);

    JKRExpHeap* heap2 = fopMsgM_createExpHeap(HEAP_SIZE(0xA800, 0x15000), NULL);
#if TARGET_PC
    heap2->setName("SceneMsgHeap");
#endif
    JUT_ASSERT(2709, heap2 != NULL);
    dComIfGp_setMsgExpHeap(heap2);

    if (fpcM_GetName(i_this) == fpcNm_OPENING_SCENE_e) {
        fopAcM_create(fpcNm_TITLE_e, 0, NULL, -1, NULL, NULL, -1);
        #if DEBUG
        g_playerKind = 0;
        #endif
        dComIfGs_init();
        dComIfGs_setOptPointer(0);
        dComIfGs_setLife(12);
        dMeter2Info_setCloth(dItemNo_WEAR_KOKIRI_e, false);
        dMeter2Info_setSword(dItemNo_SWORD_e, false);
        dMeter2Info_setShield(dItemNo_HYLIA_SHIELD_e, false);
        dComIfGs_onEventBit(0x0601);  // Epona Tamed
    }

    dMpath_c::create();
    dTres_c::create();
    dStage_Create();
    dComIfGp_createSimpleModel();
    dMdl_mng_c::create();

    mDoGph_gInf_c::setTickRate(OS_TIMER_CLOCK / 30);

    #if DEBUG
    g_preLoadHIO.entryHIO("プリロード制御");
    #endif
    g_envHIO.field_0x4 = mDoHIO_CREATE_CHILD("描画設定", &g_envHIO);
    g_save_bit_HIO.field_0x4 = mDoHIO_CREATE_CHILD("記録ビット", &g_save_bit_HIO);

    #if DEBUG
    daObj::HioObj_c::init();
    #endif

    dAttention_c* attention = dComIfGp_getAttention();
    JKR_NEW_ARGS (attention) dAttention_c(dComIfGp_getPlayer(0), 0);
    dComIfGp_getVibration().Init();
    daYkgr_c::init();

    dComIfG_setBrightness(255);
    mDoGph_gInf_c::offFade();
    mDoAud_zelAudio_c::onBgmSet();
    dScnPly_c::pauseTimer = 0;
    dScnPly_c::nextPauseTimer = 0;
    preLoadNo = -1;

    if (doPreLoad != 0) {
        const char* stageName = dComIfGp_getStartStageName();
        OS_REPORT("stageName=%s\n", stageName);
        for (u32 i = 0; i < 1; i++) {
            if (strcmp(stageName, PreLoadInfoT[i].stageName) == 0) {
                preLoadNo = i;
            }
        }
    }

    if (preLoadNo < 0) {
        return cPhs_COMPLEATE_e;
    }

    OS_REPORT("\x1b[32mプリロードやります\n\x1b[m");
    resPreLoadTime0 = OSGetTime();
    return cPhs_NEXT_e;
}

static int phase_5(dScnPly_c* i_this) {
    if (preLoadNo >= 0) {
        int phase_state = cPhs_NEXT_e;
        int goodLoads = 0;
        int loadNum = 0;

        const char** resNames = PreLoadInfoT[preLoadNo].resNameTbl;
        s32 resNameNum = PreLoadInfoT[preLoadNo].resNameNum;
        if (resNames != NULL && *resNames != NULL) {
            JUT_ASSERT(2830, resNameNum <= ARRAY_SIZEU(resPhase));

            for (int i = 0; i < resNameNum; i++) {
                int load_phase = dComIfG_resLoad(&resPhase[i], resNames[i]);
                if (load_phase != cPhs_COMPLEATE_e) {
                    phase_state = cPhs_INIT_e;
                } else {
                    goodLoads++;
                }
                loadNum++;
            }

            OS_REPORT("\x1b[32mリソースプリロード %d/%d\n\x1b[m", goodLoads, loadNum);
        }
    
        if (phase_state == cPhs_COMPLEATE_e) {
            resPreLoadTime1 = OSGetTime();
            OS_REPORT("\x1b[32mリソースプリロード %lld ms\n\x1b[m", OSTicksToMilliseconds(resPreLoadTime1 - resPreLoadTime0));
        }
    
        return phase_state;
    }

    return cPhs_NEXT_e;
}

static int phase_6(dScnPly_c* i_this) {
    if (preLoadNo >= 0) {
        int phase_state = cPhs_NEXT_e;
        int goodLoads = 0;
        int loadNum = 0;

        const s16* dylKeyTbl = PreLoadInfoT[preLoadNo].profNameTbl;
        s32 dylKeyTblNum = PreLoadInfoT[preLoadNo].dylKeyTblNum;
        if (dylKeyTbl != NULL && *dylKeyTbl != 0) {
            JUT_ASSERT(2864, dylKeyTblNum <= ARRAY_SIZEU(dylPhase));

            for (int i = 0; i < dylKeyTblNum; i++) {
                int load_phase = cDylPhs::Link(&dylPhase[i], dylKeyTbl[i]);
                if (load_phase != cPhs_COMPLEATE_e) {
                    phase_state = cPhs_INIT_e;
                } else {
                    goodLoads++;
                }
                loadNum++;
            }

            OS_REPORT("\x1b[32mダイナミックリンクプリロード %d/%d\n\x1b[m", goodLoads, loadNum);
        }

        if (phase_state == cPhs_COMPLEATE_e) {
            dylPreLoadTime1 = OSGetTime();
            OS_REPORT("\x1b[32mダイナミックリンクプリロード %lld ms\n\x1b[m", OSTicksToMilliseconds(dylPreLoadTime1 - dylPreLoadTime0));
        }

        return phase_state;
    }

    return cPhs_NEXT_e;
}

static int phase_compleate(void* i_this) {
    return cPhs_COMPLEATE_e;
}

static int dScnPly_Create(scene_class* i_this) {
    static request_of_phase_process_fn l_method[] = {
        (request_of_phase_process_fn)phase_00,
        (request_of_phase_process_fn)phase_1,
        (request_of_phase_process_fn)phase_1_0,
        (request_of_phase_process_fn)phase_01,
        (request_of_phase_process_fn)phase_0,
        (request_of_phase_process_fn)phase_2,
        (request_of_phase_process_fn)phase_3,
        (request_of_phase_process_fn)phase_4,
        (request_of_phase_process_fn)phase_5,
        (request_of_phase_process_fn)phase_6,
        (request_of_phase_process_fn)phase_compleate,
    };

    dScnPly_c* a_this = (dScnPly_c*)i_this;
    int phase_state = dComLbG_PhaseHandler(&a_this->field_0x1c4, l_method, a_this);
    return phase_state;
}

static scene_method_class l_dScnPly_Method = {
    (process_method_func)dScnPly_Create,  (process_method_func)dScnPly_Delete,
    (process_method_func)dScnPly_Execute, (process_method_func)dScnPly_IsDelete,
    (process_method_func)dScnPly_Draw,
};

scene_process_profile_definition g_profile_PLAY_SCENE = {
    /* Layer ID     */ fpcLy_ROOT_e,
    /* List ID      */ 1,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_PLAY_SCENE_e,
    /* Proc SubMtd  */ &g_fpcNd_Method.base,
    /* Size         */ sizeof(dScnPly_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopScn_Method.base,
    /* Scene SubMtd */ &l_dScnPly_Method,
};

scene_process_profile_definition g_profile_OPENING_SCENE = {
    /* Layer ID     */ fpcLy_ROOT_e,
    /* List ID      */ 1,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_OPENING_SCENE_e,
    /* Proc SubMtd  */ &g_fpcNd_Method.base,
    /* Size         */ sizeof(dScnPly_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopScn_Method.base,
    /* Scene SubMtd */ &l_dScnPly_Method,
};
