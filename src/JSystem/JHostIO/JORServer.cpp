#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JORServer.h"
#include "JSystem/JHostIO/JORReflexible.h"
#include "JSystem/JHostIO/JORFile.h"
#include "JSystem/JHostIO/JORHostInfo.h"
#include "JSystem/JSupport/JSUMemoryStream.h"
#include <string.h>

void JOREventCallbackListNode::JORAppend() {
    JORServer* pServer = JORServer::getInstance();
    JUT_ASSERT(51, pServer!=NULL);
    pServer->appendEventCallbackListNode(this);
}

void JOREventCallbackListNode::JORRemove() {
    JORServer* pServer = JORServer::getInstance();
    JUT_ASSERT(57, pServer!=NULL);
    pServer->removeEventCallbackListNode(this);
}

JOREventCallbackListNode::JOREventCallbackListNode(u32 param_0, u32 param_1, bool append) {
    field_0xc = param_0;
    field_0x10 = param_1;

    if (append) {
        JORAppend();
    }
}

JOREventCallbackListNode::~JOREventCallbackListNode() {
    JORRemove();
}

void JORReflexible::listen(u32 command, const JOREvent* event) {
    switch (command) {
    case JORServer::ECommand_GenObjInfo:
        genObjectInfo((JORGenEvent*)event);
        break;
    case JORServer::ECommand_NodeEvent:
        listenNodeEvent((JORNodeEvent*)event);
        break;
    case JORServer::ECommand_PropertyEvent:
        listenPropertyEvent((JORPropertyEvent*)event);
        break;
    }
}

void JORReflexible::genObjectInfo(const JORGenEvent* event) {
    JORMContext* mctx = getJORServer()->attachMCTX(MCTX_MSG_GEN_OBJ_INFO);
    mctx->startNode("", this, 0, 0);
    genMessage(mctx);
    mctx->endNode();
    getJORServer()->releaseMCTX(mctx);
}

void JORReflexible::listenNodeEvent(const JORNodeEvent*) {}

void JORReflexible::listenPropertyEvent(const JORPropertyEvent* pEvent) {
    if (pEvent->kind & JORPropertyEvent::EKind_HasListener) {
        JORServer* pServer = getJORServer();

        JORPropertyEvent* event = (JORPropertyEvent*)pEvent;
        event->kind &= ~JORPropertyEvent::EKind_HasListener;

        switch (pEvent->type) { 
        case 'RNGf':
        case 'RNGi':
        case 'RBTN':
        case 'CMBX':
            if (pEvent->field_0x14 >= 4) {
                pEvent->field_0x10->listenPropertyEvent(pEvent);
            }
            break;
        case 'CHBX':
            if (pEvent->field_0x14 >= 4) {
                pEvent->field_0x10->listenPropertyEvent(pEvent);
            }
            break;
        case 'EDBX':
        case 'BUTN':
            pEvent->field_0x10->listenPropertyEvent(pEvent);
            break;
        }

        event->kind |= JORPropertyEvent::EKind_HasListener;
    } else if (!(pEvent->kind & JORPropertyEvent::EKind_ValueID)) {
        switch (pEvent->type) {
        case 'RBTN':
        case 'RNGi':
        case 'RNGf':
        case 'CMBX':
            if (pEvent->field_0x14 >= 4) {
                JORPropertyEvent* event = (JORPropertyEvent*)pEvent;
                JORServer::defSetVal((void*)event->id, event->kind, event->field_0x18.U32);
            }
            break;
        case 'CHBX':
            if (pEvent->field_0x14 >= 4) {
                JORPropertyEvent* event = (JORPropertyEvent*)pEvent;
                JORServer::defSetBitVal((void*)event->id, event->kind, (u16)event->field_0x18.U16[0], (u16)event->field_0x18.U16[1]);
            }
            break;
        case 'EDBX':
            JORPropertyEvent* event = (JORPropertyEvent*)pEvent;
            JSUMemoryInputStream stream(&event->field_0x18, event->field_0x14);
            stream >> (char*)event->id;
            break;
        }
        return;
    }
}

