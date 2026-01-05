#ifndef JASBANK_H
#define JASBANK_H

#include "dolphin/types.h"

class JASChannel;
class JASInstParam;
class JASWaveBank;

namespace JASDsp {
    class TChannel;
}

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASBank {
public:
    static const int PRG_OSC = 240;

    JASBank() { mWaveBank = NULL; }
    virtual ~JASBank() {};
    virtual bool getInstParam(int, int, int, JASInstParam*) const = 0;
    virtual u32 getType() const = 0;

    JASWaveBank* getWaveBank() const { return mWaveBank; }
    void assignWaveBank(JASWaveBank* param_0) { mWaveBank = param_0; }

    static JASChannel* noteOn(JASBank const*, int, u8, u8, u16,
                                void (*)(u32, JASChannel*, JASDsp::TChannel*, void*), void*);
    static JASChannel* noteOnOsc(int, u8, u8, u16,
                                void (*)(u32, JASChannel*, JASDsp::TChannel*, void*), void*);

private:
    /* 0x04 */ JASWaveBank* mWaveBank;
};

#endif /* JASBANK_H */
