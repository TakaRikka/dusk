/*
#include <cstdio>
#include <cstring>
*/
#pragma mark J3DShapeTable
#include "JSystem/J3DGraphAnimator/J3DShapeTable.h"

void J3DShapeTable::initShapeNodes(J3DDrawMtxData* mtxData, J3DVertexData* vtxData) {
    puts("J3DShapeTable::initShapeNodes is a stub");
}

// JAISe::JAISeMgr_startID_, JAISeq::JAISeqMgr_startID_, JAIStream::JAIStreamMgr_startID_
// are compiled from their real source files (JAISe.obj, JAISeq.obj, JAIStream.obj)

#pragma mark JAUSection
#include "JSystem/JAudio2/JAUSectionHeap.h"

JAUSoundTable* JAUSection::newSoundTable(void const* data, u32 size, bool flag) {
    puts("JAUSection::newSoundTable is a stub");
    return nullptr;
}

JAUSoundNameTable* JAUSection::newSoundNameTable(void const* data, u32 size, bool flag) {
    puts("JAUSection::newSoundNameTable is a stub");
    return nullptr;
}

JAIStreamDataMgr* JAUSection::newStreamFileTable(void const* data, bool flag) {
    puts("JAUSection::newStreamFileTable is a stub");
    return nullptr;
}

#pragma mark JASHeap
#include "JSystem/JAudio2/JASHeapCtrl.h"

JASHeap::JASHeap(JASDisposer* disposer)
    : mTree(this), mDisposer(disposer), mBase(nullptr), mSize(0), field_0x40(nullptr) {
    OSInitMutex(&mMutex);
}

#pragma mark JASVoiceBank
#include "JSystem/JAudio2/JASVoiceBank.h"

bool JASVoiceBank::getInstParam(int a, int b, int c, JASInstParam* param) const {
    puts("JASVoiceBank::getInstParam is a stub");
    return false;
}

// JASSeqParser::sCallBackFunc is compiled from JASSeqParser.obj
/*
#pragma mark JHICommBuf
#include "JSystem/JHostIO/JHIComm.h"

void JHICommBufHeader::init() {
    puts("JHICommBufHeader::init is a stub");
}

int JHICommBufHeader::load() {
    puts("JHICommBufHeader::load is a stub");
    return -1;
}

int JHICommBufReader::readBegin() {
    puts("JHICommBufReader::readBegin is a stub");
    return -1;
}

void JHICommBufReader::readEnd() {
    puts("JHICommBufReader::readEnd is a stub");
}

int JHICommBufReader::read(void* buf, int size) {
    puts("JHICommBufReader::read is a stub");
    return 0;
}

u32 JHICommBufReader::Header::getReadableSize() const {
    puts("JHICommBufReader::Header::getReadableSize is a stub");
    return 0;
}

int JHICommBufWriter::writeBegin() {
    puts("JHICommBufWriter::writeBegin is a stub");
    return -1;
}

void JHICommBufWriter::writeEnd() {
    puts("JHICommBufWriter::writeEnd is a stub");
}

int JHICommBufWriter::write(void* buf, int size) {
    puts("JHICommBufWriter::write is a stub");
    return 0;
}

#pragma mark HIO / HIO2
#include <dolphin/hio.h>
#include <revolution/hio2.h>

extern "C" {

BOOL HIORead(u32 addr, void* buffer, s32 size) {
    puts("HIORead is a stub");
    return FALSE;
}

BOOL HIOWrite(u32 addr, void* buffer, s32 size) {
    puts("HIOWrite is a stub");
    return FALSE;
}

BOOL HIO2Init(void) {
    puts("HIO2Init is a stub");
    return FALSE;
}

BOOL HIO2EnumDevices(HIO2EnumCallback callback) {
    puts("HIO2EnumDevices is a stub");
    return FALSE;
}

s32 HIO2Open(HIO2DeviceType type, HIO2UnkCallback exiCb, HIO2DisconnectCallback disconnectCb) {
    puts("HIO2Open is a stub");
    return -1;
}

BOOL HIO2Close(s32 handle) {
    puts("HIO2Close is a stub");
    return FALSE;
}

}  // extern "C"

#pragma mark JOR
#include "JSystem/JHostIO/JORServer.h"

int JOREventCallbackListNode::JORAct(u32 eventID, const char* eventName) {
    puts("JOREventCallbackListNode::JORAct is a stub");
    return 0;
}
*/
#pragma mark J3DPSMtxArrayConcat
void J3DPSMtxArrayConcat(float (*a)[4], float (*b)[4], float (*out)[4], u32 count) {
    puts("J3DPSMtxArrayConcat is a stub");
}