JORServer* JORServer::instance;

JORServer* JORServer::create() {
    if (instance == NULL) {
        instance = new JORServer();
    }

    return instance;
}

void JORServer::defSetVal(void* ptr, u32 kind, s32 val) {
    switch (jhostio::GetEKindSize(kind)) {
    case jhostio::EKind_8B:
        *(u8*)ptr = val;
        break;
    case jhostio::EKind_16B:
        *(u16*)ptr = val;
        break;
    case jhostio::EKind_32B:
        *(u32*)ptr = val;
        break;
    }
}

void JORServer::defSetBitVal(void* ptr, u32 kind, u16 param_2, u16 param_3) {
    param_2 &= param_3;

    switch (jhostio::GetEKindSize(kind)) {
    case jhostio::EKind_8B: {
        u8* kptr = (u8*)ptr;
        *kptr &= (u8)~param_3;
        *kptr |= (u8)param_2;
        break;
    }
    case jhostio::EKind_16B: {
        u16* kptr = (u16*)ptr;
        *kptr &= (u16)~param_3;
        *kptr |= param_2;
    }
    }
}

JORMContext* JORServer::attachMCTX(u32 msgID) {
    m_context.bufInit();
    m_context.putMsgID(msgID);
    return &m_context;
}

void JORServer::releaseMCTX(JORMContext* mctx) {
    ASSERTMSGLINE(292, mctx->msgSize() < 0x10000, "JORServer:releaseMCTX: context buffer probably overflowed.\n");

    if (mctx->msgSize() > 4) {
        const s32 var_r28 = send(mctx->msgPtr(), mctx->msgSize());
    }
}

void JORServer::receive(const char* pBuffer, s32 length) {
    JSUMemoryInputStream stream(pBuffer, length);

    u32 command;
    stream >> command;

    if (!stream.isGood()) {
        OS_REPORT("JORServer::receive : データが足りない\n");
        OS_REPORT("JORServer::receive : データが足りない\n");
        return;
    }

    switch (command) {
    case ECommand_GetRootObj:
        if (mp_rootObj != NULL) {
            OS_REPORT("Get RootObject ref(%08X)\n", mp_rootObj);
            m_context.bufInit();
            m_context.putMsgID(MCTX_MSG_GET_ROOT_OBJ);
            m_context.genRootNode(m_rootName, mp_rootObj, field_0x10064, field_0x10068);
            send(m_context.msgPtr(), m_context.msgSize());
        }
        break;
    case ECommand_NodeEvent: {
        u32 obj_addr;
        stream.read(obj_addr);
        JORNodeEvent* pEvent = (JORNodeEvent*)(pBuffer + stream.getPosition());
        stream.skip(4);

        if (stream.isGood()) {
            reinterpret_cast<JORReflexible*>(obj_addr)->listen(command, pEvent);
        }
    }
    break;
    case ECommand_PropertyEvent: {
        u32 obj_addr;
        stream.read(obj_addr);
        JORPropertyEvent* pEvent = (JORPropertyEvent*)(pBuffer + stream.getPosition());
        stream.skip(0x18);

        if (stream.isGood()) {
            stream.skip(pEvent->field_0x14);
        }

        if (stream.isGood()) {
            reinterpret_cast<JORReflexible*>(obj_addr)->listen(command, pEvent);
        }
    }
    break;
    case ECommand_GenObjInfo: {
        u32 obj_addr;
        stream.read(obj_addr);

        if (stream.isGood()) {
            reinterpret_cast<JORReflexible*>(obj_addr)->listen(command, NULL);
        }
    }
    break;
    case ECommand_FIO:
        fio_dispatchMessage_(stream);
        break;
    case ECommand_DIR:
        dir_dispatchMessage_(stream);
        break;
    case ECommand_ReadResultS32:
        readResultS32_(stream);
        break;
    case ECommand_HostInfo:
        hostinfo_dispatchMessage_(stream);
        break;
    case ECommand_ReadResultU32:
        readResultU32_(stream);
        break;
    case ECommand_ReadOrEvent:
        readOrEvent_(stream);
        break;
    }
}

