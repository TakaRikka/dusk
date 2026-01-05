#ifndef JKRARAMPIECE_H
#define JKRARAMPIECE_H

#include "JSystem/JSupport/JSUList.h"
#include <dolphin/ar.h>
#include <dolphin/os.h>
#include <dolphin/os.h>

class JKRAramBlock;
class JKRDecompCommand;

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRAMCommand {
public:
    typedef void (*AsyncCallback)(u32);

    JKRAMCommand();
    ~JKRAMCommand();

public:
    /* 0x00 */ ARQRequest mRequest;
    /* 0x20 */ JSULink<JKRAMCommand> mPieceLink;
    /* 0x30 */ JSULink<JKRAMCommand> field_0x30;

    /* 0x40 */ s32 mTransferDirection;
    /* 0x44 */ u32 mDataLength;
    /* 0x48 */ u32 mSrc;
    /* 0x4C */ u32 mDst;
    /* 0x50 */ JKRAramBlock* mAramBlock;
    /* 0x54 */ u32 field_0x54;
    /* 0x58 */ AsyncCallback mCallback;
    /* 0x5C */ OSMessageQueue* field_0x5C;
    /* 0x60 */ s32 field_0x60;
    /* 0x64 */ JKRDecompCommand* mDecompCommand;
    /* 0x68 */ OSMessageQueue mMessageQueue;
    /* 0x88 */ OSMessage mMessage;
    /* 0x8C */ void* field_0x8C;
    /* 0x90 */ void* field_0x90;
    /* 0x94 */ void* field_0x94;
};

struct JKRAramCommand {
    s32 field_0x00;
    void* command;

    void setting(int param_1, void* param_2) {
        field_0x00 = param_1;
        command = param_2;
    }
};

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRAramPiece {
public:
    static OSMutex mMutex;
    // TODO: fix type
    static JSUList<JKRAMCommand> sAramPieceCommandList;

public:
    static JKRAMCommand* prepareCommand(int, u32, u32, u32, JKRAramBlock*,
                                        JKRAMCommand::AsyncCallback);
    static void sendCommand(JKRAMCommand*);

    static JKRAMCommand* orderAsync(int, u32, u32, u32, JKRAramBlock*, JKRAMCommand::AsyncCallback);
    static BOOL sync(JKRAMCommand*, int);
    static BOOL orderSync(int, u32, u32, u32, JKRAramBlock*);
    static void startDMA(JKRAMCommand*);
    static void doneDMA(u32);

private:
    static void lock() { OSLockMutex(&mMutex); }
    static void unlock() { OSUnlockMutex(&mMutex); }
};

inline void JKRAramPcs_SendCommand(JKRAMCommand* command) {
    JKRAramPiece::sendCommand(command);
}

inline BOOL JKRAramPcs(int direction, u32 source, u32 destination, u32 length,
                       JKRAramBlock* block) {
    return JKRAramPiece::orderSync(direction, source, destination, length, block);
}

#endif /* JKRARAMPIECE_H */
