#include "f_ap/f_ap_game.h"
#include "SSystem/SComponent/c_counter.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_draw_tag.h"
#include "f_op/f_op_overlap_mng.h"
#include "f_op/f_op_scene_mng.h"
#include "m_Do/m_Do_main.h"
#include "m_Do/m_Do_graphic.h"
#include "DynamicLink.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRAramArchive.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JHostIO/JORFile.h"
#include "JSystem/J3DGraphLoader/J3DModelLoader.h"
#include "JSystem/J3DGraphLoader/J3DModelSaver.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_midna.h"
#include "d/d_model.h"
#include "d/actor/d_a_grass.h"
#include "d/d_tresure.h"

fapGm_HIO_c::fapGm_HIO_c() {
    mUsingHostIO = true;

    if (mDoMain::developmentMode) {
        mDisplayMeter = true;
        mDisplayPrint = true;
    } else {
        mDisplayMeter = false;
        mDisplayPrint = false;
        mDisplay2D = false;
    }

    mDisplay2D = true;
    mDisplayParticle = true;
    mDisplayProcessID = false;
    mMemBlockOff = false;

    mColor = JUtility::TColor(255, 255, 255, 255);

    mLROnValue = 0.9f;
    mLROffValue = 0.6f;

    mLetterTopColor = JUtility::TColor(255, 150, 0, 255);
    mLetterBottomColor = JUtility::TColor(255, 120, 0, 255);
    mLetterTopShadowColor = JUtility::TColor(0, 0, 0, 255);
    mLetterBottomShadowColor = JUtility::TColor(0, 0, 0, 255);

    mLetterPositionX = 0;
    mLetterPositionY = 10;
    mLetterFontSize = 27;
    mLineSpacing = 0;
    mLetterSpacing = 0;
    mBackgroundAlpha = 130;

#if DEBUG
    mTrapFilter = 0;
    mGammaCorrection = 10;
#endif
}

#if DEBUG
u8 fapGm_HIO_c::mCaptureMagnification = 1;
u8 fapGm_HIO_c::mCaptureScreenDivH = 1;
u8 fapGm_HIO_c::mCaptureScreenDivV = 1;
u8 fapGm_HIO_c::mPackArchiveMode = 1;

CaptureScreen* fapGm_HIO_c::mCaptureScreen;
void* fapGm_HIO_c::mCaptureScreenBuffer;
s16 fapGm_HIO_c::mCaptureScreenFlag;
u16 fapGm_HIO_c::mCaptureScreenWidth;
u16 fapGm_HIO_c::mCaptureScreenHeight;
u16 fapGm_HIO_c::mCaptureScreenLinePf;
u16 fapGm_HIO_c::mCaptureScreenLineNum;
u8 fapGm_HIO_c::mCaptureScreenNumH;
u8 fapGm_HIO_c::mCaptureScreenNumV;

u8 fapGm_HIO_c::mParticle254Fix;
#endif

