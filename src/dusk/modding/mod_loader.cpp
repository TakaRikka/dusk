#include "dusk/mod_loader.hpp"
#include "dusk/hook_system.hpp"
#include "dusk/logging.h"
#include "mod_loader.hpp"

#include <RmlUi/Core.h>


#include <algorithm>
#include <cstdarg>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>

#include "aurora/dvd.h"
#include "dusk/io.hpp"
#include "miniz.h"
#include "native_module.hpp"
#include "nlohmann/json.hpp"

static aurora::Module Log("dusk::modLoader");

using namespace dusk::modding;
using namespace std::string_literals;
using namespace std::string_view_literals;

#if defined(_M_ARM64) || defined(__aarch64__)
static constexpr std::string_view k_archSuffix = "_arm64"sv;
#elif defined(_M_X64) || defined(__x86_64__)
static constexpr std::string_view k_archSuffix = "_x64"sv;
#elif defined(_M_IX86) || defined(__i386__)
static constexpr std::string_view k_archSuffix = "_x86"sv;
#else
static constexpr std::string_view k_archSuffix = ""sv;
#endif

static thread_local dusk::LoadedMod* g_currentMod = nullptr;
static std::unordered_map<std::string, void*> g_services;

namespace dusk {
thread_local void* g_dusk_hook_current_mod = nullptr;

}  // namespace dusk

struct ModGuard {
    explicit ModGuard(dusk::LoadedMod* m) {
        g_currentMod = m;
        dusk::g_dusk_hook_current_mod = m;
    }
    ~ModGuard() {
        g_currentMod = nullptr;
        dusk::g_dusk_hook_current_mod = nullptr;
    }
};

static const char* modName() {
    return g_currentMod ? g_currentMod->metadata.id.c_str() : "mod";
}

static void cb_log_info(const char* fmt, ...) {
    va_list ap, ap2;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    std::string s(vsnprintf(nullptr, 0, fmt, ap2), '\0');
    va_end(ap2);
    vsnprintf(s.data(), s.size() + 1, fmt, ap);
    va_end(ap);
    DuskLog.info("[{}] {}", modName(), s);
}

static void cb_log_warn(const char* fmt, ...) {
    va_list ap, ap2;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    std::string s(vsnprintf(nullptr, 0, fmt, ap2), '\0');
    va_end(ap2);
    vsnprintf(s.data(), s.size() + 1, fmt, ap);
    va_end(ap);
    DuskLog.warn("[{}] {}", modName(), s);
}

static void cb_log_error(const char* fmt, ...) {
    va_list ap, ap2;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    std::string s(vsnprintf(nullptr, 0, fmt, ap2), '\0');
    va_end(ap2);
    vsnprintf(s.data(), s.size() + 1, fmt, ap);
    va_end(ap);
    DuskLog.error("[{}] {}", modName(), s);
}

static void* cb_load_resource(const char* relative_path, size_t* out_size) {
    if (out_size) {
        *out_size = 0;
    }
    if (!g_currentMod || !relative_path) {
        DuskLog.error("load_resource: called outside mod context or with null path");
        return nullptr;
    }

    std::string entry = std::string("res/") + relative_path;
    std::vector<u8> data;
    try {
        data = g_currentMod->bundle->readFile(entry);
    } catch (const std::runtime_error& e) {
        DuskLog.error("[{}] load_resource: '{}' failed: {}", g_currentMod->metadata.id, entry, e.what());
        return nullptr;
    }

    const auto retPtr = std::malloc(data.size());
    std::memcpy(retPtr, data.data(), data.size());

    if (out_size) {
        *out_size = data.size();
    }
    return retPtr;
}

static void cb_free_resource(void* data) {
    std::free(data);
}

namespace {

class ModClickListener : public Rml::EventListener {
public:
    ModClickListener(void (*cb)(void*), void* ud) : m_cb(cb), m_ud(ud) {}
    void ProcessEvent(Rml::Event&) override { m_cb(m_ud); }
    void OnDetach(Rml::Element*) override { delete this; }
private:
    void (*m_cb)(void*);
    void* m_ud;
};

static std::string escape_rml(const char* text) {
    std::string out;
    for (const char* p = text; *p; ++p) {
        switch (*p) {
        case '&': out += "&amp;"; break;
        case '<': out += "&lt;";  break;
        case '>': out += "&gt;";  break;
        default:  out += *p;      break;
        }
    }
    return out;
}

}