void JORServer::fio_openFile_(JSUMemoryInputStream& stream) {
    u32 file_addr;
    stream >> file_addr;
    JORFile* file = (JORFile*)file_addr;

    u32 handle;
    stream >> handle;

    u32 file_len;
    stream >> file_len;

    u16 filename_len;
    u16 basename_len;
    u16 extension_name_len;
    stream >> filename_len >> basename_len >> extension_name_len;

    strncpy(file->getFilename(), (const char*)stream.getPointer(), filename_len);
    file->getFilename()[filename_len] = 0;

    file->setHandle(handle);
    file->setFileLength(file_len);
    file->setNFileName((u16)filename_len);
    file->setNBaseName((u16)basename_len);
    file->setNExtensionName((u16)extension_name_len);
    file->setStatus(JORFile::EStatus_READ_DATA);
}

void JORServer::fio_closeFile_(JSUMemoryInputStream& stream) {
    u32 file_addr;
    stream >> file_addr;
    JORFile* file = (JORFile*)file_addr;

    u32 sp8;
    stream >> sp8;

    if (sp8 == 0) {
        OS_REPORT("Close Failed!\n");
    }

    file->setStatus(JORFile::EStatus_READ_END);
}

void JORServer::fio_readData_(JSUMemoryInputStream& stream) {
    u32 file_addr;
    stream >> file_addr;
    JORFile* file = (JORFile*)file_addr;

    u32 status;
    stream >> status;

    if (status == JORFile::EStatus_READ_BEGIN) {
        file->setStatus(status);
    } else if (status == JORFile::EStatus_WRITE_BEGIN) {
        file->setStatus(status);
    } else if (status == JORFile::EStatus_WRITE_DATA) {
        u32 position, data;
        stream >> position >> data;
        file->getDataStream().seek(position, JSUStreamSeekFrom_SET);

        file->getDataStream().write(stream.getPointer(), data);
        file->setStatus(status);
    } else if (status == JORFile::EStatus_WRITE_END) {
        file->setStatus(status);
    }
}

void JORServer::fio_writeData_(JSUMemoryInputStream& stream) {
    u32 file_addr;
    stream >> file_addr;
    JORFile* file = (JORFile*)file_addr;

    u32 status;
    stream >> status;

    file->setStatus(status);
}

void JORServer::fio_dispatchMessage_(JSUMemoryInputStream& stream) {
    u32 command;
    stream >> command;

    switch (command) {
    case JORFile::ECommand_OPEN:
        fio_openFile_(stream);
        break;
    case JORFile::ECommand_CLOSE:
        fio_closeFile_(stream);
        break;
    case JORFile::ECommand_READ:
        fio_readData_(stream);
        break;
    case JORFile::ECommand_WRITE:
        fio_writeData_(stream);
        break;
    }
}

void JORServer::dir_dispatchMessage_(JSUMemoryInputStream& stream) {
    u32 command, status, dir_addr;
    stream >> command >> dir_addr >> status;

    JORDir* dir = (JORDir*)dir_addr;
    dir->setStatus(status);

    if (status != 2) {
        switch (command) {
        case 1:
            break;
        case 2:
            dir_findFirstFile_(stream, dir);
            break;
        case 3:
            dir_findNextFile_(stream, dir);
            break;
        case 0:
            dir_browseForFolder_(stream, dir);
            break;
        case 5:
            dir_browseForFolder_(stream, dir);
            break;
        }
    }
}

