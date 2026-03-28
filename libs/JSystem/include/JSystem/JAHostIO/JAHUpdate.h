#ifndef JAHUPDATE_H
#define JAHUPDATE_H

#include "JSystem/JAHostIO/JAHioNode.h"
#include "JSystem/JHostIO/JORMContext.h"
#include "JSystem/JHostIO/JORServer.h"
#include "types.h"

namespace JAHUpdate {

extern JAHioNode* spNode;
extern JORMContext* spMc;

inline void startUpdateNode(JAHioNode* param_1) {
    spMc = attachJORMContext(8);
    spNode = param_1;
    spMc->startUpdateNode(spNode);
}

inline void endUpdateNode() {
    spMc->endUpdateNode();
    releaseJORMContext(spMc);
    spMc = NULL;
    spNode = NULL;
}

inline void buttonOn(JAHioNode* param_1, u32 param_2) {
    bool b = false;
    if (spMc != NULL)
        b = true;

    if (!b)
        startUpdateNode(param_1);

    spMc->updateButton(1, param_2, 0);

    if (!b)
        endUpdateNode();
}

inline void buttonOff(JAHioNode* param_1, u32 param_2) {
    bool b = false;
    if (spMc != NULL)
        b = true;

    if (!b)
        startUpdateNode(param_1);

    spMc->updateButton(1, param_2, 0x40000000);

    if (!b)
        endUpdateNode();
}

}  // namespace JAHUpdate

#endif /* JAHUPDATE_H */
