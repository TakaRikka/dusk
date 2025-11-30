#ifndef JSGSYSTEM_H
#define JSGSYSTEM_H

#include "JSystem/JStage/JSGObject.h"

namespace JStage {
    enum TEObject {
        /* 0x0 */ OBJECT_UNDEFINED,
        /* 0x1 */ OBJECT_UNK1,
        /* 0x2 */ OBJECT_ACTOR,
        /* 0x3 */ OBJECT_CAMERA,
        /* 0x4 */ OBJECT_AMBIENT,
        /* 0x5 */ OBJECT_LIGHT,
        /* 0x6 */ OBJECT_FOG,
    };

    /**
    * @ingroup jsystem-jstage
    * 
    */
    struct TSystem : public TObject {
        virtual ~TSystem() = 0;
        virtual s32 JSGFGetType() const;
        virtual int JSGFindObject(JStage::TObject**, char const*, JStage::TEObject) const;
        virtual bool JSGCreateObject(char const*, JStage::TEObject, u32);
        virtual void JSGDestroyObject(JStage::TObject*);
        virtual bool JSGGetSystemData(u32);
        virtual void JSGSetSystemData(u32, u32);
    };
};

#endif /* JSGSYSTEM_H */