void JORServer::dir_findFirstFile_(JSUMemoryInputStream& stream, JORDir* directory) {
    char namebuf[256];
    u32 attribute, lo_time, hi_time, handle;
    stream >> handle >> attribute >> lo_time >> hi_time >> namebuf;

    directory->setFindHandle(handle);
    directory->setFileAttribute(attribute);
    directory->setLowDateTime(lo_time);
    directory->setHighDateTime(hi_time);
    strncpy(directory->getFilename(), namebuf, sizeof(namebuf) - 1);
}

void JORServer::dir_findNextFile_(JSUMemoryInputStream& stream, JORDir* directory) {
    char namebuf[256];
    u32 attribute, lo_time, hi_time;
    stream >> attribute >> lo_time >> hi_time >> namebuf;

    directory->setFileAttribute(attribute);
    directory->setLowDateTime(lo_time);
    directory->setHighDateTime(hi_time);
    strncpy(directory->getFilename(), namebuf, sizeof(namebuf) - 1);
}

void JORServer::dir_browseForFolder_(JSUMemoryInputStream& stream, JORDir* directory) {
    char namebuf[256];
    u32 attribute, lo_time, hi_time;
    stream >> attribute >> lo_time >> hi_time >> namebuf;

    directory->setFileAttribute(attribute);
    directory->setLowDateTime(lo_time);
    directory->setHighDateTime(hi_time);
    strncpy(directory->getFilename(), namebuf, sizeof(namebuf) - 1);
}

void JORServer::hostinfo_dispatchMessage_(JSUMemoryInputStream& stream) {
    u32 command, spC, hostinfo_addr;
    stream >> command >> hostinfo_addr >> spC;

    JORHostInfo* hostinfo = (JORHostInfo*)hostinfo_addr;
    hostinfo->setResult(spC != 0 ? 1 : 2);

    if (spC != 0) {
        switch (command) {
        case HOSTINFO_REQ_COMPUTER_NAME:
            hostinfo_recvString_(stream, (JORHostInfo_String*)hostinfo);
            break;
        case HOSTINFO_REQ_USERNAME:
            hostinfo_recvString_(stream, (JORHostInfo_String*)hostinfo);
            break;
        case HOSTINFO_REQ_LOCAL_TIME:
            hostinfo_localTime_(stream, (JORHostInfo_CalendarTime*)hostinfo);
            break;
        }
    }
}

void JORServer::hostinfo_recvString_(JSUMemoryInputStream& stream, JORHostInfo_String* pString) {
    char buffer[256];
    stream >> buffer;
    strncpy(pString->getString(), buffer, pString->getBufferSize());
}

void JORServer::hostinfo_localTime_(JSUMemoryInputStream& stream, JORHostInfo_CalendarTime* pCalendarTime) {
    u16 year, month, weekday, monthday, hours, minutes, seconds, milliseconds;
    stream >> year
           >> month
           >> weekday
           >> monthday
           >> hours
           >> minutes
           >> seconds
           >> milliseconds;

    OSCalendarTime* pTime = pCalendarTime->getCalendarTime();
    pTime->sec = seconds;
    pTime->min = minutes;
    pTime->hour = hours;
    pTime->mday = monthday;
    pTime->mon = month - 1;
    pTime->year = year;
    pTime->wday = weekday;
    pTime->msec = milliseconds;
    pTime->usec = 0;
    pTime->yday = monthday + JORGetYearDays(year, month - 1);
}

void JORServer::sendReset() {
    u32 spC = JHIhtonl(0);
    send(&spC, 4);
}

void JORServer::setRootNode(const char* name, JORReflexible* node, u32 param_2, u32 param_3) {
    mp_rootObj = node;
    if (name == NULL) {
        name = "ルート";
    }

    strncpy(m_rootName, name, sizeof(m_rootName) - 1);
    m_rootName[sizeof(m_rootName) - 1] = 0;
    field_0x10064 = param_2;
    field_0x10068 = param_3;
}

void JORServer::readResultU32_(JSUMemoryInputStream& stream) {
    u32 ptr_addr, val;
    stream >> ptr_addr >> val;

    u32* ptr = (u32*)ptr_addr;
    *ptr = val;
}

