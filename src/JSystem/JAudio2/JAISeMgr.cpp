#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAISeMgr.h"
#include "JSystem/JAudio2/JAISoundHandles.h"
#include "JSystem/JAudio2/JAISoundInfo.h"
#include "JSystem/JAudio2/JASReport.h"

bool JAISeCategoryMgr::isUsingSeqData(const JAISeqDataRegion& seqDataRegion) {
    {
        JSULink<JAISe>* i = mSeList.getFirst();
        while (i != NULL) {
            if (seqDataRegion.intersects(*i->getObject()->getSeqData())) {
                return true;
            }
            i = i->getNext();
        }
    }

    return false;
}

int JAISeCategoryMgr::releaseSeqData(const JAISeqDataRegion& seqDataRegion) {
    bool bVar1 = false;
    for (JSULink<JAISe>* i = mSeList.getFirst(); i != NULL; i = i->getNext()) {
        if (seqDataRegion.intersects(*i->getObject()->getSeqData())) {
            i->getObject()->stop();
            bVar1 = true;
        }
    }

    enum {_1 = 1, _2};  // fakematch, figure out what this enum is eventually
    return bVar1 ? _1 : _2;
}

void JAISeCategoryMgr::JAISeMgr_calc_() {
    mParams.calc();

    for (JSULink<JAISe>* i = mSeList.getFirst(); i != NULL; i = i->getNext()) {
        i->getObject()->JAISeCategoryMgr_calc_();
    }

    sortByPriority_();
}

void JAISeCategoryMgr::JAISeMgr_freeDeadSe_() {
    JSULink<JAISe>* i = mSeList.getFirst();
    while (i != NULL) {
        JSULink<JAISe>* link_next = i->getNext();
        if (i->getObject()->status_.isDead()) {
            JUT_ASSERT(71, ! i->getObject() ->isHandleAttached());
            mSeList.remove(i);
            delete i->getObject();
        }
        i = link_next;
    }
}

bool JAISeCategoryMgr::JAISeMgr_acceptsNewSe_(u32 priority) const {
    s32 maxSe = getMaxSe();
    if (maxSe <= 0) {
        return 1;
    } 

    s32 stopCount = 0;
    for (JSULink<JAISe>* i = mSeList.getFirst(); i != NULL; i = i->getNext()) {
        if (priority < i->getObject()->JAISeCategoryMgr_getProperPriority_()) {
            return 1;
        }
        if (!i->getObject()->isStopping()) {
            stopCount++;
        }
    }

    if (stopCount < maxSe) {
        return true;
    } else {
        return false;
    }
}

void JAISeCategoryMgr::sortByPriority_() {
    for (u32 i = mSeList.getNumLinks(); i != 0; i--) {
        JSULink<JAISe>* link = mSeList.getFirst();
        u32 lowestPriority = link->getObject()->JAISeCategoryMgr_getPriority_();
        JSULink<JAISe>* current = link->getNext();

        for (int j = 1; j < i; j++) {
            u32 itemPriority = current->getObject()->JAISeCategoryMgr_getPriority_();
            if (itemPriority < lowestPriority) {
                lowestPriority = itemPriority;
                link = current;
            }
            current = current->getNext();
        }

        bool result = mSeList.remove(link);
        JUT_ASSERT(131, result);

        result = mSeList.append(link);
        JUT_ASSERT(133, result);
    }
}

void JAISeCategoryMgr::stop(u32 fadeTime) {
    for (JSULink<JAISe>* i = mSeList.getFirst(); i != NULL; i = i->getNext()) {
        i->getObject()->stop(fadeTime);
    }
}

void JAISeCategoryMgr::stop() {
    for (JSULink<JAISe>* i = mSeList.getFirst(); i != NULL; i = i->getNext()) {
        i->getObject()->stop();
    }
}

void JAISeCategoryMgr::stopSoundID(JAISoundID id) {
    if (!id.isAnonymous()) {
        for (JSULink<JAISe>* i = mSeList.getFirst(); i != NULL; i = i->getNext()) {
            if ((u32)i->getObject()->getID() == (u32)id) {
                i->getObject()->stop();
            }
        }
    }
}

void JAISeCategoryMgr::pause(bool param_0) {
    for (JSULink<JAISe>* i = mSeList.getFirst(); i != NULL; i = i->getNext()) {
        i->getObject()->pause(param_0);
    }
}

void JAISeCategoryMgr::JAISeMgr_mixOut_(const JAISoundParamsMove& params,
                                        JAISoundActivity activity) {
    JASSoundParams mixParams;
    mixParams.combine(params.params_, mParams.params_);
    JSULink<JAISe>* i = mSeList.getFirst();
    int maxActiveSe = getMaxActiveSe();
    int sp28;
    
    if (maxActiveSe > 0) {
        while (i != NULL && maxActiveSe > 0 && !i->getObject()->isFarAway()) {
            i->getObject()->JAISeCategoryMgr_mixOut_(true, mixParams, activity);
            i = i->getNext();
            maxActiveSe--;
            sp28 = maxActiveSe;
        }
        for (; i != NULL; i = i->getNext()) {
            i->getObject()->JAISeCategoryMgr_mixOut_(false, mixParams, activity);
        }
    } else {
        while (i != NULL && !i->getObject()->isFarAway()) {
            i->getObject()->JAISeCategoryMgr_mixOut_(true, mixParams, activity);
            i = i->getNext();
        }
        for (; i != NULL; i = i->getNext()) {
            i->getObject()->JAISeCategoryMgr_mixOut_(false, mixParams, activity);
        }
    }
}