#if DEBUG
void fapGm_HIO_c::genMessage(JORMContext* mctx) {
    mctx->genButton("警告消去", 0x4000020);
    mctx->genButton("ＢＭＰキャプチャー", 0x4000003);
    mctx->genSlider("ＢＭＰキャプチャー倍率", &mCaptureMagnification, 1, 8);
    mctx->genButton("スナップショット", 0x4000002);
    mctx->genButton("ＣＰＵ時間", 0x4000007);
    mctx->genCheckBox("ホストＩＯ使用", &mUsingHostIO, 0x1);
    mctx->genCheckBox("メーター表示(JUTProcBar)", &mDisplayMeter, 0x1);
    mctx->genCheckBox("プリント表示(JUTDbPrint)", &mDisplayPrint, 0x1);
    mctx->genCheckBox("２Ｄ表示", &mDisplay2D, 0x1);
    mctx->genCheckBox("パーティクル表示", &mDisplayParticle, 0x1);
    mctx->genCheckBox("パーティクル２５４固定", &mParticle254Fix, 0x1);
    mctx->genCheckBox("パックアーカイブモード", &mPackArchiveMode, 0x1);

    mctx->genLabel("−−−−−−−【プリント】−−−−−−−", 0x4000001);
    mctx->genSlider(" 色     Ｒ", &mColor.r, 0, 0xFF, 0x4000010);
    mctx->genSlider("        Ｇ", &mColor.g, 0, 0xFF, 0x4000011);
    mctx->genSlider("        Ｂ", &mColor.b, 0, 0xFF, 0x4000012);
    mctx->genSlider("        Ａ", &mColor.a, 0, 0xFF, 0x4000013);

    mctx->genLabel("−−−−−−−【情報関係】−−−−−−−", 0x4000001);
    mctx->genButton("空きメモリ", 0x4000005);
    mctx->genCheckBox("プロセスＩＤ表示", &mDisplayProcessID, 0x1);
    mctx->genCheckBox("トラップフィルター", &mTrapFilter, 0x1);

    mctx->startComboBox("ガンマ補正係数", &mGammaCorrection);
    mctx->genComboBoxItem("0.1", 1);
    mctx->genComboBoxItem("0.2", 2);
    mctx->genComboBoxItem("0.3", 3);
    mctx->genComboBoxItem("0.4", 4);
    mctx->genComboBoxItem("0.5", 5);
    mctx->genComboBoxItem("0.6", 6);
    mctx->genComboBoxItem("0.7", 7);
    mctx->genComboBoxItem("0.8", 8);
    mctx->genComboBoxItem("0.9", 9);
    mctx->genComboBoxItem("1.0", 10);
    mctx->genComboBoxItem("1.1", 11);
    mctx->genComboBoxItem("1.2", 12);
    mctx->genComboBoxItem("1.3", 13);
    mctx->genComboBoxItem("1.4", 14);
    mctx->genComboBoxItem("1.5", 15);
    mctx->genComboBoxItem("1.6", 16);
    mctx->genComboBoxItem("1.7", 17);
    mctx->genComboBoxItem("1.8", 18);
    mctx->genComboBoxItem("1.9", 19);
    mctx->genComboBoxItem("2.0", 20);
    mctx->genComboBoxItem("2.1", 21);
    mctx->genComboBoxItem("2.2", 22);
    mctx->genComboBoxItem("2.3", 23);
    mctx->genComboBoxItem("2.4", 24);
    mctx->genComboBoxItem("2.5", 25);
    mctx->genComboBoxItem("2.6", 26);
    mctx->genComboBoxItem("2.7", 27);
    mctx->genComboBoxItem("2.8", 28);
    mctx->genComboBoxItem("2.9", 29);
    mctx->genComboBoxItem("3.0", 30);
    mctx->endComboBox();

    mctx->genLabel("−−−−−−−【肩ボタン】−−−−−−−", 0x4000001);
    mctx->genSlider(" しきい値  オン", &g_HIO.mLROnValue, 0.0f, 1.0f);
    mctx->genSlider("           オフ", &g_HIO.mLROffValue, 0.0f, 1.0f);
    
    mctx->genLabel("−−−−−−−【デバッグ用】−−−−−−", 0x4000001);
    mctx->genCheckBox("メモリーブロックＯＦＦ", &mMemBlockOff, 0x1);
    
    mctx->genLabel("−−−−−−−【システムレジスタ】−−−", 0x4000001);
    mctx->genSlider("レジスタ０", &mRegister0, -0x8000, 0x7fff);
    mctx->genSlider("レジスタ１", &mRegister1, -0x8000, 0x7fff);
    mctx->genSlider("レジスタ２", &mRegister2, -0x8000, 0x7fff);
    mctx->genSlider("レジスタ３", &mRegister3, -0x8000, 0x7fff);

    mctx->genLabel("−−−−−−−【ＢＤＬファイルへの変換】", 0x4000001);
    mctx->genButton("変換", 0x4000014);
    
    mctx->genLabel("−−−−−−−【メモリファイル】−", 0x4000001);
    mctx->genButton("出力", 0x4000016);
    
    mctx->genLabel("−−−−−−−【エラーメッセージ】−", 0x4000001);
    mctx->genSlider("文字色上ｒ", &mLetterTopColor.r, 0, 0xFF);
    mctx->genSlider("文字色上ｇ", &mLetterTopColor.g, 0, 0xFF);
    mctx->genSlider("文字色上ｂ", &mLetterTopColor.b, 0, 0xFF);
    mctx->genSlider("文字色上α", &mLetterTopColor.a, 0, 0xFF);

    mctx->genSlider("文字色下ｒ", &mLetterBottomColor.r, 0, 0xFF);
    mctx->genSlider("文字色下ｇ", &mLetterBottomColor.g, 0, 0xFF);
    mctx->genSlider("文字色下ｂ", &mLetterBottomColor.b, 0, 0xFF);
    mctx->genSlider("文字色下α", &mLetterBottomColor.a, 0, 0xFF);

    mctx->genSlider("文字位置Ｘ", &mLetterPositionX, -300, 300);
    mctx->genSlider("文字位置Ｙ", &mLetterPositionY, -300, 300);
    mctx->genSlider("文字行間", &mLineSpacing, -10, 10);
    mctx->genSlider("文字字間", &mLetterSpacing, -10, 10);
    mctx->genSlider("文字フォントサイズ", &mLetterFontSize, 0, 0x32);

    mctx->genSlider("文字色影上ｒ", &mLetterTopShadowColor.r, 0, 0xFF);
    mctx->genSlider("文字色影上ｇ", &mLetterTopShadowColor.g, 0, 0xFF);
    mctx->genSlider("文字色影上ｂ", &mLetterTopShadowColor.b, 0, 0xFF);
    mctx->genSlider("文字色影上α", &mLetterTopShadowColor.a, 0, 0xFF);

    mctx->genSlider("文字色影下ｒ", &mLetterBottomShadowColor.r, 0, 0xFF);
    mctx->genSlider("文字色影下ｇ", &mLetterBottomShadowColor.g, 0, 0xFF);
    mctx->genSlider("文字色影下ｂ", &mLetterBottomShadowColor.b, 0, 0xFF);
    mctx->genSlider("文字色影下α", &mLetterBottomShadowColor.a, 0, 0xFF);

    mctx->genSlider("背景α", &mBackgroundAlpha, 0, 0xFF);
}

char fapGm_dataMem::mCsv[0x8000];