void JORServer::readResultS32_(JSUMemoryInputStream& stream) {
    u32 ptr_addr, val;
    stream >> ptr_addr >> val;

    s32* ptr = (s32*)ptr_addr;
    *ptr = val;
}

void JORServer::readOrEvent_(JSUMemoryInputStream& stream) {
    if (m_eventDone) {
        stream >> m_eventNum;

        u16 name_len;
        stream >> name_len;
        if (name_len >= 0x1000) {
            name_len = 0x1000 - 1;
        }

        stream.read(m_eventName, name_len);
        m_eventName[name_len] = 0;
        m_event = true;
    }
}

void JORServer::doneEvent() {
    if (!m_eventDone) {
        m_eventDone = true;

        u32 sp8 = JHIhtonl(0x10);
        send(&sp8, 4);
    }
}

void JORMContext::genNodeSub(const char* name, JORReflexible* node, u32 param_2, u32 param_3) {
    mOutputStream << name;
    putNode(node);

    mOutputStream << param_2;

    if (param_2 & 4) {
        mOutputStream << param_3;
    }
}

void JORMContext::endNode() {
    mOutputStream << MCTX_COMMAND_END_NODE;
}

void JORMContext::invalidNode(JORReflexible* node, u32 param_1) {
    mOutputStream << MCTX_COMMAND_INVALID_NODE;
    putNode(node);

    mOutputStream << param_1;
}

void JORMContext::genControl(u32 type, u32 kind, const char* label, u32 style, u32 id,
                             JOREventListener* pListener, u32 initValue)
{
    mOutputStream << type << kind << label << style << id;

    if (kind & JORPropertyEvent::EKind_HasListener) {
        mOutputStream << (uintptr_t)pListener;
    }

    if ((kind & JORPropertyEvent::EKind_ValueID) && type != 'EDBX') {
        kind |= 0x20;
    }

    if (jhostio::GetEKindSize(kind) != 0) {
        mOutputStream << initValue;
    }
}

void JORMContext::genSliderSub(u32 kind, const char* label, u32 id, u32 style, s32 initValue, s32 rangeMin,
                               s32 rangeMax, JOREventListener* pListener, u16 posX, u16 posY,
                               u16 width, u16 height)
{
    if (pListener != NULL) {
        kind |= JORPropertyEvent::EKind_HasListener;
    }

    mOutputStream << MCTX_COMMAND_GEN_CONTROL;
    genControl(kind & JORPropertyEvent::EKind_FloatValue ? 'RNGf' : 'RNGi', kind, label, style, id, pListener, initValue);

    mOutputStream << rangeMin << rangeMax;
    mOutputStream << posX << posY << width << height;
}

void JORMContext::genCheckBoxSub(u32 kind, const char* label, u32 id, u32 style, u16 initValue, u16 mask,
                                 JOREventListener* pListener, u16 posX, u16 posY, u16 width,
                                 u16 height)
{
    if (pListener != NULL) {
        kind |= JORPropertyEvent::EKind_HasListener;
    }

    mOutputStream << MCTX_COMMAND_GEN_CONTROL;
    genControl('CHBX', kind, label, style, id, pListener, (mask << 0x10) | initValue);

    mOutputStream << posX << posY << width << height;
}

void JORMContext::startSelectorSub(u32 type, u32 kind, const char* label, u32 id, u32 style, s32 initValue,
                                   JOREventListener* pListener, u16 posX, u16 posY, u16 width,
                                   u16 height)
{
    if (pListener != NULL) {
        kind |= JORPropertyEvent::EKind_HasListener;
    }

    mOutputStream << MCTX_COMMAND_START_SELECTOR;
    genControl(type, kind, label, style, id, pListener, initValue);

    mOutputStream << posX << posY << width << height;
}

void JORMContext::endSelectorSub() {
    mOutputStream << MCTX_COMMAND_END_SELECTOR;
}

