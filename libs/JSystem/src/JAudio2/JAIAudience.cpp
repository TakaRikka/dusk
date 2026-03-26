#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAIAudience.h"

#include "JSystem/JAudio2/JAISeMgr.h"
#include "JSystem/JAudio2/JAISeqMgr.h"
#include "JSystem/JAudio2/JAIStreamMgr.h"
#include "dusk/main.h"

JAIAudience::~JAIAudience() {
#if TARGET_PC
    if (dusk::IsShuttingDown) {
        // Those asserts down there crash on shutdown from dtors.
        return;
    }
#endif

    JUT_ASSERT(14, ! JAISeMgr::getInstance() || ( JAISeMgr::getInstance() ->getAudience() != this ));
    JUT_ASSERT(15, ! JAISeqMgr::getInstance() || ( JAISeqMgr::getInstance() ->getAudience() != this ));
    JUT_ASSERT(16, ! JAIStreamMgr::getInstance() || ( JAIStreamMgr::getInstance() ->getAudience() != this ));
}
