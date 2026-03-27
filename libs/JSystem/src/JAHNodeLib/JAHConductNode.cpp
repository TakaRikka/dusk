#include <JSystem/JAHostIO/JAHioMessage.h>

void dummy(JAHControl* cont, u16 s) {
    cont->startRadioButton(NULL, (u8*)NULL, (u8)0, 0);
    cont->makeRadioButtonItem(NULL, (u8)0);
    cont->endRadioButton();
    cont->makeButton(NULL, 0, (u8)0, 0);
}