void JORMContext::genSelectorItemSub(const char* label, s32 itemNo, u32 param_2, u16 posX,
                                     u16 posY, u16 width, u16 height)
{
    mOutputStream << MCTX_COMMAND_SELECTOR_ITEM << label << (u32)itemNo;
    mOutputStream << param_2 << posX << posY << width << height;
}

void JORMContext::genButton(const char* label, u32 id, u32 style, JOREventListener* pListener,
                            u16 posX, u16 posY, u16 width, u16 height)
{
    u32 kind = 0;
    if (pListener != NULL) {
        kind |= JORPropertyEvent::EKind_HasListener;
    }

    mOutputStream << MCTX_COMMAND_GEN_CONTROL;
    genControl('BUTN', kind, label, style, id, pListener, 0);

    mOutputStream << posX << posY << width << height;
}

void JORMContext::genLabel(const char* label, u32 id, u32 style, JOREventListener* pListener,
                           u16 posX, u16 posY, u16 width, u16 height)
{
    u32 kind = 0;
    if (pListener != NULL) {
        kind |= JORPropertyEvent::EKind_HasListener;
    }

    mOutputStream << MCTX_COMMAND_GEN_CONTROL;
    genControl('LABL', kind, label, style, id, pListener, 0);

    mOutputStream << posX << posY << width << height;
}

void JORMContext::genGroupBox(const char* label, u32 id, u32 style, JOREventListener* pListener,
                              u16 posX, u16 posY, u16 width, u16 height)
{
    u32 kind = 0;
    if (pListener != NULL) {
        kind |= JORPropertyEvent::EKind_HasListener;
    }

    mOutputStream << MCTX_COMMAND_GEN_CONTROL;
    genControl('GRBX', kind, label, style, id, pListener, 0);

    mOutputStream << posX << posY << width << height;
}

void JORMContext::genEditBoxID(const char* label, u32 id, const char* string, u16 length, u32 style,
                               JOREventListener* pListener, u16 posX, u16 posY, u16 width, u16 height)
{
    u32 kind = 0;
    if (pListener != NULL) {
        kind |= JORPropertyEvent::EKind_HasListener;
    }

    mOutputStream << MCTX_COMMAND_GEN_CONTROL;
    genControl('EDBX', kind | JORPropertyEvent::EKind_ValueID, label, style, id, pListener, 0);

    mOutputStream << length << string;
    mOutputStream << posX << posY << width << height;
}

void JORMContext::updateControl(u32 mode, u32 id, u32 param_2) {
    mOutputStream << MCTX_COMMAND_UPDATE_CONTROL << mode << id;

    if (mode & 1) {
        mOutputStream << param_2;
    }
}

void JORMContext::updateControl(u32 mode, u32 id, const char* param_2) {
    if (mode & 2) {
        mOutputStream << MCTX_COMMAND_UPDATE_CONTROL << mode << id << param_2;
    }
}

void JORMContext::updateSliderSub(u32 mode, u32 id, s32 value, s32 rangeMin, s32 rangeMax, u32 param_5) {
    updateControl(mode, id, param_5);

    if (mode & 2) {
        mOutputStream << value;
    }

    if (mode & 4) {
        mOutputStream << rangeMin << rangeMax;
    }
}

void JORMContext::updateCheckBoxSub(u32 mode, u32 id, u16 value, u16 mask, u32 param_4) {
    mOutputStream << MCTX_COMMAND_UPDATE_CONTROL << (mode | JORPropertyEvent::EKind_HasListener) << id << (u32)mask;

    if (mode & 1) {
        mOutputStream << param_4;
    }

    if (mode & 2) {
        mOutputStream << (u32)((mask << 0x10) | value);
    }
}

void JORMContext::updateSelectorSub(u32 mode, u32 id, s32 value, u32 param_3) {
    updateControl(mode, id, param_3);

    if (mode & 2) {
        mOutputStream << value;
    }
}

