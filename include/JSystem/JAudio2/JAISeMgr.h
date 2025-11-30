#ifndef JAISEMGR_H
#define JAISEMGR_H

#include "JSystem/JAudio2/JAISe.h"
#include "global.h"

struct JAIAudience;
struct JASSoundParams;

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASNonCopyable {
    JASNonCopyable() {}

    /* 0x0 */ int field_0x0;
};  // Size: 0x4

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISeCategoryArrangementItem {
    u8 mMaxActiveSe;
    u8 mMaxInactiveSe;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISeCategoryArrangement {
    JAISeCategoryArrangementItem mItems[16];
};

class JAISeMgr;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAISeCategoryMgr : public JAISeqDataUser, public JASNonCopyable {
public:
    void JAISeMgr_calc_();
    void JAISeMgr_freeDeadSe_();
    bool JAISeMgr_acceptsNewSe_(u32 priority) const;
    void sortByPriority_();
    void stop(u32 fadeTime);
    void stop();
    void stopSoundID(JAISoundID id);
    void pause(bool);
    void JAISeMgr_mixOut_(const JAISoundParamsMove& params, JAISoundActivity activity);
    JAISeCategoryMgr() {
        mParams.init();
        mMaxActiveSe = 0;
        mMaxInactiveSe = 0;
        field_0x0 = 0;
    }

    virtual bool isUsingSeqData(const JAISeqDataRegion& seqDataRegion);
    virtual int releaseSeqData(const JAISeqDataRegion& seqDataRegion);

    JAISoundParamsMove* getParams() { return &mParams; }
    int getMaxSe() const {
        if (mMaxActiveSe == 0) {
            return 0;
        }

        return mMaxActiveSe + mMaxInactiveSe;
    }
    int getMaxActiveSe() const { return mMaxActiveSe; }
    void setMaxActiveSe(int num) { mMaxActiveSe = num; }
    void setMaxInactiveSe(int num) { mMaxInactiveSe = num; }
    const JSUList<JAISe>* getSeList() const { return &mSeList; }
    int getNumSe() const { return mSeList.getNumLinks(); }
    JAIAudience* getAudience() { return (JAIAudience*)field_0x0; }
    void JAISeMgr_appendSe_(JAISe* se) { mSeList.append(se); }

    /* 0x08 */ JAISoundParamsMove mParams;
    /* 0x58 */ JSUList<JAISe> mSeList;
    /* 0x64 */ int mMaxInactiveSe;
    /* 0x68 */ int mMaxActiveSe;
};  // Size: 0x6C

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAISeMgr : public JASGlobalInstance<JAISeMgr>,
                 public JAISeqDataUser,
                 public JAISoundActivity {
public:
    JAISeMgr(bool setInstance);
    void setCategoryArrangement(const JAISeCategoryArrangement& arrangement);
    void stop();
    void stopSoundID(JAISoundID id);
    void initParams();
    void setAudience(JAIAudience* audience);
    void setSeqDataMgr(JAISeqDataMgr* seqDataMgr);
    void resetSeqDataMgr();
    JAISe* newSe_(int category, u32 priority);
    void calc();
    void mixOut();
    bool startSound(JAISoundID id, JAISoundHandle* handle, const JGeometry::TVec3<f32>* posPtr);
    int getNumActiveSe() const;

    virtual bool isUsingSeqData(const JAISeqDataRegion& seqDataRegion);
    virtual int releaseSeqData(const JAISeqDataRegion& seqDataRegion);

    JAISeCategoryMgr* getCategory(int index) { return &mCategoryMgrs[index]; }
    JAIAudience* getAudience() { return mAudience; }
    JAIAudience* getAudience(int index) {
        if (index >= 0 && index < NUM_CATEGORIES) {
            JAIAudience* rv = mCategoryMgrs[index].getAudience();
            if (rv) return rv;
        }
        return mAudience;
    }
    JAISeqDataMgr* getSeqDataMgr() { return mSeqDataMgr; }
    JAISoundParamsMove* getParams() { return &mParams; }

    bool isActive() const { return getNumActiveSe() > 0; }

    static const int NUM_CATEGORIES = 16;

private:
    /* 0x008 */ JAIAudience* mAudience;
    /* 0x00C */ JAISeqDataMgr* mSeqDataMgr;
    /* 0x010 */ JAISoundStrategyMgr<JAISe>* mStrategyMgr;
    /* 0x014 */ JAISeCategoryMgr mCategoryMgrs[NUM_CATEGORIES];
    /* 0x6D4 */ JAISoundParamsMove mParams;
};  // Size: 0x724

#if VERSION != VERSION_SHIELD_DEBUG
STATIC_ASSERT(sizeof(JAISeMgr) == 0x724);
#endif

#endif /* JAISEMGR_H */