int dumpTagObject(void* i_object, void*) {
    char profname_str[64];
    s16 profname = fopAcM_GetProfName(i_object);
    sprintf(profname_str, "%d", profname);

    if (fopAcM_IsActor(i_object)) {
        fopAc_ac_c* a_actor = (fopAc_ac_c*)i_object;

        const char* actorname = dStage_getName(profname, -1);
        fapGm_dataMem::printfTag(fapGm_dataMem::TagAtt_Variable_e, fapGm_dataMem::TagType_Object_e, 0, profname_str, a_actor, NULL, *actorname == 0 ? NULL : actorname, NULL);

        if (a_actor->heap != NULL) {
            fapGm_dataMem::printfTag(fapGm_dataMem::TagAtt_Variable_e, fapGm_dataMem::TagType_Object_e, 0, profname_str, a_actor->heap, NULL, actorname, NULL);
        }

        u32 other_heap_size = 0;
        if (profname == PROC_ALINK) {
            fopAc_ac_c* spC = a_actor;
            other_heap_size = daAlink_c::getOtherHeapSize();
        } else if (profname == PROC_MIDNA) {
            fopAc_ac_c* sp8 = a_actor;
            other_heap_size = daMidna_c::getOtherHeapSize();
        }

        if (other_heap_size != 0) {
            fapGm_dataMem::printfTag(fapGm_dataMem::TagAtt_Variable_e, fapGm_dataMem::TagType_Object_e, 6, profname_str, NULL, other_heap_size, NULL, NULL);
        }
    } else {
        fapGm_dataMem::printfTag(fapGm_dataMem::TagAtt_Variable_e, fapGm_dataMem::TagType_Object_e, 0, profname_str, i_object, NULL, NULL, NULL);
    }

    return 1;
}

int dumpObject(void* i_object, void* i_data) {
    char* csv = (char*)i_data;
    csv += strlen(csv);

    if (fopAcM_IsActor(i_object)) {
        fopAc_ac_c* a_actor = (fopAc_ac_c*)i_object;

        s16 profname = fopAcM_GetProfName(a_actor);
        const char* actor_name = dStage_getName(profname, -1);
        u32 actor_size = a_actor->base.base.profile->process_size + (a_actor->heap == NULL ? 0 : a_actor->heap->getHeapSize());

        u32 other_heap_size = 0;
        if (profname == PROC_ALINK) {
            fopAc_ac_c* sp24 = a_actor;
            other_heap_size = daAlink_c::getOtherHeapSize();
        } else if (profname == PROC_MIDNA) {
            fopAc_ac_c* sp20 = a_actor;
            other_heap_size = daMidna_c::getOtherHeapSize();
        }

        sprintf(csv, ",%s(%d),%d,\n", actor_name != NULL ? actor_name : "???", profname, actor_size);

        if (other_heap_size != 0) {
            csv += strlen(csv);
            sprintf(csv, ",%s-Buffer(%d),%d,\n", actor_name != NULL ? actor_name : "???", profname, other_heap_size);
        }
    } else {
        s16 profname = fpcM_GetProfName(i_object);
        u32 process_size = ((base_process_class*)i_object)->profile->process_size;
        sprintf(csv, ",no actor(%d),%d,\n", profname, process_size);
    }

    return 1;
}

void fapGm_dataMem::printfTag(int i_att, int i_type, int i_heapType, const char* i_label, void* i_object, u32 i_otherHeapSize, const char* i_actorName, const char* param_7) {
    static const char* l_heapString[] = {
        "error",
        "aram",
        "root_heap",
        "system_heap",
        "zelda_heap",
        "archive_heap",
        "game_heap",
        "command_heap",
        "stage_heap",
        "j2d_heap",
        "dynamic_heap",
    };

    static const char* l_attString[] = {
        "fixed",
        "variable",
    };

    static const char* l_typeString[] = {
        "audio",
        "particle",
        "system",
        "unknown",
        "message",
        "2d",
        "player",
        "archive",
        "archive-sub",
        "archive-stage",
        "archive-stage-sub",
        "program",
        "object",
        "stage",
        NULL,
    };
    
    if (i_type < TagType_Heap_e) {
        if (i_object != NULL) {
            i_heapType = fapGm_dataMem::findParentHeap(i_object);
            if (i_otherHeapSize == 0) {
                i_otherHeapSize = JKRGetMemBlockSize(NULL, i_object);
            }
        }

        sprintf(fapGm_dataMem::getCsvEnd(), "%s\t%d\t%s\t%s", l_heapString[i_heapType], i_otherHeapSize, l_attString[i_att], l_typeString[i_type]);

        if (i_label != NULL) {
            sprintf(fapGm_dataMem::getCsvEnd(), "\t%s", i_label);
            if (i_actorName != NULL) {
                sprintf(fapGm_dataMem::getCsvEnd(), "\t%s", i_actorName);
                if (param_7 != NULL) {
                    sprintf(fapGm_dataMem::getCsvEnd(), "\t%s", param_7);
                }
            }
        }

        sprintf(fapGm_dataMem::getCsvEnd(), "\n");
    } else if (i_type == TagType_Heap_e) {
        u32 heap_size;
        int is_product = 0;
        u32 total_free_size;
        if (i_heapType == HeapType_ARAM_e) {
            heap_size = 0x1000000;
            total_free_size = JKRGetAramTotalFreeSize();
            is_product = 1;
        } else if (i_heapType == HeapType_Stage_e) {
            heap_size = i_otherHeapSize;
            total_free_size = 0;
        } else {
            JKRHeap* heap = (JKRHeap*)i_object;
            heap_size = heap->getHeapSize();
            total_free_size = heap->getTotalFreeSize();

            if (i_heapType == HeapType_Archive_e) {
                u32 temp_r24 = heap_size - mDoMain::archiveHeapSize;
                heap_size -= temp_r24;
                total_free_size -= temp_r24;
                is_product = 1;
            } else if (i_heapType == HeapType_Game_e) {
                u32 temp_r23 = heap_size - mDoMain::gameHeapSize;
                heap_size -= temp_r23;
                total_free_size -= temp_r23;
                is_product = 1;
            } else if (i_heapType == HeapType_Command_e) {
                is_product = 1;
            } else if (i_heapType == HeapType_J2D_e) {
                is_product = 1;
            } else if (i_heapType == HeapType_Dynamic_e) {
                is_product = 1;
            }
        }

        static const char* l_heapAllString[] = {
            "all",
            "product-all",
        };

        static const char* l_heapRestString[] = {
            "rest",
            "product-rest",
        };

        sprintf(fapGm_dataMem::getCsvEnd(), "%s\t%d\t%s\n", l_heapString[i_heapType], heap_size, l_heapAllString[is_product]);
        sprintf(fapGm_dataMem::getCsvEnd(), "%s\t%d\t%s\n", l_heapString[i_heapType], total_free_size, l_heapRestString[is_product]);
    }
}

