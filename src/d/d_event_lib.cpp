/**
 * d_event_lib.cpp
 * Event Callback Library
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_event_lib.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor.h"

bool dEvLib_callback_c::eventUpdate() {
    if (mAction == NULL) {
        return FALSE;
    } else {
        executeAction();
        return TRUE;
    }
}

static action_class<dEvLib_callback_c> l_startAction(&dEvLib_callback_c::initStart,
                                                     &dEvLib_callback_c::executeStart);
static action_class<dEvLib_callback_c> l_runAction(&dEvLib_callback_c::initRun,
                                                   &dEvLib_callback_c::executeRun);

bool dEvLib_callback_c::setEvent(int mapToolId, int eventIdx, int param_2) {
    if (mAction != NULL) {
        return FALSE;
    } else {
        mActor->eventInfo.setMapToolId(mapToolId);
        if (mapToolId != 0xFF && eventIdx == 0xFF) {
            eventIdx = dComIfGp_getEventManager().getEventIdx(mActor, static_cast<u16>(mapToolId));
        }
        mActor->eventInfo.setEventId(eventIdx);
        _C = param_2;
        return setAction(&l_startAction);
    }
}

void dEvLib_callback_c::orderEvent(int param_0, int param_1, int param_2) {
    setEvent(param_0, param_1, param_2 != 0 ? 0x101 : 1);
}

bool dEvLib_callback_c::setAction(action_class<dEvLib_callback_c>* action) {
    mAction = action;
    if (mAction == NULL) {
        return false;
    } else {
        return initAction();
    }
}

bool dEvLib_callback_c::initAction() {
    if (!mAction->getInit()) {
        return TRUE;
    } else {
        return (this->*(mAction->getInit()))();
    }
}

bool dEvLib_callback_c::executeAction() {
    if (!mAction->getExecute()) {
        return TRUE;
    } else {
        return (this->*(mAction->getExecute()))();
    }
}

bool dEvLib_callback_c::initStart() {
    return executeStart();
}

bool dEvLib_callback_c::executeStart() {
    if (!mActor->eventInfo.checkCommandDemoAccrpt()) {
        if (mActor->eventInfo.getMapToolId() != 0xFF) {
            fopAcM_orderMapToolEvent(mActor, mActor->eventInfo.getMapToolId(),
                                     mActor->eventInfo.getEventId(), 0xFFFF, _C, 0);
        } else {
            fopAcM_orderOtherEventId(mActor, mActor->eventInfo.getEventId(),
                                     mActor->eventInfo.getMapToolId(), 0xFFFF, 0, _C);
        }
        mActor->eventInfo.onCondition(2);
        return TRUE;
    } else {
        return setAction(&l_runAction);
    }
}

bool dEvLib_callback_c::initRun() {
    return eventStart();
}

bool dEvLib_callback_c::executeRun() {
    if (!dComIfGp_evmng_endCheck(mActor->eventInfo.getEventId())) {
        return eventRun();
    } else {
        dComIfGp_getEvent().reset();
        setAction(NULL);
        return eventEnd();
    }
}
