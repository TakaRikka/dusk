#ifndef JORMCONTEXT_H
#define JORMCONTEXT_H

#include <dolphin/dolphin.h>
#include <stdint.h>
#include "JSystem/JHostIO/JORReflexible.h"
#include "JSystem/JSupport/JSUMemoryStream.h"

#define MCTX_MSG_RESET 0
#define MCTX_MSG_GET_ROOT_OBJ 2
#define MCTX_MSG_GEN_OBJ_INFO 4
#define MCTX_MSG_INVALID_NODE 5
#define MCTX_MSG_UPDATE_NODE 8
#define MCTX_MSG_FIO 9
#define MCTX_MSG_OPEN_MESSAGE_BOX 10
#define MCTX_MSG_START_NODE 12
#define MCTX_MSG_GET_HOST_INFO 14
#define MCTX_MSG_SHELL_EXEC 15

#define MCTX_COMMAND_START_NODE (u32)0
#define MCTX_COMMAND_END_NODE (u32)1
#define MCTX_COMMAND_GEN_CONTROL (u32)2
#define MCTX_COMMAND_GEN_NODE (u32)3
#define MCTX_COMMAND_START_SELECTOR (u32)4
#define MCTX_COMMAND_END_SELECTOR (u32)5
#define MCTX_COMMAND_SELECTOR_ITEM (u32)6
#define MCTX_COMMAND_INVALID_NODE (u32)7
#define MCTX_COMMAND_UPDATE_CONTROL (u32)8

#define JORM_DEFAULT_WIDTH 0x200
#define JORM_DEFAULT_HEIGHT 0x18

#define DEFINE_GEN_CHECKBOX(T, kind)                                                               \
    void genCheckBox(const char* label, T* pSrc, T mask, u32 style = 0,                            \
                     JOREventListener* pListener = NULL, u16 posX = -1, u16 posY = -1,             \
                     u16 width = JORM_DEFAULT_WIDTH, u16 height = JORM_DEFAULT_HEIGHT) {           \
        genCheckBoxSub(kind, label, (uintptr_t)pSrc, style, *pSrc, mask, pListener, posX, posY,    \
                       width, height);                                                             \
    }

#define DEFINE_GEN_CHECKBOX_ID(T, kind)                                                            \
    void genCheckBoxID(const char* label, u32 id, T mask, T initValue, u32 style = 0,              \
                       JOREventListener* pListener = NULL, u16 posX = -1, u16 posY = -1,           \
                       u16 width = JORM_DEFAULT_WIDTH, u16 height = JORM_DEFAULT_HEIGHT) {         \
        genCheckBoxSub(kind, label, id, style, initValue, mask, pListener, posX, posY, width,      \
                       height);                                                                    \
    }

#define DEFINE_GEN_SLIDER(T, kind)                                                                 \
    void genSlider(const char* label, T* pSrc, T rangeMin, T rangeMax, u32 style = 0,              \
                   JOREventListener* pListener = NULL, u16 posX = -1, u16 posY = -1,               \
                   u16 width = JORM_DEFAULT_WIDTH, u16 height = JORM_DEFAULT_HEIGHT) {             \
        genSliderSub(kind, label, (uintptr_t)pSrc, style, *pSrc, rangeMin, rangeMax, pListener,    \
                     posX, posY, width, height);                                                   \
    }

#define DEFINE_GEN_SLIDER_ID(T, kind)                                                              \
    void genSliderID(const char* label, u32 id, T data, T rangeMin, T rangeMax, u32 style = 0,     \
                     JOREventListener* pListener = NULL, u16 posX = -1, u16 posY = -1,             \
                     u16 width = JORM_DEFAULT_WIDTH, u16 height = JORM_DEFAULT_HEIGHT) {           \
        genSliderSub(kind, label, id, style, data, rangeMin, rangeMax, pListener, posX, posY,      \
                     width, height);                                                               \
    }