int fapGm_dataMem::findParentHeap(void* i_object) {
    JKRHeap* heap = JKRHeap::findFromRoot(i_object);
    if (heap == NULL) {
        return NULL;
    }

    if (heap == heap->getRootHeap()) {
        return HeapType_Root_e;
    } else if (heap == JKRGetSystemHeap()) {
        return HeapType_System_e;
    } else if (heap == mDoExt_getGameHeap()) {
        return HeapType_Game_e;
    } else if (heap == mDoExt_getArchiveHeap()) {
        return HeapType_Archive_e;
    } else if (heap == mDoExt_getJ2dHeap()) {
        return HeapType_J2D_e;
    #if PLATFORM_WII || PLATFORM_SHIELD
    } else if (heap == DynamicModuleControlBase::getHeap()) {
        return HeapType_Dynamic_e;
    #endif
    } else if (heap == mDoExt_getZeldaHeap()) {
        return HeapType_Zelda_e;
    }

    for (int i = 0; i < 19; i++) {
        if (heap == dStage_roomControl_c::getMemoryBlockHeap(i)) {
            return HeapType_Stage_e;
        }
    }

    return findParentHeap(heap);
}

void fapGm_dataMem::dumpTag() {
    *mCsv = 0;

    printfTag(TagAtt_Variable_e, TagType_Heap_e, HeapType_Root_e, NULL, JKRHeap::getRootHeap(), 0, NULL, NULL);
    printfTag(TagAtt_Variable_e, TagType_Heap_e, HeapType_System_e, NULL, JKRGetSystemHeap(), 0, NULL, NULL);
    printfTag(TagAtt_Variable_e, TagType_Heap_e, HeapType_Zelda_e, NULL, mDoExt_getZeldaHeap(), 0, NULL, NULL);
    printfTag(TagAtt_Fixed_e, TagType_Heap_e, HeapType_Archive_e, NULL, mDoExt_getArchiveHeap(), 0, NULL, NULL);
    printfTag(TagAtt_Fixed_e, TagType_Heap_e, HeapType_J2D_e, NULL, mDoExt_getJ2dHeap(), 0, NULL, NULL);
    #if PLATFORM_WII || PLATFORM_SHIELD
    printfTag(TagAtt_Fixed_e, TagType_Heap_e, HeapType_Dynamic_e, NULL, DynamicModuleControlBase::getHeap(), 0, NULL, NULL);
    #endif
    printfTag(TagAtt_Fixed_e, TagType_Heap_e, HeapType_Game_e, NULL, mDoExt_getGameHeap(), 0, NULL, NULL);
    printfTag(TagAtt_Fixed_e, TagType_Heap_e, HeapType_Command_e, NULL, mDoExt_getCommandHeap(), 0, NULL, NULL);
    printfTag(TagAtt_Fixed_e, TagType_Heap_e, HeapType_ARAM_e, NULL, NULL, 0, NULL, NULL);
    printfTag(TagAtt_Fixed_e, TagType_Audio_e, 0, "オーディオ", g_mDoAud_audioHeap, 0x169000, NULL, NULL);
    printfTag(TagAtt_Fixed_e, TagType_Particle_e, 0, "パーティクルアーカイブ", g_dComIfG_gameInfo.play.getParticle()->getResHeap(), 0, NULL, NULL);
    printfTag(TagAtt_Fixed_e, TagType_Particle_e, 0, "パーティクル(常駐)", g_dComIfG_gameInfo.play.getParticle()->getHeap(), 0, NULL, NULL);
    printfTag(TagAtt_Variable_e, TagType_Particle_e, 0, "パーティクル(ステージ)", g_dComIfG_gameInfo.play.getParticle()->getSceneHeap(), 0, NULL, NULL);
    printfTag(TagAtt_Fixed_e, TagType_System_e, HeapType_System_e, "グラフィックFIFO", NULL, 0xA0000, 0, NULL);
    #if PLATFORM_WII || PLATFORM_SHIELD
    printfTag(TagAtt_Fixed_e, TagType_System_e, 0, "グラフィック関係バッファ", mDoGph_gInf_c::getHeap(), 0, NULL, NULL);
    #endif

    if (mDoGph_gInf_c::getBloom()->getBuffer() != NULL) {
        printfTag(TagAtt_Variable_e, TagType_System_e, 0, "飽和加算用バッファ", mDoGph_gInf_c::getBloom()->getBuffer(), NULL, 0, NULL);
    }

    if (dComIfGp_getSimpleModel() != NULL) {
        printfTag(TagAtt_Fixed_e, TagType_System_e, 0, "シンプルモデルバッファ", dComIfGp_getSimpleModel(), NULL, 0, NULL);
    }

    if (dMdl_mng_c::m_myObj != NULL) {
        printfTag(TagAtt_Fixed_e, TagType_System_e, 0, "節約モデルバッファ", dMdl_mng_c::m_myObj, NULL, 0, NULL);
    }

    if (daGrass_c::getGrass() != NULL) {
        printfTag(TagAtt_Variable_e, TagType_System_e, 0, "草バッファ", daGrass_c::getGrass(), NULL, 0, NULL);
    }

    if (daGrass_c::getFlower() != NULL) {
        printfTag(TagAtt_Variable_e, TagType_System_e, 0, "花バッファ", daGrass_c::getFlower(), NULL, 0, NULL);
    }

    if (dTres_c::getTypeGroupData() != NULL) {
        printfTag(TagAtt_Fixed_e, TagType_System_e, 0, "常駐宝箱バッファ", (char*)dTres_c::getTypeGroupData() - 16, NULL, 0, NULL);
    }

    if (dComIfGp_getAttention()->getHeap() != NULL) {
        printfTag(TagAtt_Fixed_e, TagType_System_e, 0, "注目処理用バッファ", dComIfGp_getAttention()->getHeap(), NULL, 0, NULL);
    }

    printfTag(TagAtt_Fixed_e, TagType_2D_e, 0, "2D転送用", dComIfGp_getExpHeap2D(), JKRGetMemBlockSize(NULL, dComIfGp_getExpHeap2D()) + JKRGetMemBlockSize(NULL, dComIfGp_getMsgExpHeap()), 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_Message_e, 0, "メッセージデータ(常駐)", ((JKRAramArchive*)dComIfGp_getMsgDtArchive())->mBlock, NULL, 0, NULL);
    printfTag(TagAtt_Variable_e, TagType_Message_e, 0, "メッセージデータ(ステージ)", ((JKRAramArchive*)dComIfGp_getMsgDtArchive(1))->mBlock, NULL, 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_2D_e, 0, "メーター関係", ((JKRAramArchive*)dComIfGp_getMain2DArchive())->mBlock, NULL, 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_System_e, 0, "アイテム出現テーブル", dComIfGp_getItemTable(), NULL, 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_Message_e, HeapType_Zelda_e, "キャッシュフォントバッファ", NULL, JKRGetMemBlockSize(NULL, ((JKRAramArchive*)dComIfGp_getRubyArchive())->mBlock) + 0x1C328, 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_Audio_e, HeapType_ARAM_e, "オーディオ", NULL, 0xB00000, 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_Player_e, HeapType_ARAM_e, "リンクアニメーション", NULL, ((JKRAramArchive*)dComIfGp_getAnmArchive())->mBlock->getSize(), 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_2D_e, HeapType_ARAM_e, "フィールドマップレイアウト", NULL, ((JKRAramArchive*)dComIfGp_getFmapResArchive())->mBlock->getSize(), 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_2D_e, HeapType_ARAM_e, "フィールドマップデータ", NULL, ((JKRAramArchive*)dComIfGp_getFieldMapArchive2())->mBlock->getSize(), 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_2D_e, HeapType_ARAM_e, "ダンジョンマップレイアウト", NULL, ((JKRAramArchive*)dComIfGp_getDmapResArchive())->mBlock->getSize(), 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_2D_e, HeapType_ARAM_e, "コレクト&セーブ&オプション", NULL, ((JKRAramArchive*)dComIfGp_getCollectResArchive())->mBlock->getSize(), 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_2D_e, HeapType_ARAM_e, "リングアイテム選択", NULL, ((JKRAramArchive*)dComIfGp_getRingResArchive())->mBlock->getSize(), 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_2D_e, HeapType_ARAM_e, "アイテムアイコン", NULL, ((JKRAramArchive*)dComIfGp_getItemIconArchive())->mBlock->getSize(), 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_2D_e, HeapType_ARAM_e, "アイテム説明画面", NULL, ((JKRAramArchive*)dComIfGp_getDemoMsgArchive())->mBlock->getSize(), 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_2D_e, HeapType_ARAM_e, "強調ボタン表示", NULL, ((JKRAramArchive*)dComIfGp_getMeterButtonArchive())->mBlock->getSize(), 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_Message_e, HeapType_ARAM_e, "フキダシ共通2Dデータ", NULL, ((JKRAramArchive*)dComIfGp_getMsgCommonArchive())->mBlock->getSize(), 0, NULL);
    printfTag(TagAtt_Fixed_e, TagType_Message_e, HeapType_ARAM_e, "キャッシュフォント", NULL, mDoExt_getAraCacheSize(), 0, NULL);

    for (int i = 0; i < 6; i++) {
        char label[64];
        sprintf(label, "メッセージ(%d)枠ほか", i);
        printfTag(TagAtt_Fixed_e, TagType_Message_e, HeapType_ARAM_e, label, NULL, ((JKRAramArchive*)dComIfGp_getMsgArchive(i))->mBlock->getSize(), 0, NULL);
    }

    g_dComIfG_gameInfo.mResControl.dumpTag();
    u32 var_r28 = 0;

    for (int i = 0; i < 19; i++) {
        JKRExpHeap* memblock = dStage_roomControl_c::getMemoryBlockHeap(i);
        if (memblock != NULL) {
            var_r28 += memblock->getHeapSize();
        }
    }

    if (var_r28 != NULL) {
        printfTag(TagAtt_Variable_e, TagType_Stage_e, 0, "ステージメモリーブロック", dStage_roomControl_c::getMemoryBlockHeap(0), var_r28, 0, NULL);
        printfTag(TagAtt_Variable_e, TagType_Heap_e, HeapType_Stage_e, NULL, NULL, var_r28, 0, NULL);
    }

    DynamicModuleControlBase::dumpTag();
    fpcLyIt_All(dumpTagObject, mCsv);
}

