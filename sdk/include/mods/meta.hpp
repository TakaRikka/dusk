#pragma once

#include <mods/api.h>

#include <array>
#include <cstddef>
#include <string_view>
#include <type_traits>
#include <utility>

/*
 * modmeta records. Each IMPORT_SERVICE/EXPORT_SERVICE/DEFINE_HOOK use places one
 * constant-initialized record object in the metadata section.
 */
#if defined(_WIN32)
#pragma section("modmeta$a", read, write)
#pragma section("modmeta$d", read, write)
#pragma section("modmeta$z", read, write)
#if defined(__clang__)
#define MOD_META_RECORD __declspec(allocate("modmeta$d")) __attribute__((used))
#else
#define MOD_META_RECORD __declspec(allocate("modmeta$d"))
#endif
#elif defined(__APPLE__)
#define MOD_META_RECORD __attribute__((section("__DATA,__modmeta"), used))
#elif defined(__has_attribute) && __has_attribute(retain)
#define MOD_META_RECORD __attribute__((section("modmeta"), used, retain))
#else
#define MOD_META_RECORD __attribute__((section("modmeta"), used))
#endif

/* Section bounds for the mod_meta descriptor */
#if defined(_WIN32)
#define MOD_META_BOUNDS_DEFN                                                                       \
    extern "C" {                                                                                   \
    __declspec(allocate("modmeta$a")) constinit unsigned long long mod_meta_bounds_begin = 0;      \
    __declspec(allocate("modmeta$z")) constinit unsigned long long mod_meta_bounds_end = 0;        \
    }
#define MOD_META_BOUNDS_BEGIN (&mod_meta_bounds_begin)
#define MOD_META_BOUNDS_END (&mod_meta_bounds_end)
#elif defined(__APPLE__)
extern "C" const unsigned char mod_meta_bounds_begin[] __asm("section$start$__DATA$__modmeta");
extern "C" const unsigned char mod_meta_bounds_end[] __asm("section$end$__DATA$__modmeta");
#define MOD_META_BOUNDS_DEFN
#define MOD_META_BOUNDS_BEGIN (mod_meta_bounds_begin)
#define MOD_META_BOUNDS_END (mod_meta_bounds_end)
#else
extern "C" const unsigned char __start_modmeta[];
extern "C" const unsigned char __stop_modmeta[];
#define MOD_META_BOUNDS_DEFN
#define MOD_META_BOUNDS_BEGIN (__start_modmeta)
#define MOD_META_BOUNDS_END (__stop_modmeta)
#endif