#define DEFINE_START_COMBO_BOX(T, kind)                                                            \
    void startComboBox(const char* label, T* pSrc, u32 style = 0,                                  \
                       JOREventListener* pListener = NULL, u16 posX = -1, u16 posY = -1,           \
                       u16 width = JORM_DEFAULT_WIDTH, u16 height = JORM_DEFAULT_HEIGHT) {         \
        startSelectorSub('CMBX', kind, label, (uintptr_t)pSrc, style, *pSrc, pListener, posX,      \
                         posY, width, height);                                                     \
    }

#define DEFINE_START_COMBO_BOX_ID(T, kind)                                                         \
    void startComboBoxID(const char* label, u32 id, T data, u32 style = 0,                         \
                         JOREventListener* pListener = NULL, u16 posX = -1, u16 posY = -1,         \
                         u16 width = JORM_DEFAULT_WIDTH, u16 height = JORM_DEFAULT_HEIGHT) {       \
        startSelectorSub('CMBX', kind, label, id, style, data, pListener, posX, posY, width,       \
                         height);                                                                  \
    }

#define DEFINE_UPDATE_COMBO_BOX(T)                                                                 \
    void updateComboBox(u32 mode, T* pSrc, u32 param_2) {                                          \
        updateSelectorSub(mode, (uintptr_t)pSrc, *pSrc, param_2);                                  \
    }

#define DEFINE_UPDATE_SLIDER(T)                                                                    \
    void updateSlider(u32 mode, T* pSrc, T rangeMin, T rangeMax, u32 param_5) {                    \
        updateSliderSub(mode, (uintptr_t)pSrc, *pSrc, rangeMin, rangeMax, param_5);                \
    }

#define DEFINE_UPDATE_SLIDER_ID(T)                                                                 \
    void updateSliderID(u32 mode, u32 id, T value, T rangeMin, T rangeMax, u32 param_5) {          \
        updateSliderSub(mode, id, value, rangeMin, rangeMax, param_5);                             \
    }

#define DEFINE_START_RADIO_BUTTON(T, kind)                                                         \
    void startRadioButton(const char* label, T* pSrc, u32 style, JOREventListener* pListener,      \
                          u16 posX, u16 posY, u16 width, u16 height) {                             \
        startSelectorSub('RBTN', kind, label, (uintptr_t)pSrc, style, *pSrc, pListener, posX,      \
                         posY, width, height);                                                     \
    }

namespace jhostio {
enum EKind {
    EKind_8B = 0x08,
    EKind_16B = 0x10,
    EKind_32B = 0x20,
};

inline u32 GetEKindSize(u32 param_0) {
    return param_0 & 0xFF;
}
}  // namespace jhostio

class JORReflexible;
class JORFile;
class JOREventListener;
class JORHostInfo;

class JORMContext {
public:
    JORMContext() : mOutputStream(this, 0x10000) {}

    void bufInit() { mOutputStream.seek(0, JSUStreamSeekFrom_SET); }
    void putMsgID(u32 msgID) { mOutputStream << msgID; }
    s32 msgSize() { return mOutputStream.getPosition(); }
    u8* msgPtr() { return mBuffer; }

    void openFile(JORFile* pFile, u32 flags, const char* path, const char* extMask, u32 maskSize,
                  const char* defaultExt, const char* param_6, const char* fileSuffix);
    void closeFile(JORFile* pFile);
    void readBegin(JORFile* pFile, s32 size);
    void readData(JORFile* pFile);
    void writeBegin(JORFile* pFile, u16 flags, u32 size);
    void writeData(JORFile* pFile, const void* pBuffer, s32 size, u32 position);
    void writeDone(JORFile* pFile, u32 size);
    void sendShellExecuteRequest(void*, const char*, const char*, const char*, const char*, int);
    void sendHostInfoRequest(u32 requestType, JORHostInfo* pHostInfo);
    void endNode();

    void genRootNode(const char* label, JORReflexible* obj, u32 param_2, u32 param_3) {
        genNodeSub(label, obj, param_2, param_3);
    }

    void genNode(const char* label, JORReflexible* obj, u32 param_2, u32 param_3) {
        mOutputStream << MCTX_COMMAND_GEN_NODE;
        genNodeSub(label, obj, param_2, param_3);
    }

