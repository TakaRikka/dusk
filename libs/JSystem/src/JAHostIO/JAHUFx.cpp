#include <JSystem/JAHostIO/JAHioMessage.h>

void dummy(JAHControl* cont, u16 s) {
    cont->makeSlider(NULL, (s16*)NULL, (s16)0, (s16)0, 0);
    cont->makeSlider(NULL, (s32*)NULL, (s32)0, (s32)0, 0);
    cont->returnYDirectSize(s);
    cont->startRadioButton(NULL, (u8*)NULL, (u8)0, 0);
}