namespace dusk::mods {

/* A string usable as a template argument: carries a symbol/target name into record builders
 * and makes each hook declaration's static state unique. */
template <size_t N>
struct FixedString {
    char chars[N]{};
    constexpr FixedString(const char (&s)[N]) noexcept {
        for (size_t i = 0; i < N; ++i) {
            chars[i] = s[i];
        }
    }
};

namespace detail {

template <class T>
constexpr std::string_view class_name() {
#if defined(__clang__) || defined(__GNUC__)
    // "... class_name() [T = daAlink_c]" / "... [with T = daAlink_c; ...]"
    constexpr std::string_view fn = __PRETTY_FUNCTION__;
    constexpr size_t start = fn.find("T = ") + 4;
    return fn.substr(start, fn.find_first_of(";]", start) - start);
#elif defined(_MSC_VER)
    // "... class_name<class daAlink_c>(void)"
    constexpr std::string_view fn = __FUNCSIG__;
    constexpr size_t start = fn.find("class_name<") + 11;
    constexpr std::string_view name = fn.substr(start, fn.rfind(">(") - start);
    if constexpr (name.starts_with("class ")) {
        return name.substr(6);
    } else if constexpr (name.starts_with("struct ")) {
        return name.substr(7);
    } else {
        return name;
    }
#else
#error "unsupported compiler"
#endif
}

/* The symbol name of C's vtable. Only unscoped, non-template class names are supported (an
 * empty result makes hooks on virtual members of C fail resolution, which is reported). */
template <class C>
constexpr auto vtable_symbol() {
    constexpr std::string_view name = class_name<C>();
    constexpr bool simple = name.find_first_of(":<> ") == std::string_view::npos;
    // "_ZTV" + decimal length + name / "??_7" + name + "@@6B@", NUL-terminated
    std::array<char, name.size() + 12> out{};
    if constexpr (!simple) {
        return out;
    }
    size_t n = 0;
#if defined(_WIN32)
    for (char c : {'?', '?', '_', '7'}) {
        out[n++] = c;
    }
    for (char c : name) {
        out[n++] = c;
    }
    for (char c : {'@', '@', '6', 'B', '@'}) {
        out[n++] = c;
    }
#else
    for (char c : {'_', 'Z', 'T', 'V'}) {
        out[n++] = c;
    }
    size_t len = name.size();
    char digits[8]{};
    size_t d = 0;
    while (len != 0) {
        digits[d++] = static_cast<char>('0' + len % 10);
        len /= 10;
    }
    while (d != 0) {
        out[n++] = digits[--d];
    }
    for (char c : name) {
        out[n++] = c;
    }
#endif
    return out;
}

template <class F>
struct member_traits;
template <class C, class R, class... A>
struct member_traits<R (C::*)(A...)> {
    using Class = C;
};
template <class C, class R, class... A>
struct member_traits<R (C::*)(A...) const> {
    using Class = C;
};

consteval ModMetaServiceId make_service_id(const char* id) {
    ModMetaServiceId out{};
    size_t n = 0;
    for (; id[n] != '\0'; ++n) {
        if (n + 1 >= MOD_META_SERVICE_ID_SIZE) {
            throw "service id exceeds MOD_META_SERVICE_ID_SIZE";
        }
        out.chars[n] = id[n];
    }
    return out;
}

consteval ModMetaHeader make_header() {
    ModMetaHeader r{};
    r.rec = {sizeof(ModMetaHeader), MOD_META_HEADER, 0};
    r.abi_version = MOD_ABI_VERSION;
    return r;
}

/*
 * Typed record variants: embedding the target as its native type makes the compiler emit the
 * on-disk representation (relocations, PMF slot words) that static parsers read; the layouts
 * match the byte-view structs in api.h.
 */

template <class F>
struct HookFnRecord {
    ModMetaRecord rec;
    uint32_t reserved;
    F target;
    void* resolved;
};

template <class F, size_t N>
struct HookMemRecord {
    ModMetaRecord rec;
    uint32_t reserved;
    union {
        F fn;
        unsigned char raw[16];
    } pmf;
    void* resolved;
    char names[N];
};

template <size_t N>
struct HookNameRecord {
    ModMetaRecord rec;
    uint32_t reserved;
    void* resolved;
    char name[N];
};

template <size_t N>
constexpr size_t align_up(size_t n) {
    return (n + (N - 1)) & ~(N - 1);
}

template <size_t N>
struct HookMemNames {
    char chars[N]{};
    size_t len{};
};

template <auto Target, FixedString Disp>
consteval auto make_hook_mem_names() {
    using C = member_traits<decltype(Target)>::Class;
    // Strip the leading '&' of the stringified target expression for display.
    constexpr size_t dispFrom = Disp.chars[0] == '&' ? 1 : 0;
    constexpr size_t dispLen = sizeof(Disp.chars) - 1 - dispFrom;
    constexpr auto vtbl = vtable_symbol<C>();
    constexpr size_t vtblLen = std::string_view{vtbl.data()}.size();
    HookMemNames<align_up<8>(vtblLen + 1 + dispLen + 1)> r{};
    size_t at = 0;
    for (size_t i = 0; i < vtblLen; ++i) {
        r.chars[at++] = vtbl[i];
    }
    r.chars[at++] = '\0';
    for (size_t i = 0; i < dispLen; ++i) {
        r.chars[at++] = Disp.chars[dispFrom + i];
    }
    r.len = sizeof(r.chars);
    return r;
}

/*
 * MSVC constant-evaluates a pointer-to-member only when every other operand in the
 * initializer is a literal: no consteval calls, constexpr-object copies, or default
 * member initializers.
 */
template <auto Target, char... Cs>
struct HookMemHolder {
    using F = decltype(Target);
    static_assert(sizeof(F) <= 16, "unsupported pointer-to-member representation");
    MOD_META_RECORD static constinit inline HookMemRecord<F, sizeof...(Cs)> record = {
        {sizeof(HookMemRecord<F, sizeof...(Cs)>), MOD_META_HOOK_MEM, 0}, 0, {Target}, nullptr,
        {Cs...}};
};

template <auto Target>
struct HookFnHolder {
    using F = decltype(Target);
    static_assert(std::is_pointer_v<F> && std::is_function_v<std::remove_pointer_t<F>>,
        "hook target must be a function or member function");
    MOD_META_RECORD static constinit inline HookFnRecord<F> record = {
        {sizeof(HookFnRecord<F>), MOD_META_HOOK_FN, 0}, 0, Target, nullptr};
};

template <auto Target, FixedString Disp,
    bool = std::is_member_function_pointer_v<decltype(Target)>>
struct HookRecordFor {
    using Holder = HookFnHolder<Target>;
};

template <auto Target, FixedString Disp>
struct HookRecordFor<Target, Disp, true> {
    template <class Seq>
    struct Bind;
    template <size_t... Is>
    struct Bind<std::index_sequence<Is...>> {
        using Type = HookMemHolder<Target, make_hook_mem_names<Target, Disp>().chars[Is]...>;
    };
    using Holder =
        Bind<std::make_index_sequence<make_hook_mem_names<Target, Disp>().len>>::Type;
};

template <FixedString Name>
consteval auto make_hook_name_record() {
    constexpr size_t len = sizeof(Name.chars) - 1;
    HookNameRecord<align_up<8>(len + 1)> r{};
    r.rec = {sizeof(r), MOD_META_HOOK_NAME, 0};
    for (size_t i = 0; i < len; ++i) {
        r.name[i] = Name.chars[i];
    }
    return r;
}

}  // namespace detail
}  // namespace dusk::mods