    void genNode(JORReflexible* parentObj, u32 param_1, const char* label, JORReflexible* obj,
                 u32 param_4, u32 param_5) {
        ASSERTMSGLINE(97, parentObj != NULL,
                      "JORMContext: genNode must specify strict( not null node ) parent object\n");

        mOutputStream << MCTX_COMMAND_GEN_NODE;
        mOutputStream << param_1;
        putNode(parentObj);
        genNodeSub(label, obj, param_4, param_5);
    }

    void startNode(JORReflexible* parentObj, u32 param_1, const char* label, JORReflexible* obj,
                   u32 param_4, u32 param_5) {
        ASSERTMSGLINE(
            113, parentObj != NULL,
            "JORMContext: startNode must specify strict( not null node ) parent object\n");

        mOutputStream << MCTX_COMMAND_START_NODE;
        mOutputStream << param_1;
        putNode(parentObj);
        genNodeSub(label, obj, param_4, param_5);
    }

    void startNode(const char* label, JORReflexible* obj, u32 param_2, u32 param_3) {
        mOutputStream << MCTX_COMMAND_START_NODE;
        genNodeSub(label, obj, param_2, param_3);
    }

    void endUpdateNode() {
        // empty function
    }

    void startUpdateNode(JORReflexible* obj) { putNode(obj); }

    void genNodeSub(const char* label, JORReflexible* i_node, u32, u32);
    void putNode(JORReflexible* obj) {
        mOutputStream << (uintptr_t)obj;
    }
    void invalidNode(JORReflexible* i_node, u32);

    void genControl(u32 type, u32 kind, const char* label, u32 style, u32 id,
                    JOREventListener* pListener, u32 initValue);

    void genSliderSub(u32 kind, const char* label, u32 id, u32 style, s32 initValue, s32 rangeMin,
                      s32 rangeMax, JOREventListener* pListener, u16 posX, u16 posY, u16 width,
                      u16 height);

    void genCheckBoxSub(u32 kind, const char* label, u32 id, u32 style, u16 initValue, u16 mask,
                        JOREventListener* pListener, u16 posX, u16 posY, u16 width, u16 height);

    void startSelectorSub(u32 type, u32 kind, const char* label, u32 id, u32 style, s32 initValue,
                          JOREventListener* pListener, u16 posX, u16 posY, u16 width, u16 height);

    void endSelectorSub();

    void genSelectorItemSub(const char* label, s32 itemNo, u32 param_2, u16 posX, u16 posY,
                            u16 width, u16 height);

    void genButton(const char* label, u32 id, u32 style = 0, JOREventListener* pListener = NULL,
                   u16 posX = -1, u16 posY = -1, u16 width = JORM_DEFAULT_WIDTH,
                   u16 height = JORM_DEFAULT_HEIGHT);

    void genLabel(const char* label, u32 id, u32 style = 0, JOREventListener* pListener = NULL,
                  u16 posX = -1, u16 posY = -1, u16 width = JORM_DEFAULT_WIDTH,
                  u16 height = JORM_DEFAULT_HEIGHT);

    void genGroupBox(const char* label, u32 id, u32 style = 0, JOREventListener* pListener = NULL,
                     u16 posX = -1, u16 posY = -1, u16 width = JORM_DEFAULT_WIDTH,
                     u16 height = JORM_DEFAULT_HEIGHT);

    void genEditBoxID(const char* label, u32 id, const char* string, u16 length, u32 style = 0,
                      JOREventListener* pListener = NULL, u16 posX = -1, u16 posY = -1,
                      u16 width = JORM_DEFAULT_WIDTH, u16 height = JORM_DEFAULT_HEIGHT);

    /**
     * === CHECKBOX ===
     */
    DEFINE_GEN_CHECKBOX(u8, 0x100 | jhostio::EKind_8B)
    DEFINE_GEN_CHECKBOX(u16, 0x100 | jhostio::EKind_16B)

    DEFINE_GEN_CHECKBOX_ID(u16, JORPropertyEvent::EKind_ValueID | 0x100)

