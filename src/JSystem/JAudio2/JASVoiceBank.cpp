#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASVoiceBank.h"
#include "JSystem/JAudio2/JASBasicInst.h"

const JASOscillator::Data JASVoiceBank::sOscData = {
    0, 1.0f, NULL, NULL, 1.0f, 0.0f,
};

JASOscillator::Data* JASVoiceBank::sOscTable;

bool JASVoiceBank::getInstParam(int param_0, int param_1, int param_2,
                                    JASInstParam* param_3) const {
    if (param_0 < 0) {
        return false;
    }
    sOscTable = (JASOscillator::Data*)&sOscData;
    param_3->field_0x1a = param_0;
    param_3->field_0x1d = 1;
    param_3->field_0x14 = &sOscTable;
    return true;
}

JASVoiceBank::~JASVoiceBank() {}

u32 JASVoiceBank::getType() const {
    return 'VOIC';
}
