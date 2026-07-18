#pragma once

#include <filesystem>

namespace dusk::mods::loader {
class NativeModule final {
public:
    NativeModule() noexcept;
    NativeModule(const NativeModule& other) = delete;
    NativeModule(NativeModule&& other) noexcept;
    explicit NativeModule(const std::filesystem::path& path);
    ~NativeModule();

    void* LookupSymbol(const char* name) const;

    template<typename T>
    T LookupSymbol(const char* name) const {
        return reinterpret_cast<T>(LookupSymbol(name));
    }

    NativeModule& operator=(NativeModule&& other) noexcept;

#if defined(_WIN32)
    static constexpr auto LibraryExtension = ".dll";
#else
    static constexpr auto LibraryExtension = ".so";
#endif

private:
    void* handle;
};

}
