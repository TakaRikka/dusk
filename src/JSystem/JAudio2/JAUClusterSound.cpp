//
// JAUClusterSound
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAUClusterSound.h"
#include "JSystem/JAudio2/JAISoundHandles.h"
#include "JSystem/JAudio2/JAISoundStarter.h"

// Dummy function to generate JAISoundHandle::JAISoundHandle
static void dummyCtor() {
    JASGlobalInstance<JAISoundStarter>::getInstance();
    JAISoundHandle dummy[20];
    JAISoundHandles handles(dummy, 20);
}