void fapGm_dataMem::dumpCsv() {
    sprintf(mCsv, "メインメモリ,,,\n");
    sprintf(getCsvEnd(), ",,,最大サイズ,空きサイズ\n");
    sprintf(getCsvEnd(), ",システムヒープ,%d,%d,%d,\n\n", ((JKRExpHeap*)JKRGetSystemHeap())->getTotalUsedSize(), JKRGetSystemHeap()->getHeapSize(), JKRGetSystemHeap()->getFreeSize());
    sprintf(getCsvEnd(), ",ゼルダヒープ,%d,%d,%d,\n", mDoExt_getZeldaHeap()->getTotalUsedSize(), mDoExt_getZeldaHeap()->getHeapSize(), mDoExt_getZeldaHeap()->getFreeSize());
    sprintf(getCsvEnd(), ",オーディオメモリ,%d,\n", 0x169000);
    sprintf(getCsvEnd(), ",パーティクルメモリ（常駐）,%d,\n", g_dComIfG_gameInfo.play.getParticle()->getHeap()->getHeapSize());
    sprintf(getCsvEnd(), ",パーティクルメモリ（シーン依存）,%d,\n", g_dComIfG_gameInfo.play.getParticle()->getSceneHeap()->getHeapSize());
    sprintf(getCsvEnd(), ",グラフィックＦＩＦＯ,%d,\n", 0xA0000);
    sprintf(getCsvEnd(), ",フレームバッファ,%d,\n", mDoGph_gInf_c::getFrameBufferSize() * 2);
    sprintf(getCsvEnd(), ",フレームバッファテクスチャー,%d,\n", 0x70820);
    sprintf(getCsvEnd(), ",２Ｄ転送用ヒープ,%d,\n", dComIfGp_getExpHeap2D()->getHeapSize() + dComIfGp_getMsgExpHeap()->getHeapSize());
    sprintf(getCsvEnd(), ",アーカイブヒープ,%d,%d,%d,\n\n", mDoExt_getArchiveHeap()->getTotalUsedSize(), mDoExt_getArchiveHeap()->getHeapSize(), mDoExt_getArchiveHeap()->getFreeSize());
    sprintf(getCsvEnd(), ",メッセージデータ（常駐）,%d,\n", JKRGetMemBlockSize(NULL, ((JKRAramArchive*)dComIfGp_getMsgDtArchive())->mBlock));
    sprintf(getCsvEnd(), ",メッセージデータ（ステージ読み）,%d,\n", JKRGetMemBlockSize(NULL, ((JKRAramArchive*)dComIfGp_getMsgDtArchive(1))->mBlock));
    sprintf(getCsvEnd(), ",メーター関係,%d,\n", JKRGetMemBlockSize(NULL, ((JKRAramArchive*)dComIfGp_getMain2DArchive())->mBlock));
    sprintf(getCsvEnd(), ",アイテム出現テーブル,%d,\n", JKRGetMemBlockSize(NULL, dComIfGp_getItemTable()));
    sprintf(getCsvEnd(), ",キャッシュフォントバッファ,%d,\n", JKRGetMemBlockSize(NULL, ((JKRAramArchive*)dComIfGp_getRubyArchive())->mBlock) + 0x1C328);
    sprintf(getCsvEnd(), ",ゲームヒープ,%d,%d,%d,\n", mDoExt_getGameHeap()->getTotalUsedSize(), mDoExt_getGameHeap()->getHeapSize(), mDoExt_getGameHeap()->getFreeSize());
    sprintf(getCsvEnd(), ",コマンドヒープ,%d,%d,%d,\n\n", mDoExt_getCommandHeap()->getTotalUsedSize(), mDoExt_getCommandHeap()->getHeapSize(), mDoExt_getCommandHeap()->getFreeSize());
    sprintf(getCsvEnd(), ",ＡＲＡＭメモリ,%d,%d,%d,\n\n", JKRGetAramUsedSize(0) + 0xB00000, (JKRGetAramUsedSize(0) + 0xB00000 + JKRGetAramTotalFreeSize()), JKRGetAramFreeSize());
    sprintf(getCsvEnd(), ",オーディオＡＲＡＭメモリ,%d,\n", 0xB00000);
    sprintf(getCsvEnd(), ",リンクアニメーション,%d,\n", ((JKRAramArchive*)dComIfGp_getAnmArchive())->mBlock->getSize());
    sprintf(getCsvEnd(), ",フィールドマップレイアウト,%d,\n", ((JKRAramArchive*)dComIfGp_getFmapResArchive())->mBlock->getSize());
    sprintf(getCsvEnd(), ",フィールドマップデータ,%d,\n", ((JKRAramArchive*)dComIfGp_getFieldMapArchive2())->mBlock->getSize());
    sprintf(getCsvEnd(), ",ダンジョンマップレイアウト,%d,\n", ((JKRAramArchive*)dComIfGp_getDmapResArchive())->mBlock->getSize());
    sprintf(getCsvEnd(), ",コレクト＆セーブ＆オプション,%d,\n", ((JKRAramArchive*)dComIfGp_getCollectResArchive())->mBlock->getSize());
    sprintf(getCsvEnd(), ",リングアイテム選択,%d,\n", ((JKRAramArchive*)dComIfGp_getRingResArchive())->mBlock->getSize());
    sprintf(getCsvEnd(), ",アイテムアイコン,%d,\n", ((JKRAramArchive*)dComIfGp_getItemIconArchive())->mBlock->getSize());
    sprintf(getCsvEnd(), ",アイテム説明画面,%d,\n", ((JKRAramArchive*)dComIfGp_getDemoMsgArchive())->mBlock->getSize());
    sprintf(getCsvEnd(), ",強調ボタン表示,%d,\n", ((JKRAramArchive*)dComIfGp_getMeterButtonArchive())->mBlock->getSize());
    sprintf(getCsvEnd(), ",フキダシ共通２Ｄデータ,%d,\n", ((JKRAramArchive*)dComIfGp_getMsgCommonArchive())->mBlock->getSize());
    sprintf(getCsvEnd(), ",キャッシュフォント,%d,\n\n", mDoExt_getAraCacheSize());

    for (int i = 0; i < 6; i++) {
        sprintf(getCsvEnd(), ",メッセージ(%d)枠ほか,%d,\n", i, ((JKRAramArchive*)dComIfGp_getMsgArchive(i))->mBlock->getSize());
    }

    g_dComIfG_gameInfo.mResControl.dump(mCsv);

    u32 memblock_total_size = 0;
    for (int i = 0; i < 19; i++) {
        JKRExpHeap* memblock = dStage_roomControl_c::getMemoryBlockHeap(i);
        if (memblock != NULL) {
            memblock_total_size += memblock->getHeapSize();
        }
    }

    if (memblock_total_size != NULL) {
        sprintf(getCsvEnd(), ",ステージメモリーブロック,%d\n\n", memblock_total_size);
    }

    DynamicModuleControlBase::dump(mCsv);

    sprintf(getCsvEnd(), ",オブジェクト名（ＩＤ）,サイズ,\n");
    fpcLyIt_All(dumpObject, mCsv);
    sprintf(getCsvEnd(), ",２Ｄメモリ,%d,\n", dComIfGp_getExpHeap2D()->getHeapSize() + dComIfGp_getMsgExpHeap()->getHeapSize());
}