void JORMContext::updateEditBoxID(u32 mode, u32 id, const char* string, u32 param_3, u16 length) {
    updateControl(mode, id, param_3);

    if (mode & 2) {
        mOutputStream << string;
    }

    if (mode & 0x10) {
        mOutputStream << length;
    }
}

void JORMContext::editComboBoxItem(u32 param_0, u32 id, const char* param_2, s32 param_3, u32 param_4) {
    updateControl(8, id, (u32)0);

    mOutputStream << param_0 << param_4 << param_2 << param_3;
}

void JORMContext::openFile(JORFile* pFile, u32 flags, const char* path, const char* extMask,
                           u32 maskSize, const char* defaultExt, const char* param_6, const char* fileSuffix)
{
    if (defaultExt != NULL) {
        flags |= JORFile::EFlags_DEFAULT_EXT;
    }

    if (param_6 != NULL) {
        flags |= JORFile::EFlags_UNK_0x20;
    }

    if (fileSuffix != NULL) {
        flags |= JORFile::EFlags_HAS_SUFFIX;
    }

    mOutputStream << (u32)JORFile::ECommand_OPEN << (uintptr_t)pFile << flags << path;
    mOutputStream << (u16)maskSize;
    mOutputStream.write(extMask, maskSize);

    if (defaultExt != NULL) {
        mOutputStream << defaultExt;
    }

    if (param_6 != NULL) {
        mOutputStream << param_6;
    }

    if (fileSuffix != NULL) {
        mOutputStream << fileSuffix;
    }
}

void JORMContext::closeFile(JORFile* pFile) {
    mOutputStream << (u32)JORFile::ECommand_CLOSE << (uintptr_t)pFile << pFile->getHandle();
}

void JORMContext::readBegin(JORFile* pFile, s32 size) {
    mOutputStream << (u32)JORFile::ECommand_READ
                  << (u32)JORFile::EStatus_READ_BEGIN
                  << (uintptr_t)pFile
                  << pFile->getHandle()
                  << (u32)size;
}

void JORMContext::readData(JORFile* pFile) {
    mOutputStream << (u32)JORFile::ECommand_READ
                  << (u32)JORFile::EStatus_READ_DATA
                  << (uintptr_t)pFile
                  << pFile->getHandle();
}

void JORMContext::writeBegin(JORFile* pFile, u16 flags, u32 size) {
    mOutputStream << (u32)JORFile::ECommand_WRITE
                  << (u32)JORFile::EStatus_WRITE_BEGIN
                  << (uintptr_t)pFile
                  << pFile->getHandle()
                  << (u32)size
                  << (u32)flags;
}

void JORMContext::writeData(JORFile* pFile, const void* pBuffer, s32 size, u32 position) {
    mOutputStream << (u32)JORFile::ECommand_WRITE
                  << (u32)JORFile::EStatus_WRITE_DATA
                  << (uintptr_t)pFile
                  << pFile->getHandle()
                  << (u32)position;

    mOutputStream << (u16)size;
    mOutputStream.write(pBuffer, size);
}

void JORMContext::writeDone(JORFile* pFile, u32 size) {
    mOutputStream << (u32)JORFile::ECommand_WRITE
                  << (u32)JORFile::EStatus_WRITE_END
                  << (uintptr_t)pFile
                  << pFile->getHandle()
                  << (u32)size;
}

void JORMContext::openMessageBox(void* param_0, u32 style, const char* message, const char* title) {
    mOutputStream << (uintptr_t)param_0 << (u32)style << message << title;
}

void JORMContext::sendHostInfoRequest(u32 requestType, JORHostInfo* pHostInfo) {
    mOutputStream << requestType << (uintptr_t)pHostInfo;
}

void JORMContext::sendShellExecuteRequest(void* param_0, const char* param_1, const char* param_2,
                                          const char* param_3, const char* param_4, int param_5)
{
    mOutputStream << (uintptr_t)param_0 << param_1 << param_2 << param_3 << param_4 << (u32)param_5;
}
