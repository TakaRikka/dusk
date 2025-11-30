#ifndef JSGFOG_H
#define JSGFOG_H

#include "JSystem/JStage/JSGObject.h"
#include <dolphin/gx.h>

namespace JStage {
    
    /**
    * @ingroup jsystem-jstage
    * 
    */
    struct TFog : public TObject {
        virtual ~TFog() = 0;
        virtual s32 JSGFGetType() const;
        virtual bool JSGGetFogFunction() const;
        virtual void JSGSetFogFunction(GXFogType);
        virtual f32 JSGGetStartZ() const;
        virtual void JSGSetStartZ(f32);
        virtual f32 JSGGetEndZ() const;
        virtual void JSGSetEndZ(f32);
        virtual GXColor JSGGetColor() const;
        virtual void JSGSetColor(GXColor);
    };
};

#endif /* JSGFOG_H */
