#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAIAudience.h"
#include "JSystem/JAudio2/JAISeMgr.h"
#include "JSystem/JAudio2/JAISeqMgr.h"
#include "JSystem/JAudio2/JAIStreamMgr.h"

JAIAudience::~JAIAudience() {
    JUT_ASSERT(14, ! JAISeMgr::getInstance() || ( JAISeMgr::getInstance() ->getAudience() != this ));
    JUT_ASSERT(15, ! JAISeqMgr::getInstance() || ( JAISeqMgr::getInstance() ->getAudience() != this ));
    JUT_ASSERT(16, ! JAIStreamMgr::getInstance() || ( JAIStreamMgr::getInstance() ->getAudience() != this ));
}