static void cb_panel_add_section(DuskPanelHandle panel, const char* text) {
    auto* pane = static_cast<Rml::Element*>(panel);
    if (!pane || !text) {
        return;
    }
    auto el = pane->GetOwnerDocument()->CreateElement("div");
    el->SetClass("section-heading", true);
    el->SetInnerRML(escape_rml(text));
    pane->AppendChild(std::move(el));
}

static void cb_panel_add_button(DuskPanelHandle panel, const char* label,
                                void (*cb)(void*), void* userdata) {
    auto* pane = static_cast<Rml::Element*>(panel);
    if (!pane || !label || !cb) {
        return;
    }
    auto btn = pane->GetOwnerDocument()->CreateElement("button");
    btn->SetInnerRML(escape_rml(label));
    btn->AddEventListener(Rml::EventId::Click, new ModClickListener(cb, userdata));
    pane->AppendChild(std::move(btn));
}

static DuskElemHandle cb_panel_add_badge_row(DuskPanelHandle panel, const char* label, int ok) {
    auto* pane = static_cast<Rml::Element*>(panel);
    if (!pane || !label) {
        return nullptr;
    }
    auto* doc = pane->GetOwnerDocument();

    auto row = doc->CreateElement("div");
    row->SetClass("mod-info-row", true);

    auto badge = doc->CreateElement("span");
    badge->SetClass("achievement-badge", true);
    badge->SetClass(ok ? "unlocked" : "locked", true);
    badge->SetInnerRML(ok ? "PASS" : "WAIT");
    Rml::Element* badgePtr = row->AppendChild(std::move(badge));

    auto lbl = doc->CreateElement("span");
    lbl->SetClass("mod-info-value", true);
    lbl->SetInnerRML(escape_rml(label));
    row->AppendChild(std::move(lbl));

    pane->AppendChild(std::move(row));
    return static_cast<DuskElemHandle>(badgePtr);
}

static DuskElemHandle cb_panel_add_dyn_text(DuskPanelHandle panel, const char* text) {
    auto* pane = static_cast<Rml::Element*>(panel);
    if (!pane) {
        return nullptr;
    }
    auto el = pane->GetOwnerDocument()->CreateElement("div");
    el->SetInnerRML(text ? escape_rml(text) : std::string{});
    Rml::Element* ptr = pane->AppendChild(std::move(el));
    return static_cast<DuskElemHandle>(ptr);
}

static void cb_elem_set_badge(DuskElemHandle elem, int ok) {
    auto* el = static_cast<Rml::Element*>(elem);
    if (!el) {
        return;
    }
    el->SetClass("unlocked", ok != 0);
    el->SetClass("locked",   ok == 0);
    el->SetInnerRML(ok ? "PASS" : "WAIT");
}

static void cb_elem_set_text(DuskElemHandle elem, const char* text) {
    auto* el = static_cast<Rml::Element*>(elem);
    if (!el || !text) {
        return;
    }
    el->SetInnerRML(escape_rml(text));
}

static DuskElemHandle cb_panel_add_progress(DuskPanelHandle panel, float value) {
    auto* pane = static_cast<Rml::Element*>(panel);
    if (!pane) {
        return nullptr;
    }
    auto el = pane->GetOwnerDocument()->CreateElement("progress");
    el->SetClass("progress-health", true);
    el->SetAttribute("value", value);
    Rml::Element* ptr = pane->AppendChild(std::move(el));
    return static_cast<DuskElemHandle>(ptr);
}

static void cb_elem_set_progress(DuskElemHandle elem, float value) {
    auto* el = static_cast<Rml::Element*>(elem);
    if (!el) {
        return;
    }
    el->SetAttribute("value", value);
}

static void cb_register_tab_content(void (*build_fn)(void*, void*), void* userdata) {
    if (g_currentMod && build_fn) {
        g_currentMod->tab_content.push_back({build_fn, userdata});
    }
}

static void cb_register_tab_update(void (*update_fn)(void*), void* userdata) {
    if (g_currentMod && update_fn) {
        g_currentMod->tab_updates.push_back({update_fn, userdata});
    }
}

static void cb_service_publish(const char* name, void* ptr) {
    if (!name) {
        return;
    }
    if (g_services.count(name)) {
        DuskLog.error(
            "[{}] service_publish: '{}' already published by another mod", modName(), name);
    }
    g_services[name] = ptr;
}

