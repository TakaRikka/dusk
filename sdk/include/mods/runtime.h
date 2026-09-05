#pragma once

#include <mods/api.h>

/*
 * Lifecycle service for mods delegated to a runtime named by mod.json. The host passes the
 * runtime's context as ctx and the delegated mod's context as subject.
 */
typedef struct ModRuntimeService {
    ServiceHeader header;

    ModResult (*activate)(ModContext* ctx, ModContext* subject, ModError* out_error);
    ModResult (*update)(ModContext* ctx, ModContext* subject, ModError* out_error);
    ModResult (*deactivate)(ModContext* ctx, ModContext* subject, ModError* out_error);
} ModRuntimeService;
