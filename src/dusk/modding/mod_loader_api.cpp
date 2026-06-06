#include <RmlUi/Core.h>

#include "dusk/hook_system.hpp"
#include "dusk/logging.h"
#include "dusk/mod_api.h"
#include "dusk/mod_loader.hpp"
#include "mod_loader.hpp"

using namespace dusk::modding;

namespace dusk::modding {

thread_local LoadedMod* g_currentMod = nullptr;
std::unordered_map<std::string, void*> g_services;

thread_local void* g_dusk_hook_current_mod = nullptr;

}

namespace {

void cb_log_info(const char* fmt, ...) {
    va_list ap, ap2;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    std::string s(vsnprintf(nullptr, 0, fmt, ap2), '\0');
    va_end(ap2);
    vsnprintf(s.data(), s.size() + 1, fmt, ap);
    va_end(ap);
    DuskLog.info("[{}] {}", modName(), s);
}

void cb_log_warn(const char* fmt, ...) {
    va_list ap, ap2;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    std::string s(vsnprintf(nullptr, 0, fmt, ap2), '\0');
    va_end(ap2);
    vsnprintf(s.data(), s.size() + 1, fmt, ap);
    va_end(ap);
    DuskLog.warn("[{}] {}", modName(), s);
}

void cb_log_error(const char* fmt, ...) {
    va_list ap, ap2;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    std::string s(vsnprintf(nullptr, 0, fmt, ap2), '\0');
    va_end(ap2);
    vsnprintf(s.data(), s.size() + 1, fmt, ap);
    va_end(ap);
    DuskLog.error("[{}] {}", modName(), s);
}

void* cb_load_resource(const char* relative_path, size_t* out_size) {
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

void cb_free_resource(void* data) {
    std::free(data);
}

class ModClickListener : public Rml::EventListener {
public:
    ModClickListener(void (*cb)(void*), void* ud) : m_cb(cb), m_ud(ud) {}
    void ProcessEvent(Rml::Event&) override { m_cb(m_ud); }
    void OnDetach(Rml::Element*) override { delete this; }
private:
    void (*m_cb)(void*);
    void* m_ud;
};

std::string escape_rml(const char* text) {
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

void cb_panel_add_section(DuskPanelHandle panel, const char* text) {
    auto* pane = static_cast<Rml::Element*>(panel);
    if (!pane || !text) {
        return;
    }
    auto el = pane->GetOwnerDocument()->CreateElement("div");
    el->SetClass("section-heading", true);
    el->SetInnerRML(escape_rml(text));
    pane->AppendChild(std::move(el));
}

void cb_panel_add_button(DuskPanelHandle panel, const char* label,
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

DuskElemHandle cb_panel_add_badge_row(DuskPanelHandle panel, const char* label, int ok) {
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

DuskElemHandle cb_panel_add_dyn_text(DuskPanelHandle panel, const char* text) {
    auto* pane = static_cast<Rml::Element*>(panel);
    if (!pane) {
        return nullptr;
    }
    auto el = pane->GetOwnerDocument()->CreateElement("div");
    el->SetInnerRML(text ? escape_rml(text) : std::string{});
    Rml::Element* ptr = pane->AppendChild(std::move(el));
    return static_cast<DuskElemHandle>(ptr);
}

void cb_elem_set_badge(DuskElemHandle elem, int ok) {
    auto* el = static_cast<Rml::Element*>(elem);
    if (!el) {
        return;
    }
    el->SetClass("unlocked", ok != 0);
    el->SetClass("locked",   ok == 0);
    el->SetInnerRML(ok ? "PASS" : "WAIT");
}

void cb_elem_set_text(DuskElemHandle elem, const char* text) {
    auto* el = static_cast<Rml::Element*>(elem);
    if (!el || !text) {
        return;
    }
    el->SetInnerRML(escape_rml(text));
}

DuskElemHandle cb_panel_add_progress(DuskPanelHandle panel, float value) {
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

void cb_elem_set_progress(DuskElemHandle elem, float value) {
    auto* el = static_cast<Rml::Element*>(elem);
    if (!el) {
        return;
    }
    el->SetAttribute("value", value);
}

void cb_register_tab_content(void (*build_fn)(void*, void*), void* userdata) {
    if (g_currentMod && build_fn) {
        g_currentMod->tab_content.push_back({build_fn, userdata});
    }
}

void cb_register_tab_update(void (*update_fn)(void*), void* userdata) {
    if (g_currentMod && update_fn) {
        g_currentMod->tab_updates.push_back({update_fn, userdata});
    }
}

void cb_service_publish(const char* name, void* ptr) {
    if (!name) {
        return;
    }
    if (g_services.count(name)) {
        DuskLog.error(
            "[{}] service_publish: '{}' already published by another mod", modName(), name);
    }
    g_services[name] = ptr;
}

void* cb_service_get(const char* name) {
    if (!name) {
        return nullptr;
    }
    auto it = g_services.find(name);
    return it != g_services.end() ? it->second : nullptr;
}

void cb_define_settings(const DuskSetting* settings, uint32_t count) {
    if (g_currentMod && settings) {
        dusk::ModLoader::instance().modDefineSettings(*g_currentMod, settings, count);
    }
}

double cb_setting_get(const char* key) {
    return g_currentMod ? dusk::ModLoader::instance().modGetSetting(*g_currentMod, key) : 0.0;
}

void cb_setting_set(const char* key, double value) {
    if (g_currentMod) {
        dusk::ModLoader::instance().modSetSetting(*g_currentMod, key, value);
    }
}

void api_hook_pre(void* addr, int32_t (*fn)(void* args)) {
    dusk::hookRegisterPre(addr, g_currentMod, fn);
}

void api_hook_post(void* addr, void (*fn)(void* args, void* retval)) {
    dusk::hookRegisterPost(addr, g_currentMod, modName(), fn);
}

void api_hook_replace(void* addr, void (*fn)(void* args, void* retval)) {
    if (!dusk::hookSetReplace(addr, g_currentMod, modName(), fn)) {
        if (g_currentMod) {
            g_currentMod->load_failed = true;
        }
    }
}

}

namespace dusk {
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
    native.api.define_settings = cb_define_settings;
    native.api.setting_get = cb_setting_get;
    native.api.setting_set = cb_setting_set;
}

}