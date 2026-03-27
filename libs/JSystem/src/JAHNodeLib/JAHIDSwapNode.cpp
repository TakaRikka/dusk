#include "JSystem/JAHostIO/JAHUTableEdit.h"
#include "JSystem/JAHostIO/JAHioMessage.h"

static void dummy(JAHUSeBox* box, JAHControl* ctrl) {
    box->getId();
    ctrl->makeCheckBox(NULL, (u8*)NULL, 0, false, 0);
    box->~JAHUSeBox();
}
