#ifndef D_D_RESORCE_H
#define D_D_RESORCE_H

#include "m_Do/m_Do_dvd_thread.h"
#include "m_Do/m_Do_ext.h"

class JKRArchive;
class JKRHeap;
class JKRSolidHeap;

class dRes_info_c {
public:
    dRes_info_c();
    ~dRes_info_c();

    int set(char const* i_arcName, char const* i_path, u8 i_mountDirection, JKRHeap* i_heap);
    int loadResource();
    void deleteArchiveRes();
    int setRes(JKRArchive* i_archive, JKRHeap* i_heap);
    int setRes();

    static void onWarpMaterial(J3DModelData* i_modelData);
    static void offWarpMaterial(J3DModelData* i_modelData);
    static void setWarpSRT(J3DModelData* i_modelData, const cXyz& i_pos, f32 i_transX, f32 i_transY);
    static J3DModelData* loaderBasicBmd(u32 i_tag, void* i_data);
    static void dump_long(dRes_info_c* i_resInfo, int i_infoNum);
    static void dump(dRes_info_c* i_resInfo, int i_infoNum);

    void* getRes(s32 i_index) {
        JUT_ASSERT(25, i_index >= 0 && i_index < getResNum());
        return *(mRes + i_index);
    }

    s32 getResNum() { return mArchive->countFile(); }

    int getCount() { return mCount; }
    char* getArchiveName() { return mArchiveName; }
    mDoDvdThd_mountArchive_c* getDMCommand() { return mDMCommand; }
    JKRArchive* getArchive() { return mArchive; }
    void incCount() { mCount++; }
    u16 decCount() {
        return --mCount;
    }

    static const int NAME_MAX = 9;

private:
    /* 0x00 */ char mArchiveName[11];
    /* 0x0C */ u16 mCount;
    /* 0x10 */ mDoDvdThd_mountArchive_c* mDMCommand;
    /* 0x14 */ JKRArchive* mArchive;
    /* 0x18 */ JKRHeap* heap;
    /* 0x1C */ JKRSolidHeap* mDataHeap;
    /* 0x20 */ void** mRes;
#if DEBUG
    /* 0x24 */ void* unk_0x24;
#endif
};  // Size: 0x24

STATIC_ASSERT(sizeof(dRes_info_c) == 0x24);

class dRes_control_c {
public:
    dRes_control_c() {}
    ~dRes_control_c();

    int setObjectRes(char const* i_arcName, void* i_archiveRes, u32 i_bufferSize, JKRHeap* i_heap);
    int setStageRes(char const* i_arcName, JKRHeap* i_heap);
    void dump();
    void dump(char*);
    int getObjectResName2Index(char const* i_arcName, char const* i_resName);

    static int setRes(char const* i_arcName, dRes_info_c* i_resInfo, int i_infoNum, char const* i_path, u8 i_mountDirection, JKRHeap* i_heap);
    static int syncRes(char const* i_arcName, dRes_info_c* i_resInfo, int i_infoNum);
    static int deleteRes(char const* i_arcName, dRes_info_c* i_resInfo, int i_infoNum);
    static void* getRes(char const* i_arcName, char const* i_resName, dRes_info_c* i_resInfo, int i_infoNum);
    static dRes_info_c* getResInfo(char const* i_arcName, dRes_info_c* i_resInfo, int i_infoNum);
    static dRes_info_c* newResInfo(dRes_info_c* i_resInfo, int i_infoNum);
    static dRes_info_c* getResInfoLoaded(char const* i_arcName, dRes_info_c* i_resInfo, int i_infoNum);
    static void* getRes(char const* i_arcName, s32 i_index, dRes_info_c* i_resInfo, int i_infoNum);
    static void* getIDRes(char const* i_arcName, u16 i_resID, dRes_info_c* i_resInfo, int i_infoNum);
    static int syncAllRes(dRes_info_c* i_resInfo, int i_infoNum);

    int setObjectRes(const char* i_arcName, u8 i_mountDirection, JKRHeap* i_heap) {
        return setRes(i_arcName, mObjectInfo, ARRAY_SIZEU(mObjectInfo), "/res/Object/", i_mountDirection,
                      i_heap);
    }

    void* getObjectRes(const char* i_arcName, const char* resName) {
        return getRes(i_arcName, resName, mObjectInfo, ARRAY_SIZEU(mObjectInfo));
    }

    void* getObjectRes(const char* i_arcName, s32 i_index) {
        return getRes(i_arcName, i_index, mObjectInfo, ARRAY_SIZEU(mObjectInfo));
    }

    void* getObjectIDRes(const char* i_arcName, u16 i_resID) {
        return getIDRes(i_arcName, i_resID, mObjectInfo, ARRAY_SIZEU(mObjectInfo));
    }

    int syncObjectRes(const char* i_arcName) {
        return syncRes(i_arcName, mObjectInfo, ARRAY_SIZEU(mObjectInfo));
    }

    int syncStageRes(const char* i_arcName) {
        return syncRes(i_arcName, mStageInfo, ARRAY_SIZEU(mStageInfo));
    }

    int syncAllObjectRes() { return syncAllRes(mObjectInfo, ARRAY_SIZEU(mObjectInfo)); }

    int deleteObjectRes(const char* i_arcName) {
        return deleteRes(i_arcName, mObjectInfo, ARRAY_SIZEU(mObjectInfo));
    }

    int deleteStageRes(const char* i_arcName) {
        return deleteRes(i_arcName, mStageInfo, ARRAY_SIZEU(mStageInfo));
    }

    void* getStageRes(const char* i_arcName, const char* i_resName) {
        return getRes(i_arcName, i_resName, mStageInfo, ARRAY_SIZEU(mStageInfo));
    }

    dRes_info_c* getObjectResInfo(const char* i_arcName) {
        return getResInfo(i_arcName, mObjectInfo, ARRAY_SIZEU(mObjectInfo));
    }

    dRes_info_c* getStageResInfo(const char* i_arcName) {
        return getResInfo(i_arcName, mStageInfo, ARRAY_SIZEU(mStageInfo));
    }
    
    #if DEBUG
    void dumpTag() {
        // TODO
    }
    #endif

    /* 0x0000 */ dRes_info_c mObjectInfo[128];
    /* 0x1200 */ dRes_info_c mStageInfo[64];
};  // Size: 0x1B00

#endif /* D_D_RESORCE_H */
