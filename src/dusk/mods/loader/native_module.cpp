#include "native_module.hpp"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif

#if defined(__SANITIZE_ADDRESS__)
#define ADDRESS_SANITIZER 1
#elif defined(__has_feature)
#if __has_feature(address_sanitizer)
#define ADDRESS_SANITIZER 1
#endif
#endif

namespace {
#if defined(_WIN32)
void* pl_dlopen(const std::filesystem::path& p) {
    return LoadLibraryExW(p.wstring().c_str(), nullptr,
        LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
}
void* pl_dlsym(void* h, const char* name) {
    return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(h), name));
}
void pl_dlclose(void* h) {
    FreeLibrary(static_cast<HMODULE>(h));
}
std::string pl_dlerror() {
    char buf[256]{};
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr,
        GetLastError(), 0, buf, sizeof(buf), nullptr);
    std::string s = buf;
    while (!s.empty() && (s.back() == '\r' || s.back() == '\n')) {
        s.pop_back();
    }
    return s;
}
#else
#include <dlfcn.h>
void* pl_dlopen(const std::filesystem::path& p) {
    int flags = RTLD_LAZY | RTLD_LOCAL;
#if defined(RTLD_DEEPBIND) && !defined(ADDRESS_SANITIZER)
    flags |= RTLD_DEEPBIND;
#endif
    return dlopen(p.c_str(), flags);
}
void* pl_dlsym(void* h, const char* name) {
    return dlsym(h, name);
}
void pl_dlclose(void* h) {
    dlclose(h);
}
std::string pl_dlerror() {
    const char* e = dlerror();
    return e ? e : "(unknown error)";
}
#endif
}  // namespace

namespace dusk::mods::loader {
NativeModule::NativeModule() noexcept : handle(nullptr) {}

NativeModule::NativeModule(NativeModule&& other) noexcept {
    handle = other.handle;
    other.handle = nullptr;
}

NativeModule& NativeModule::operator=(NativeModule&& other) noexcept {
    handle = other.handle;
    other.handle = nullptr;
    return *this;
}

NativeModule::NativeModule(const std::filesystem::path& path) {
    handle = pl_dlopen(path);
    if (!handle) {
        throw std::runtime_error(pl_dlerror());
    }
}

NativeModule::~NativeModule() {
    if (handle) {
        pl_dlclose(handle);
    }
}

void* NativeModule::LookupSymbol(const char* name) const {
    return pl_dlsym(handle, name);
}
}  // namespace dusk::mods::loader