void fapGm_HIO_c::listenPropertyEvent(const JORPropertyEvent* property) {
    JORReflexible::listenPropertyEvent(property);
    JORFile file;

    switch ((u32)property->id) {
    case 0x4000020:
        JUTAssertion::setMessageCount(0);
        break;
    case 0x4000003:
        mCaptureScreenFlag = -1;
        break;
    case 0x4000002:
        if (file.open(6, "ゲームキューブイメージファイル(*.gfb)", "gfb", NULL, NULL)) {
            file.writeData(mDoGph_gInf_c::getFrameBufferMemory(), mDoGph_gInf_c::getFrameBufferSize());
            file.close();
        }
        break;
    case 0x4000005:
        OSReport("\x1b[43;30m連続空き=%fK 残り空き=%fK\n\x1b[m", JKRGetSystemHeap()->getFreeSize() * 0.0009765625f, JKRGetSystemHeap()->getTotalFreeSize() * 0.0009765625f);
        break;
    case 0x4000007:
        onCpuTimer();
        break;
    case 0x4000014: {
        u8* bdlData;
        void* model_buffer = mDoExt_getArchiveHeap()->alloc(0x96000, 0x20);
        
        if (model_buffer == NULL) {
            OSReport("ＢＭＤファイル読み込みメモリがありません！！\n");
        } else if (!file.open(1, "ＢＭＤファイル(*.bmd)", NULL, NULL, NULL)) {
            OSReport("ＢＭＤファイルオープン失敗！！\n");
        } else {
            file.readData(model_buffer, 0);
            file.close();

            J3DModelData* modelData = J3DModelLoaderDataBase::load(model_buffer, 0x10000);
            if (modelData == NULL) {
                OSReport("ＢＭＤデータロード失敗！！\n");
            }

            JKRSolidHeap* heap = mDoExt_createSolidHeapFromGameToCurrent(0x4B000, 0x20);
            if (heap == NULL) {
                OSReport("Ｊ３ＤＭｏｄｅｌ用メモリがありません！！\n");
                JKRFree(model_buffer);
                break;
            }

            J3DModel* model = mDoExt_J3DModel__create(modelData, 0x10000, 0x11020203);
            mDoExt_restoreCurrentHeap();
            mDoExt_adjustSolidHeap(heap);

            if (model == NULL) {
                OSReport("Ｊ３ＤＭｏｄｅｌ生成失敗！！\n");
                mDoExt_destroySolidHeap(heap);
                JKRFree(model_buffer);
                break;
            }

            j3dSys.setViewMtx(cMtx_getIdentity());

            model->calc();
            model->calcMaterial();
            model->makeDL();
            model->calcMaterial();
            model->makeDL();
            model->lock();

            JORFile out_file;
            int temp_r24 = strlen(file.getFilename());
            char* filename_and_ext = file.getFilename() + temp_r24;

            int sp10 = 0;
            while (*filename_and_ext != '\\') {
                filename_and_ext--;
                sp10++;
            }

            filename_and_ext++;

            char new_filename[32];
            for (int i = 0; i < sp10; i++) {
                new_filename[i] = filename_and_ext[i];
            }

            new_filename[sp10 - 3] = 'd';
            new_filename[sp10 - 2] = 'l';

            if (!out_file.open(NULL, 6, "ＢＤＬファイル(*.bdl)", "bdl", NULL, new_filename)) {
                OSReport("ＢＤＬファイルオープン失敗！！\n");
            } else {
                bdlData = (u8*)J3DModelSaverDataBase::saveBinaryDisplayList(model, J3DBinaryDisplayListSaverFlag_UNK_1, 0xC8000);
                if (bdlData == NULL) {
                    OSReport("ＢＤＬファイル生成失敗！！\n");
                } else {
                    OSReport("############ bdlData=%x size=%d\n", bdlData, *(int*)((char*)bdlData + 8));
                    out_file.writeData(bdlData, *(int*)((char*)bdlData + 8));
                    out_file.close();
                }

                mDoExt_destroySolidHeap(heap);
                JKRFree(model_buffer);
                delete[] bdlData;
            }
        }   
        break;
    }
    case 0x4000016:
        if (!file.open(NULL, 6, "メモリファイル(*.csv)", "csv", NULL, NULL)) {
            OSReport("メモリファイルオープン失敗！！\n");
        } else {
            fapGm_dataMem::dumpCsv();
            file.writeData(fapGm_dataMem::getCsv(), fapGm_dataMem::getCsvSize());
            file.close();
            OSReport("csv file size=%d\n", fapGm_dataMem::getCsvSize());
        }

        if (!file.open(NULL, 6, "新メモリファイル(*.tag)", "tag", NULL, NULL)) {
            OSReport("新メモリファイルオープン失敗！！\n");
        } else {
            fapGm_dataMem::dumpTag();
            file.writeData(fapGm_dataMem::getCsv(), fapGm_dataMem::getCsvSize());
            file.close();
            OSReport("tag file size=%d\n", fapGm_dataMem::getCsvSize());
        }
        break;
    }
}

