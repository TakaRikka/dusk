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

    static const JASOscillator::Data sOscData;
    static JASOscillator::Data* sOscTable;
};

#endif /* JASVOICEBANK_H */
