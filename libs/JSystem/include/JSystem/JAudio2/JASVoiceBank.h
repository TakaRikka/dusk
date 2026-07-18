#ifndef JASVOICEBANK_H
#define JASVOICEBANK_H

#include "JSystem/JAudio2/JASBank.h"
#include "JSystem/JAudio2/JASOscillator.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASVoiceBank : public JASBank {
public:
    virtual bool getInstParam(int, int, int, JASInstParam*) const;
    virtual ~JASVoiceBank();
    virtual u32 getType() const;

    static DUSK_GAME_DATA const JASOscillator::Data sOscData;
    static DUSK_GAME_DATA JASOscillator::Data* sOscTable;
};

#endif /* JASVOICEBANK_H */
