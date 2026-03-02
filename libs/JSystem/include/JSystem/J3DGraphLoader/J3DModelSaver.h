#ifndef J3DMODELSAVER_H
#define J3DMODELSAVER_H

#include "JSystem/J3DGraphAnimator/J3DModel.h"

enum J3DBinaryDisplayListSaverFlag {
    J3DBinaryDisplayListSaverFlag_UNK_1 = 1,
};

namespace J3DModelSaverDataBase {
    void* saveBinaryDisplayList(const J3DModel*, J3DBinaryDisplayListSaverFlag, u32);
};

#endif
