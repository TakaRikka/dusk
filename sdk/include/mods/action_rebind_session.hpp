#pragma once

#if !defined(DUSK_BUILDING_GAME) && !defined(DUSK_MOD_FEATURE_ACTIONS)
#error "mods/action_rebind_session.hpp requires add_mod(... FEATURES actions) or DUSK_BUILDING_GAME"
#endif

#include "mods/svc/actions.h"

namespace mods {

// Shared UI copy while an Action rebind capture is active.
inline constexpr const char* kActionRebindCapturingPrompt = "Press a key or button...";

/*
 * Thin RAII helper over ActionService rebind session entry points. Session state lives in the host;
 * this type only creates/destroys a session handle and forwards begin/cancel/query.
 *
 * Ops must provide ModContext** and const ActionService** (import slots) so a file-scope helper
 * stays valid after host fill-in.
 */
struct ActionServiceRebindOps {
    ModContext** ctx = nullptr;
    const ActionService** actions = nullptr;
};

class ActionRebindSession {
public:
    explicit ActionRebindSession(ActionServiceRebindOps ops = {}) : mOps(ops) {
        if (mOps.ctx != nullptr && mOps.actions != nullptr && *mOps.ctx != nullptr &&
            *mOps.actions != nullptr)
        {
            (*mOps.actions)->create_rebind_session(*mOps.ctx, &mSession);
        }
    }

    ActionRebindSession(const ActionRebindSession&) = delete;
    ActionRebindSession& operator=(const ActionRebindSession&) = delete;

    ~ActionRebindSession() { destroy(); }

    ActionServiceRebindOps& ops() { return mOps; }
    const ActionServiceRebindOps& ops() const { return mOps; }

    bool is_capturing() const {
        bool capturing = false;
        query(&capturing, nullptr);
        return capturing;
    }

    bool is_capturing(ActionHandle action) const {
        bool capturing = false;
        ActionHandle current = 0;
        query(&capturing, &current);
        return capturing && current == action;
    }

    ActionHandle capturing_action() const {
        bool capturing = false;
        ActionHandle current = 0;
        query(&capturing, &current);
        return capturing ? current : 0;
    }

    const char* capturing_prompt(ActionHandle action) const {
        return is_capturing(action) ? kActionRebindCapturingPrompt : nullptr;
    }

    ModResult begin(ActionHandle action, ActionCaptureFilter filter = ACTION_CAPTURE_EITHER) {
        if (!ensure_session()) {
            return MOD_INVALID_ARGUMENT;
        }
        return (*mOps.actions)->begin_rebind_session(*mOps.ctx, mSession, action, filter);
    }

    void cancel() {
        if (mSession != 0 && mOps.ctx != nullptr && mOps.actions != nullptr &&
            *mOps.ctx != nullptr && *mOps.actions != nullptr)
        {
            (*mOps.actions)->cancel_rebind_session(*mOps.ctx, mSession);
        }
    }

private:
    bool ensure_session() {
        if (mSession != 0) {
            return true;
        }
        if (mOps.ctx == nullptr || mOps.actions == nullptr || *mOps.ctx == nullptr ||
            *mOps.actions == nullptr)
        {
            return false;
        }
        return (*mOps.actions)->create_rebind_session(*mOps.ctx, &mSession) == MOD_OK &&
               mSession != 0;
    }

    void query(bool* outCapturing, ActionHandle* outAction) const {
        if (outCapturing != nullptr) {
            *outCapturing = false;
        }
        if (outAction != nullptr) {
            *outAction = 0;
        }
        if (mSession == 0 || mOps.ctx == nullptr || mOps.actions == nullptr ||
            *mOps.ctx == nullptr || *mOps.actions == nullptr)
        {
            return;
        }
        (*mOps.actions)->query_rebind_session(*mOps.ctx, mSession, outCapturing, outAction);
    }

    void destroy() {
        if (mSession == 0) {
            return;
        }
        if (mOps.ctx != nullptr && mOps.actions != nullptr && *mOps.ctx != nullptr &&
            *mOps.actions != nullptr)
        {
            (*mOps.actions)->destroy_rebind_session(*mOps.ctx, mSession);
        }
        mSession = 0;
    }

    ActionServiceRebindOps mOps;
    ActionRebindSessionHandle mSession = 0;
};

using ActionServiceRebindSession = ActionRebindSession;

}  // namespace mods
