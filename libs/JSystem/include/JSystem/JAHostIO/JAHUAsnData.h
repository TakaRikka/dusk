#ifndef JAHUASNNDATA_H
#define JAHUASNNDATA_H

#include "types.h"

class JORReflexible;
class JAHControl;

class JAHUAsn {
public:
    JAHUAsn(u16 param_1) : unk0(param_1) {}

    u8 getAttribute(u32);
    u16 getSubNum(s32, s32);
    u16 getDataNum(s32, s32, s32);
    const char* getName(s32, s32, s32, s32);
    u32 getDataId(s32, s32, s32, s32);
    u8 getCategory(u32);
    u8 getSub(u32);
    u16 getDataPerCat(u32);
    u16 getDataPerSub(u32);
    u32 getNextId(u32, bool);

    void createComboBoxCategory(JAHControl&);
    void createComboBoxSub(JAHControl&, s32, s32);
    void createComboBoxData(JAHControl&, s32, s32, s32);
    void checkAndUpdateNameBox(s32*, s32*, s32*, s32*, s32, s32, s32, JORReflexible*);
    void updateNameBoxAll(s32*, s32*, s32*, s32*, JORReflexible*);

private:
    void __updateNameBoxCategory(s32*, JORReflexible*);
    void __updateNameBoxSub(s32, s32, s32*, JORReflexible*);
    void __updateNameBoxData(s32, s32, s32, s32*, JORReflexible*);
    u32 __GetSubNo(s32);
    u32 __GetDataNo(s32);

    /* 0x0 */ u16 unk0;
};

#endif /* JAHUASNNDATA_H */
