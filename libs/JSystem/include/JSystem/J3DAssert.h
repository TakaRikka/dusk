#ifndef J3DASSERT_H
#define J3DASSERT_H

#include "JSystem/JUtility/JUTAssert.h"

#define J3D_ASSERT_NULLPTR(LINE, COND) JUT_ASSERT_MSG(LINE, (COND) != 0, "Error : null pointer.")
#define J3D_ASSERT_RANGE(LINE, COND) JUT_ASSERT_MSG(LINE, (COND) != 0, "Error : range over.")
#define J3D_ASSERT_NONZEROARG(LINE, COND) JUT_ASSERT_MSG(LINE, (COND) != 0, "Error : non-zero argument is specified 0.")
#define J3D_ASSERT_ALLOCMEM(LINE, COND) JUT_ASSERT_MSG(LINE, (COND) != 0, "Error : allocate memory.")

#endif /* J3DASSERT_H */
