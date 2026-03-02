#ifndef NW4HBM_LYT_RESOURCE_ACCESSOR_H
#define NW4HBM_LYT_RESOURCE_ACCESSOR_H

#include <revolution/types.h>

namespace nw4hbm {

    namespace ut {
        class Font;
    }

    namespace lyt {
        class ResourceAccessor {
        public:
            ResourceAccessor();

            /* 0x08 */ virtual ~ResourceAccessor();
            /* 0x0C */ virtual void* GetResource(u32 resType, const char* name, u32* pSize) = 0;
            /* 0x10 */ virtual ut::Font* GetFont(const char* name);

            /* 0x00 (vtable) */
        };  // size = 0x04

    }  // namespace lyt
}  // namespace nw4hbm

#endif