    /**
     * === SLIDER ===
     */
    DEFINE_GEN_SLIDER(u8, 0x100 | jhostio::EKind_8B)
    DEFINE_GEN_SLIDER(s16, jhostio::EKind_16B)
    DEFINE_GEN_SLIDER(f32, JORPropertyEvent::EKind_FloatValue | jhostio::EKind_32B)
    DEFINE_GEN_SLIDER(s32, jhostio::EKind_32B)

    DEFINE_GEN_SLIDER_ID(f64, JORPropertyEvent::EKind_ValueID | JORPropertyEvent::EKind_FloatValue)
    DEFINE_GEN_SLIDER_ID(int, JORPropertyEvent::EKind_ValueID)

    DEFINE_UPDATE_SLIDER(u8)
    DEFINE_UPDATE_SLIDER(s16)
    DEFINE_UPDATE_SLIDER(f32)
    DEFINE_UPDATE_SLIDER(s32)

    DEFINE_UPDATE_SLIDER_ID(f64)
    DEFINE_UPDATE_SLIDER_ID(int)

    /**
     * === COMBO BOX ===
     */
    DEFINE_START_COMBO_BOX(u8, 0x100 | jhostio::EKind_8B)
    DEFINE_START_COMBO_BOX(s16, jhostio::EKind_16B)
    DEFINE_START_COMBO_BOX(s32, jhostio::EKind_32B)

    DEFINE_START_COMBO_BOX_ID(int, JORPropertyEvent::EKind_ValueID)

    DEFINE_UPDATE_COMBO_BOX(u8)

    void endComboBox() { endSelectorSub(); }

    void genComboBoxItem(const char* label, s32 itemNo) {
        genSelectorItemSub(label, itemNo, 0, 0, 0, 0, 0);
    }

    /**
     * === RADIO BUTTON ===
     */
    DEFINE_START_RADIO_BUTTON(u8, 0x100 | jhostio::EKind_8B)
    DEFINE_START_RADIO_BUTTON(s16, jhostio::EKind_16B)
    DEFINE_START_RADIO_BUTTON(s32, jhostio::EKind_32B)

    void endRadioButton() { endSelectorSub(); }

    void genRadioButtonItem(const char* label, s32 itemNo, u32 param_2, u16 posX, u16 posY,
                            u16 width, u16 height) {
        genSelectorItemSub(label, itemNo, param_2, posX, posY, width, height);
    }

    void updateControl(u32 mode, u32 id, u32 param_2);
    void updateControl(u32 mode, u32 id, const char* param_2);
    void updateLabel(u32 mode, u32 id, const char* param_2) { updateControl(mode, id, param_2); }
    void updateSliderSub(u32 mode, u32 id, s32 value, s32 rangeMin, s32 rangeMax, u32 param_5);
    void updateCheckBoxSub(u32 mode, u32 id, u16 value, u16 mask, u32 param_4);
    void updateSelectorSub(u32 mode, u32 id, s32 value, u32 param_3);
    void updateEditBoxID(u32 mode, u32 id, const char* string, u32 param_3, u16 length);

    void updateCheckBox(u32 mode, u8* pSrc, u8 mask, u32 param_4) {
        updateCheckBoxSub(mode, (uintptr_t)pSrc, *pSrc, mask, param_4);
    }

    void updateCheckBoxID(u32 mode, u32 id, u8 value, u8 mask, u32 param_4) {
        updateCheckBoxSub(mode, id, value, mask, param_4);
    }

    void updateRadioButton(u32 mode, s16* pSrc, u32 param_3) {
        updateSelectorSub(mode, (uintptr_t)pSrc, *pSrc, param_3);
    }

    void editComboBoxItem(u32 param_0, u32 param_1, const char* param_2, s32 param_3, u32 param_4);

    void openMessageBox(void* param_0, u32 style, const char* message, const char* title);

    /* 0x00000 */ u8 mBuffer[0x10000];
    /* 0x10000 */ JSUMemoryOutputStream mOutputStream;
};

#endif /* JORMCONTEXT_H */