JAISeMgr::JAISeMgr(bool setInstance) : JASGlobalInstance<JAISeMgr>(setInstance) {
    mAudience = NULL;
    mSeqDataMgr = NULL;
    mStrategyMgr = NULL;
    mParams.init();
    mSoundActivity.init();
}

bool JAISeMgr::isUsingSeqData(const JAISeqDataRegion& seqDataRegion) {
    for (int i = 0; i < 16; i++) {
        if (mCategoryMgrs[i].JAISeCategoryMgr::isUsingSeqData(seqDataRegion)) {
            return true;
        }
    }
    return false;
}

int JAISeMgr::releaseSeqData(const JAISeqDataRegion& seqDataRegion) {
    bool r30 = 0;
    for (int i = 0; i < 16; i++) {
        switch (mCategoryMgrs[i].JAISeCategoryMgr::releaseSeqData(seqDataRegion)) {
        case 0:
            return 0;
        case 1:
            r30 = 1;
            break;
        }
    }

    return (r30 != 0) ? 1 : 0;
}

void JAISeMgr::setCategoryArrangement(const JAISeCategoryArrangement& arrangement) {
    for (int i = 0; i < 16; i++) {
        mCategoryMgrs[i].setMaxActiveSe(arrangement.mItems[i].mMaxActiveSe);
        mCategoryMgrs[i].setMaxInactiveSe(arrangement.mItems[i].mMaxInactiveSe);
    }
}

void JAISeMgr::getCategoryArrangement(JAISeCategoryArrangement* arrangement) {
    for (int i = 0; i < 16; i++) {
        int active = mCategoryMgrs[i].getMaxActiveSe();
        JUT_ASSERT(299, active <= 255);
        arrangement->mItems[i].mMaxActiveSe = active;
        int inactive = mCategoryMgrs[i].getMaxInactiveSe();
        JUT_ASSERT(302, inactive <= 255);
        arrangement->mItems[i].mMaxInactiveSe = inactive;
    }
}

void JAISeMgr::stop() {
    for (int i = 0; i < 16; i++) {
        mCategoryMgrs[i].stop();
    }
}

void JAISeMgr::stopSoundID(JAISoundID id) {
    for (int i = 0; i < 16; i++) {
        mCategoryMgrs[i].stopSoundID(id);
    }
}

void JAISeMgr::initParams() {
    for (int i = 0; i < 16; i++) {
        mCategoryMgrs[i].getParams()->init();
    }
}


void JAISeMgr::setAudience(JAIAudience* audience) {
    JUT_ASSERT(363, ! isActive());
    mAudience = audience;
}

void JAISeMgr::setSeqDataMgr(JAISeqDataMgr* seqDataMgr) {
    JUT_ASSERT(377, ! isActive());
    resetSeqDataMgr();
    mSeqDataMgr = seqDataMgr;
    mSeqDataMgr->setSeqDataUser(this);
}

void JAISeMgr::resetSeqDataMgr() {
    if (mSeqDataMgr) {
        mSeqDataMgr->setSeqDataUser(NULL);
        mSeqDataMgr = NULL;
    }
}

JAISe* JAISeMgr::newSe_(int category, u32 priority) {
    JUT_ASSERT(396, category < NUM_CATEGORIES);

    if (category < 0) {
        category = 0;
    }

    JAISeCategoryMgr* categoryMgr = &mCategoryMgrs[category];
    if (!categoryMgr->JAISeMgr_acceptsNewSe_(priority)) {
        return NULL;
    }

    JAISe* se = new JAISe(this, mStrategyMgr, priority);
    if (se == NULL) {
        JUT_WARN(410, "%s", "JASPoolAllocObject::<JAISe>::operator new failed .\n")
        return NULL;
    }

    categoryMgr->JAISeMgr_appendSe_(se);
    return se;
}

void JAISeMgr::calc() {
    mParams.calc();
    for (int i = 0; i < 16; i++) {
        mCategoryMgrs[i].JAISeMgr_calc_();
    }
    for (int i = 0; i < 16; i++) {
        mCategoryMgrs[i].JAISeMgr_freeDeadSe_();
    }
}

void JAISeMgr::mixOut() {
    for (int i = 0; i < 16; i++) {
        mCategoryMgrs[i].JAISeMgr_mixOut_(mParams, mSoundActivity);
    }
}

bool JAISeMgr::startSound(JAISoundID id, JAISoundHandle* handle, const JGeometry::TVec3<f32>* posPtr) {
    if (handle && *handle) {
        (*handle)->stop();
    }

    JAISoundInfo* soundInfoPtr = JASGlobalInstance<JAISoundInfo>::getInstance();
    u32 priority;
    u32 category;
    if (soundInfoPtr != NULL) {
        category = soundInfoPtr->getCategory(id);
        priority = soundInfoPtr->getPriority(id);
    } else {
        category = id.id_.info.type.parts.groupID;
        priority = 0;
    }

    JAISe* se = newSe_(category, priority);
    if (se == NULL) {
        #if !PLATFORM_SHIELD
        JASReport("cannot new Se %08x.", id.id_.composite_);
        #endif
        return false;
    } 

    JAIAudience* audiencePtr = getAudience(category);
    se->JAISeMgr_startID_(id, posPtr, audiencePtr);

    if (soundInfoPtr != NULL) {
        soundInfoPtr->getSeInfo(id, se);
    }

    if (handle != NULL) {
        se->attachHandle(handle);
    }

    return true;
}

int JAISeMgr::getNumActiveSe() const {
    int num = 0;
    for (int i = 0; i < 16; i++) {
        num += mCategoryMgrs[i].getNumSe();
    }
    return num;
}
