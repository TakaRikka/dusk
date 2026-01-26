#ifndef METROTRK_PORTABLE_MSG_H
#define METROTRK_PORTABLE_MSG_H

#include <dolphin/types.h>

typedef struct _TRK_Msg {
	u8 _00[8];
	u32 m_msgLength;
	u32 _0C;
	u32 m_msg;
} TRK_Msg;

#endif /* METROTRK_PORTABLE_MSG_H */