static void* cb_service_get(const char* name) {
    if (!name) {
        return nullptr;
    }
    auto it = g_services.find(name);
    return it != g_services.end() ? it->second : nullptr;
}

static void api_hook_pre(void* addr, int32_t (*fn)(void* args)) {
    dusk::hookRegisterPre(addr, g_currentMod, fn);
}

static void api_hook_post(void* addr, void (*fn)(void* args, void* retval)) {
    dusk::hookRegisterPost(addr, g_currentMod, modName(), fn);
}

static void api_hook_replace(void* addr, void (*fn)(void* args, void* retval)) {
    if (!dusk::hookSetReplace(addr, g_currentMod, modName(), fn)) {
        if (g_currentMod) {
            g_currentMod->load_failed = true;
        }
    }
}

static dusk::ModLoader g_modLoader;

namespace dusk {

ModLoader& ModLoader::instance() {
    return g_modLoader;
}

void ModLoader::buildAPI(LoadedMod& mod) {
    auto& native = *mod.native;
    native.api.api_version = DUSK_MOD_API_VERSION;
    native.api.mod_dir = mod.dir.c_str();
    native.api.log_info = cb_log_info;
    native.api.log_warn = cb_log_warn;
    native.api.log_error = cb_log_error;
    native.api.load_resource = cb_load_resource;
    native.api.free_resource = cb_free_resource;
    native.api.register_tab_content = cb_register_tab_content;
    native.api.register_tab_update = cb_register_tab_update;
    native.api.panel_add_section   = cb_panel_add_section;
    native.api.panel_add_button    = cb_panel_add_button;
    native.api.panel_add_badge_row = cb_panel_add_badge_row;
    native.api.panel_add_dyn_text  = cb_panel_add_dyn_text;
    native.api.elem_set_badge      = cb_elem_set_badge;
    native.api.elem_set_text       = cb_elem_set_text;
    native.api.panel_add_progress  = cb_panel_add_progress;
    native.api.elem_set_progress   = cb_elem_set_progress;
    native.api.hook_install = hookInstallByAddr;
    native.api.hook_pre = api_hook_pre;
    native.api.hook_post = api_hook_post;
    native.api.hook_replace = api_hook_replace;
    native.api.hook_dispatch_pre = hookDispatchPre;
    native.api.hook_dispatch_post = hookDispatchPost;
    native.api.service_publish = cb_service_publish;
    native.api.service_get = cb_service_get;
}

static std::unique_ptr<ModBundle> loadBundle(const std::filesystem::path& modPath, bool fromDir) {
    if (fromDir) {
        return std::make_unique<ModBundleDisk>(modPath);
    } else {
        std::vector<u8> data = io::FileStream::ReadAllBytes(modPath);
        return std::make_unique<ModBundleZip>(std::move(data));
    }
}

struct DllLocateResult {
    std::string primary;
    std::string fallback;
};

static std::string_view getFileNameWithoutExtension(const std::string_view fileName) {
    return fileName.substr(0, fileName.find_last_of('.'));
}

static DllLocateResult LocateDllInBundle(ModBundle& bundle) {
    std::string dllEntry, dllFallback;
    for (const auto name : bundle.getFileNames()) {
        if (!name.ends_with(".dll"sv) && !name.ends_with(".dylib"sv) && !name.ends_with(".so"sv)) {
            continue;
        }

        if (!k_archSuffix.empty() && getFileNameWithoutExtension(name).ends_with(k_archSuffix)) {
            dllEntry = name;
        } else if (dllFallback.empty()) {
            dllFallback = name;
        }
    }

    return DllLocateResult{dllEntry, dllFallback};
}

class InvalidModDataException : public std::runtime_error {
public:
    explicit InvalidModDataException(const std::string& msg) : runtime_error(msg) {}
    explicit InvalidModDataException(const char* msg) : runtime_error(msg) {}
};

static ModMetadata loadMetadata(const std::filesystem::path& modPath, ModBundle& bundle) {
    const auto metaJson = bundle.readFile("mod.json");
    auto j = nlohmann::json::parse(metaJson);

    std::string metaId = j.value("id", "");
    std::string metaName = j.value("name", "");
    std::string metaVersion = j.value("version", "");
    std::string metaAuthor = j.value("author", "");
    std::string metaDescription = j.value("description", "");
    const bool hasCode = j.value("has_code", false);

    if (metaId.empty()) {
        throw InvalidModDataException("Missing ID value in mod metadata!");
    }
    if (metaName.empty()) {
        metaName = io::fs_path_to_string(modPath.stem());
    }
    if (metaVersion.empty()) {
        metaVersion = "?"s;
    }
    if (metaAuthor.empty()) {
        metaAuthor = "unknown"s;
    }

    return ModMetadata{
        std::move(metaId),
        std::move(metaName),
        std::move(metaVersion),
        std::move(metaAuthor),
        std::move(metaDescription),
        hasCode,
    };
}

static bool checkDuplicateMod(const ModMetadata& metadata, const std::vector<LoadedMod>& mods) {
    return std::ranges::any_of(mods, [&](const LoadedMod& mod) {
        return mod.metadata.id == metadata.id;
    });
}

bool ModLoader::tryLoadNativeMod(LoadedMod& mod) {
    namespace fs = std::filesystem;

    auto [dllEntry, dllFallback] = LocateDllInBundle(*mod.bundle);
    if (dllEntry.empty()) {
        dllEntry = dllFallback;
    }

    if (dllEntry.empty()) {
        DuskLog.error(
            "ModLoader: no *{} found in {} — skipping", NativeModule::LibraryExtension, mod.metadata.id);
        return false;
    }

    const fs::path cacheDir = m_modsDir / ".cache" / mod.metadata.id;
    std::error_code ec;
    fs::create_directories(cacheDir, ec);

    const fs::path dllCachePath = cacheDir / fs::path(dllEntry).filename();

    std::vector<u8> dllData;
    try {
        dllData = mod.bundle->readFile(dllEntry);
    } catch (const std::runtime_error& e) {
        DuskLog.error(
            "ModLoader: failed to extract {} from {}", dllEntry, mod.metadata.id);
        return false;
    }

    {
        std::ofstream out(dllCachePath, std::ios::binary | std::ios::out);
        if (!out) {
            DuskLog.error("ModLoader: failed to write {}", io::fs_path_to_string(dllCachePath));
            return false;
        }

        out.write(
            reinterpret_cast<const char*>(dllData.data()),
            static_cast<std::streamsize>(dllData.size()));
    }

    auto nativeMod = std::make_unique<NativeMod>();
    try {
        nativeMod->handle = std::make_unique<NativeModule>(dllCachePath);
    } catch (const std::runtime_error& e) {
        DuskLog.error("ModLoader: failed to open {}: {}", io::fs_path_to_string(dllCachePath), e.what());
        return false;
    }

    const auto mod_api_ver = nativeMod->handle->LookupSymbol<uint32_t*>("mod_api_version");
    if (mod_api_ver && *mod_api_ver != DUSK_MOD_API_VERSION) {
        DuskLog.error("ModLoader: {} expects API v{} but engine is v{}, skipping",
            io::fs_path_to_string(fs::path(dllEntry).filename()), *mod_api_ver, DUSK_MOD_API_VERSION);
        return false;
    }

    nativeMod->fn_init = nativeMod->handle->LookupSymbol<NativeMod::FnInit>("mod_init");
    nativeMod->fn_tick = nativeMod->handle->LookupSymbol<NativeMod::FnTick>("mod_tick");
    nativeMod->fn_cleanup = nativeMod->handle->LookupSymbol<NativeMod::FnCleanup>("mod_cleanup");

    if (!nativeMod->fn_init || !nativeMod->fn_tick) {
        DuskLog.error("ModLoader: {} missing mod_init or mod_tick — skipping",
            io::fs_path_to_string(fs::path(dllEntry).filename()));
        return false;
    }

    mod.dir = io::fs_path_to_string(fs::absolute(cacheDir));
    mod.native = std::move(nativeMod);
    return true;
}

void ModLoader::tryLoadDusk(const std::filesystem::path& modPath, bool fromDir) {
    namespace fs = std::filesystem;

    std::unique_ptr<ModBundle> bundle;
    try {
        bundle = loadBundle(modPath, fromDir);
    } catch (const std::runtime_error& e) {
        Log.error("Failed to open {} bundle: {}", io::fs_path_to_string(modPath.filename()), e.what());
        return;
    }

    ModMetadata metadata;
    try
    {
        metadata = loadMetadata(modPath, *bundle);
    }
    catch (const std::runtime_error& e) {
        Log.error(
            "ModLoader: bad mod.json in {}: {}", io::fs_path_to_string(modPath.filename()), e.what());
        return;
    }

    if (checkDuplicateMod(metadata, m_mods)) {
        Log.error(
            "ModLoader: mod with id '{}' already exists, not loading {}",
            metadata.id,
            io::fs_path_to_string(modPath.filename()));
        return;
    }

    LoadedMod mod;
    mod.mod_path = io::fs_path_to_string(fs::absolute(modPath));
    mod.metadata = std::move(metadata);
    mod.bundle = std::move(bundle);

    if (mod.metadata.hasCode && !tryLoadNativeMod(mod)) {
        return;
    }

    const auto& inserted = m_mods.emplace_back(std::move(mod));

    DuskLog.info(
        "ModLoader: found '{}' ('{}') v{} by {} ({})",
        inserted.metadata.name,
        inserted.metadata.id,
        inserted.metadata.version,
        inserted.metadata.author,
        io::fs_path_to_string(modPath.filename()));
}

void ModLoader::init() {
    if (m_initialized) {
        return;
    }
    m_initialized = true;

    namespace fs = std::filesystem;
    if (!fs::is_directory(m_modsDir)) {
        DuskLog.info(
            "ModLoader: mods directory '{}' not found — mod loading skipped", io::fs_path_to_string(m_modsDir));
        return;
    }

    std::error_code ec;
    std::vector<fs::directory_entry> entries;
    for (auto& e : fs::directory_iterator(m_modsDir, ec)) {
        if (e.is_directory() && std::filesystem::exists(e.path() / "mod.json")) {
            entries.push_back(e);
        } else if (e.is_regular_file() && e.path().extension() == ".dusk") {
            entries.push_back(e);
        }
    }
    std::sort(entries.begin(), entries.end(),
        [](const fs::directory_entry& a, const fs::directory_entry& b) {
            return a.path().filename() < b.path().filename();
        });

    m_mods.reserve(entries.size());
    for (auto& entry : entries) {
        tryLoadDusk(entry.path(), entry.is_directory());
    }

    if (m_mods.empty()) {
        DuskLog.info("ModLoader: no mods found");
        return;
    }

    initOverlayFiles();

    DuskLog.info("ModLoader: initializing {} mod(s)...", m_mods.size());
    for (auto& mod : m_mods) {
        if (mod.native) {
            buildAPI(mod);
        }
    }

    for (auto& mod : m_mods) {
        if (!mod.native) {
            continue;
        }

        ModGuard guard(&mod);
        try {
            mod.native->fn_init(&mod.native->api);
            if (!mod.load_failed) {
                mod.active = true;
                DuskLog.info("ModLoader: '{}' initialized", mod.metadata.id);
            } else {
                DuskLog.error("ModLoader: '{}' failed to load due to hook conflicts", mod.metadata.id);
            }
        } catch (const std::exception& e) {
            DuskLog.error("ModLoader: exception in {}.mod_init(): {}", mod.metadata.id, e.what());
        } catch (...) {
            DuskLog.error("ModLoader: unknown exception in {}.mod_init()", mod.metadata.id);
        }
    }

    auto active =
        std::count_if(m_mods.begin(), m_mods.end(), [](const LoadedMod& m) { return m.active; });
    DuskLog.info("ModLoader: {}/{} mod(s) active", active, m_mods.size());
}

void ModLoader::tick() {
    for (auto& mod : m_mods) {
        if (!mod.active || !mod.native) {
            continue;
        }
        ModGuard guard(&mod);
        try {
            mod.native->fn_tick(&mod.native->api);
        } catch (const std::exception& e) {
            DuskLog.error(
                "ModLoader: exception in {}.mod_tick(): {} — disabling", mod.metadata.id, e.what());
            mod.active = false;
        } catch (...) {
            DuskLog.error("ModLoader: unknown exception in {}.mod_tick() — disabling", mod.metadata.id);
            mod.active = false;
        }
    }
}

void ModLoader::shutdown() {
    for (auto& mod : m_mods) {
        hookClearMod(&mod);
        if (mod.native && mod.native->fn_cleanup) {
            ModGuard guard(&mod);
            try {
                mod.native->fn_cleanup(&mod.native->api);
            } catch (...) {
            }
        }
    }
    m_mods.clear();
    g_services.clear();
    DuskLog.info("ModLoader: all mods unloaded");
}

}  // namespace dusk
