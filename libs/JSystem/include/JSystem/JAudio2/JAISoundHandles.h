#ifndef JAISOUNDHANDLES_H
#define JAISOUNDHANDLES_H

#include "JSystem/JAudio2/JAISound.h"
#include "JSystem/JUtility/JUTAssert.h"

class JAISound;
class JAISoundID;

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAISoundHandles {
public:
    JAISoundHandles(JAISoundHandle* handle, int handleNum) {
        handle_ = handle;
        numHandles_ = handleNum;
    };

    JAISoundHandle& operator[](int n) { return handle_[n]; }

    JAISoundHandle* getHandleSoundID(JAISoundID id);
    JAISoundHandle* getFreeHandle();

private:
    JAISoundHandle* handle_;
    int numHandles_;
};

#endif /* JAISOUNDHANDLES_H */
