#include "hooks.hpp"
#include "texture_utils.hpp"
#include "types.h"

#include "mods/svc/hook.hpp"
#include "mods/svc/log.hpp"

DEFINE_HOOK_SYMBOL(
    "mDoDvdThd_mountArchive_c::execute", s32(mDoDvdThd_mountArchive_c*), MountArchiveExecute);

void mount_archive_execute_post(ModContext*, void* args, void* retval, void* userdata) {
    auto archive = mods::arg<mDoDvdThd_mountArchive_c*>(args, 0);
    handle_texture_overrides_on_load(archive);
}

ModResult add_all_hooks() {
    auto result = mods::hook::add_post<MountArchiveExecute>(mount_archive_execute_post);
    if (result != MOD_OK) {
        mods::log::debug("failed to add post hook to mountArchive_execute, Result {}", static_cast<int>(result));
        return result;
    }

    return MOD_OK;
}