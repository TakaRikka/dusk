#pragma once

#ifdef DEVKITPRO
#include <cstdint>
#include <list>
#include <coreinit/thread.h>
#endif

enum struct DataIDs : uint32_t {
#ifdef DEVKITPRO
    FILE_OP_BUFFER = OS_THREAD_SPECIFIC_0
#else
    FILE_OP_BUFFER = 0
#endif
};

template<typename T, DataIDs ID>
class ThreadLocal {
private:
#ifdef DEVKITPRO //TODO: somehow unregister data on all threads during destruct?
    std::list<T> data;
#else
    inline static thread_local T data;
#endif

public:
    T& get() {
    #ifdef DEVKITPRO
        const OSThreadSpecificID& id = static_cast<OSThreadSpecificID>(ID);
        if(OSGetThreadSpecific(id) == nullptr) {
            data.emplace_back();
            OSSetThreadSpecific(id, &data.back());
        }
        return *reinterpret_cast<T*>(OSGetThreadSpecific(id));
    #else
        return data;
    #endif
    }

    ThreadLocal() = default;
};