void fapGm_HIO_c::startCpuTimer() {
    if (m_CpuTimerOn) {
        m_CpuTimerOff = TRUE;
        m_CpuTimerOn = FALSE;
    }

    if (m_CpuTimerOff) {
        int stackDummy;
        JUT_ASSERT(1302, !m_CpuTimerStart)
        m_CpuTimerStart = TRUE;
        m_CpuTimerTick = OSGetTick();
    }
}

void fapGm_HIO_c::stopCpuTimer(const char* message) {
    if (m_CpuTimerOff) {
        JUT_ASSERT(1311, m_CpuTimerStart)
        m_CpuTimerStart = FALSE;

        u32 var_r29 = OSGetTick() - m_CpuTimerTick;
        OSReport("%fms : <%s>\n", OSTicksToMicroseconds(var_r29) / 1000.0f, message);
    }
}

void fapGm_HIO_c::printCpuTimer(const char* message) {
    if (m_CpuTimerOff) {
        OSReport("%s\n", message);
    }
}
#endif

void fapGm_After() {
    fopScnM_Management();
    fopOvlpM_Management();
    fopCamM_Management();
}

void fapGm_Execute() {
    #if DEBUG
    JUTDbPrint::getManager()->setCharColor(g_HIO.mColor);
    #endif

    fpcM_Management(NULL, fapGm_After);
    cCt_Counter(0);
}

fapGm_HIO_c g_HIO;

void fapGm_Create() {
    // unused, unknown purpose
    u16 var_r30 = 184;
    u16 var_r29 = 432;
    u16 var_r28 = 1392;
    u16 var_r27 = 456;

    fpcM_Init();
    fopScnM_Init();
    fopOvlpM_Init();
    fopCamM_Init();
    fopDwTg_CreateQueue();
    
    #if DEBUG
    fapGm_HIO_c::createCaptureScreen();
    #endif
    g_HIO.field_0x04 = mDoHIO_CREATE_CHILD("ゲームシステム", &g_HIO);
}
