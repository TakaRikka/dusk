#include "HBMAnmController.h"

#include "nw4hbm/lyt/animation.h"
#include "nw4hbm/lyt/group.h"

namespace homebutton {

    GroupAnmController::GroupAnmController() : mpGroup(), mpAnimGroup() {}

    GroupAnmController::~GroupAnmController() {}

    void GroupAnmController::do_calc() {
        bool flag;

        if (mState == ANIM_STATE_PLAY) {
            calc();
            flag = true;

            mpAnimGroup->SetFrame(mFrame);
        } else {
            flag = false;
        }

        nw4hbm::lyt::PaneLinkList& list = mpGroup->GetPaneList();

        for (nw4hbm::lyt::PaneLinkList::Iterator it = list.GetBeginIter(); it != list.GetEndIter();
             it++)
        {
            it->mTarget->SetAnimationEnable(mpAnimGroup, flag, false);
        }
    }

}  // namespace homebutton
