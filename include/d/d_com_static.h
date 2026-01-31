#ifndef D_COM_D_COM_STATIC_H
#define D_COM_D_COM_STATIC_H

#include "d/actor/d_a_suspend.h"

u8 daNpcKakashi_getSwdTutorialStep();
void daNpcKakashi_setSwdTutorialStep(u8 iEvtNum);
bool daNpcKakashi_getSwdTutorialResult();
bool daNpcKakashi_chkSwdTutorialStage();
u8 daNpcKakashi_getSuccessCount();
void daNpcKakashi_clrSuccessCount();
void daNpcKakashi_setSwdTutorialResult(bool param_0);
void daNpcKakashi_incSuccessCount();
int daNpcMsg_setEvtNum(u8 iEvtNum);

#endif /* D_COM_D_COM_STATIC_H */
