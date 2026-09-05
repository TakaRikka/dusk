#include "runtime.hpp"

#include "Luau/Common.h"
#include "luacode.h"
#include "mods/runtime.h"
#include "mods/service.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <new>
#include <optional>
#include <string_view>
#include <utility>

LUAU_FASTFLAG(LuauIntegerLibrary)
LUAU_FASTFLAG(LuauIntegerType2)

DEFINE_MOD();
IMPORT_OPTIONAL_SERVICE(LogService, svc_log);
IMPORT_OPTIONAL_SERVICE(HostService, svc_host);
IMPORT_OPTIONAL_SERVICE(ConfigService, svc_config);
IMPORT_OPTIONAL_SERVICE(ResourceService, svc_resource);
IMPORT_OPTIONAL_SERVICE(OverlayService, svc_overlay);
IMPORT_OPTIONAL_SERVICE(TextureService, svc_texture);
IMPORT_OPTIONAL_SERVICE(UiService, svc_ui);

namespace luau_runtime {
namespace {

std::unordered_map<ModContext*, std::unique_ptr<Vm>> s_vms;

void* limited_realloc(void* userData, void* pointer, size_t oldSize, size_t newSize) {
    auto& budget = *static_cast<MemoryBudget*>(userData);
    if (newSize == 0) {
        std::free(pointer);
        budget.used -= std::min(budget.used, oldSize);
        return nullptr;
    }

    const size_t growth = newSize > oldSize ? newSize - oldSize : 0;
    if (growth > budget.limit - std::min(budget.used, budget.limit)) {
        return nullptr;
    }
    void* result = std::realloc(pointer, newSize);
    if (result != nullptr) {
        budget.used -= std::min(budget.used, oldSize);
        budget.used += newSize;
    }
    return result;
}

int traceback_handler(lua_State* state) {
    const char* message = lua_tostring(state, 1);
    luaL_traceback(state, state, message != nullptr ? message : "Luau error", 1);
    return 1;
}

class DeadlineScope {
public:
    DeadlineScope(Vm& vm, std::chrono::steady_clock::duration budget)
        : m_vm{vm}, m_previousDeadline{vm.deadline}, m_previousActive{vm.deadlineActive} {
        const auto requested = std::chrono::steady_clock::now() + budget;
        if (!vm.deadlineActive || requested < vm.deadline) {
            vm.deadline = requested;
        }
        vm.deadlineActive = true;
        ++vm.callDepth;
    }

    ~DeadlineScope() {
        --m_vm.callDepth;
        m_vm.deadline = m_previousDeadline;
        m_vm.deadlineActive = m_previousActive;
    }

private:
    Vm& m_vm;
    std::chrono::steady_clock::time_point m_previousDeadline;
    bool m_previousActive;
};

bool protected_call(lua_State* state, Vm& vm, int argumentCount, int resultCount,
    std::chrono::steady_clock::duration budget, std::string& outError) {
    const int functionIndex = lua_gettop(state) - argumentCount;
    lua_pushcfunction(state, traceback_handler, "dusklight traceback");
    lua_insert(state, functionIndex);

    DeadlineScope deadline{vm, budget};
    const int status = lua_pcall(state, argumentCount, resultCount, functionIndex);
    if (status != LUA_OK) {
        const char* message = lua_tostring(state, -1);
        outError = message != nullptr ? message : "unknown Luau error";
        lua_pop(state, 1);
        lua_remove(state, functionIndex);
        return false;
    }
    lua_remove(state, functionIndex);
    return true;
}

std::optional<std::string> normalize_module_path(
    std::string_view currentPath, std::string_view requested) {
    if ((!requested.starts_with("./") && !requested.starts_with("../")) ||
        requested.find('\\') != std::string_view::npos)
    {
        return std::nullopt;
    }

    std::vector<std::string_view> parts;
    const auto parentEnd = currentPath.rfind('/');
    std::string combined;
    if (parentEnd != std::string_view::npos) {
        combined.assign(currentPath.substr(0, parentEnd + 1));
    }
    combined.append(requested);

    size_t begin = 0;
    while (begin <= combined.size()) {
        const size_t end = combined.find('/', begin);
        const auto part = std::string_view{combined}.substr(
            begin, end == std::string::npos ? combined.size() - begin : end - begin);
        if (part.empty() || part == ".") {
        } else if (part == "..") {
            if (parts.empty()) {
                return std::nullopt;
            }
            parts.pop_back();
        } else {
            parts.push_back(part);
        }
        if (end == std::string::npos) {
            break;
        }
        begin = end + 1;
    }
    if (parts.empty()) {
        return std::nullopt;
    }

    std::string normalized;
    for (const auto part : parts) {
        if (!normalized.empty()) {
            normalized.push_back('/');
        }
        normalized.append(part);
    }
    if (!normalized.ends_with(".luau")) {
        normalized += ".luau";
    }
    return normalized;
}

ModuleOpenFn module_factory(std::string_view name) {
    if (name == "dusklight.log") {
        return open_log;
    }
    if (name == "dusklight.host") {
        return open_host;
    }
    if (name == "dusklight.resource") {
        return open_resource;
    }
    if (name == "dusklight.overlay") {
        return open_overlay;
    }
    if (name == "dusklight.texture") {
        return open_texture;
    }
    if (name == "dusklight.config") {
        return open_config;
    }
    if (name == "dusklight.ui") {
        return open_ui;
    }
    return nullptr;
}

int module_require(lua_State* state);

void install_module_require(lua_State* state, Vm& vm, std::string_view currentPath) {
    lua_pushlightuserdata(state, &vm);
    lua_pushlstring(state, currentPath.data(), currentPath.size());
    lua_pushcclosure(state, module_require, "require", 2);
    lua_setglobal(state, "require");
}

bool load_source_module(
    lua_State* caller, Vm& vm, const std::string& path, bool keepResult, std::string& outError) {
    if (svc_resource == nullptr) {
        outError = "ResourceService is not available in this Dusklight build";
        return false;
    }

    ResourceBuffer buffer = RESOURCE_BUFFER_INIT;
    const ModResult loadResult = svc_resource->load(vm.subject, path.c_str(), &buffer);
    if (loadResult != MOD_OK) {
        outError = loadResult == MOD_UNAVAILABLE ? "module not found: res/" + path :
                                                   "failed to load module: res/" + path;
        return false;
    }
    std::string source;
    if (buffer.data != nullptr) {
        source.assign(static_cast<const char*>(buffer.data), buffer.size);
    }
    svc_resource->free(vm.subject, &buffer);

    size_t bytecodeSize = 0;
    lua_CompileOptions options{};
    options.optimizationLevel = 1;
    options.debugLevel = 1;
    char* bytecode = luau_compile(source.data(), source.size(), &options, &bytecodeSize);
    if (bytecode == nullptr) {
        outError = "Luau compiler ran out of memory";
        return false;
    }

    lua_State* moduleState = lua_newthread(caller);
    if (moduleState == nullptr) {
        std::free(bytecode);
        outError = "Luau VM ran out of memory";
        return false;
    }
    luaL_sandboxthread(moduleState);
    install_module_require(moduleState, vm, path);

    const std::string chunkName = "@res/" + path;
    const int loadStatus = luau_load(moduleState, chunkName.c_str(), bytecode, bytecodeSize, 0);
    std::free(bytecode);
    if (loadStatus != LUA_OK) {
        const char* message = lua_tostring(moduleState, -1);
        outError = message != nullptr ? message : "failed to load Luau bytecode";
        lua_pop(caller, 1);
        return false;
    }
    if (!protected_call(moduleState, vm, 0, keepResult ? 1 : 0, kLifecycleBudget, outError)) {
        lua_pop(caller, 1);
        return false;
    }
    if (!keepResult) {
        lua_pop(caller, 1);
        return true;
    }
    if (lua_isnil(moduleState, -1)) {
        outError = "module res/" + path + " must return a value";
        lua_pop(caller, 1);
        return false;
    }

    lua_xmove(moduleState, caller, 1);
    lua_remove(caller, -2);
    return true;
}

int module_require(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    const std::string requested = luaL_checkstring(state, 1);

    std::string moduleName;
    ModuleOpenFn factory = nullptr;
    if (requested.starts_with("dusklight.")) {
        moduleName = requested;
        factory = module_factory(moduleName);
        if (factory == nullptr) {
            luaL_error(state, "unknown module '%s'", moduleName.c_str());
        }
    } else {
        const char* currentPath = lua_tostring(state, lua_upvalueindex(2));
        const auto normalized =
            normalize_module_path(currentPath != nullptr ? currentPath : "main.luau", requested);
        if (!normalized.has_value()) {
            luaL_error(
                state, "module paths must be relative and remain inside the mod's res directory");
        }
        moduleName = *normalized;
    }

    if (const auto found = vm.moduleRefs.find(moduleName); found != vm.moduleRefs.end()) {
        lua_getref(state, found->second);
        return 1;
    }
    if (!vm.loadingModules.insert(moduleName).second) {
        luaL_error(state, "cyclic require of '%s'", moduleName.c_str());
    }

    std::string error;
    if (factory != nullptr) {
        factory(state);
    } else if (!load_source_module(state, vm, moduleName, true, error)) {
        vm.loadingModules.erase(moduleName);
        luaL_error(state, "%s", error.c_str());
    }
    vm.loadingModules.erase(moduleName);

    const int ref = lua_ref(state, -1);
    vm.moduleRefs.emplace(moduleName, ref);
    return 1;
}

ModResult runtime_activate(ModContext*, ModContext* subject, ModError* outError) {
    if (subject == nullptr) {
        return set_error(outError, MOD_INVALID_ARGUMENT, "Delegated mod context is null");
    }
    if (s_vms.contains(subject)) {
        return set_error(outError, MOD_CONFLICT, "Delegated mod is already active");
    }

    auto vm = std::make_unique<Vm>();
    vm->subject = subject;
    vm->state = lua_newstate(limited_realloc, &vm->memory);
    if (vm->state == nullptr) {
        return set_error(outError, MOD_ERROR, "Failed to create Luau VM");
    }
    lua_callbacks(vm->state)->userdata = vm.get();
    lua_callbacks(vm->state)->interrupt = [](lua_State* state, int gc) {
        auto* current = static_cast<Vm*>(lua_callbacks(state)->userdata);
        if (gc < 0 && current != nullptr && current->deadlineActive &&
            std::chrono::steady_clock::now() > current->deadline)
        {
            luaL_error(state, "script execution exceeded its time budget");
        }
    };
    luaL_openlibs(vm->state);
    luaL_sandbox(vm->state);

    std::string error;
    vm->loadingModules.insert("main.luau");
    const bool loadedMain = load_source_module(vm->state, *vm, "main.luau", false, error);
    vm->loadingModules.erase("main.luau");
    if (!loadedMain) {
        return set_error(outError, MOD_ERROR, error);
    }

    s_vms.emplace(subject, std::move(vm));
    return MOD_OK;
}

ModResult runtime_update(ModContext*, ModContext* subject, ModError* outError) {
    const auto found = s_vms.find(subject);
    if (found == s_vms.end()) {
        return set_error(outError, MOD_INVALID_ARGUMENT, "Delegated mod is not active");
    }
    Vm& vm = *found->second;
    const size_t callbackCount = vm.updateRefs.size();
    if (callbackCount == 0) {
        return MOD_OK;
    }

    DeadlineScope deadline{vm, kUpdateBudget};
    std::string error;
    for (size_t i = 0; i < callbackCount; ++i) {
        const int ref = vm.updateRefs[i];
        if (!call_ref(vm, ref, 0, 0, kUpdateBudget, error)) {
            return set_error(outError, MOD_ERROR, error);
        }
    }
    return MOD_OK;
}

ModResult runtime_deactivate(ModContext*, ModContext* subject, ModError*) {
    const auto found = s_vms.find(subject);
    if (found == s_vms.end()) {
        return MOD_OK;
    }

    {
        Vm& vm = *found->second;
        DeadlineScope deadline{vm, kLifecycleBudget};
        const size_t callbackCount = vm.shutdownRefs.size();
        for (size_t i = callbackCount; i > 0; --i) {
            std::string error;
            const int ref = vm.shutdownRefs[i - 1];
            if (!call_ref(vm, ref, 0, 0, kLifecycleBudget, error) && svc_log != nullptr) {
                svc_log->write(subject, LOG_LEVEL_ERROR, error.c_str());
            }
        }
    }
    s_vms.erase(found);
    return MOD_OK;
}

constexpr ModRuntimeService s_runtimeService{
    .header = SERVICE_HEADER(ModRuntimeService, 1, 0),
    .activate = runtime_activate,
    .update = runtime_update,
    .deactivate = runtime_deactivate,
};
EXPORT_SERVICE_AS(s_runtimeService, "dev.twilitrealm.luau");

}  // namespace

Vm::~Vm() {
    if (state != nullptr) {
        lua_close(state);
    }
}

Vm& vm_from_upvalue(lua_State* state) {
    auto* vm = static_cast<Vm*>(lua_tolightuserdata(state, lua_upvalueindex(1)));
    if (vm == nullptr) {
        luaL_error(state, "missing Luau runtime context");
    }
    return *vm;
}

void push_vm_closure(lua_State* state, Vm& vm, lua_CFunction function, const char* name) {
    lua_pushlightuserdata(state, &vm);
    lua_pushcclosure(state, function, name, 1);
}

void set_function(lua_State* state, Vm& vm, const char* name, lua_CFunction function) {
    push_vm_closure(state, vm, function, name);
    lua_setfield(state, -2, name);
}

[[noreturn]] void service_unavailable(lua_State* state, const char* name) {
    luaL_error(state, "%s is not available in this Dusklight build", name);
}

void check_result(lua_State* state, ModResult result, const char* operation) {
    if (result == MOD_OK) {
        return;
    }
    const char* resultName = "error";
    switch (result) {
    case MOD_UNAVAILABLE:
        resultName = "unavailable";
        break;
    case MOD_UNSUPPORTED:
        resultName = "unsupported";
        break;
    case MOD_CONFLICT:
        resultName = "conflict";
        break;
    case MOD_INVALID_ARGUMENT:
        resultName = "invalid argument";
        break;
    default:
        break;
    }
    luaL_error(state, "%s failed: %s", operation, resultName);
}

bool get_optional_bool(lua_State* state, int table, const char* field, bool fallback) {
    lua_getfield(state, table, field);
    const bool value = lua_isnil(state, -1) ? fallback : luaL_checkboolean(state, -1) != 0;
    lua_pop(state, 1);
    return value;
}

bool to_int64(lua_State* state, int index, int64_t& outValue) {
    if (lua_isinteger64(state, index)) {
        outValue = lua_tointeger64(state, index, nullptr);
        return true;
    }
    if (!lua_isnumber(state, index)) {
        return false;
    }
    const double value = lua_tonumber(state, index);
    constexpr double kMaxSafeInteger = 9007199254740991.0;
    if (!std::isfinite(value) || value < -kMaxSafeInteger || value > kMaxSafeInteger ||
        std::trunc(value) != value)
    {
        return false;
    }
    outValue = static_cast<int64_t>(value);
    return true;
}

int64_t check_int64(lua_State* state, int index) {
    int64_t value = 0;
    if (!to_int64(state, index, value)) {
        luaL_argerror(state, index, "integer value expected");
    }
    return value;
}

int64_t get_optional_int(lua_State* state, int table, const char* field, int64_t fallback) {
    lua_getfield(state, table, field);
    const int64_t value = lua_isnil(state, -1) ? fallback : check_int64(state, -1);
    lua_pop(state, 1);
    return value;
}

double get_optional_number(lua_State* state, int table, const char* field, double fallback) {
    lua_getfield(state, table, field);
    const double value = lua_isnil(state, -1) ? fallback : luaL_checknumber(state, -1);
    lua_pop(state, 1);
    return value;
}

std::string get_optional_string(
    lua_State* state, int table, const char* field, std::string fallback) {
    lua_getfield(state, table, field);
    if (!lua_isnil(state, -1)) {
        size_t length = 0;
        const char* value = luaL_checklstring(state, -1, &length);
        fallback.assign(value, length);
    }
    lua_pop(state, 1);
    return fallback;
}

int ref_optional_function(lua_State* state, int table, const char* field) {
    lua_getfield(state, table, field);
    if (lua_isnil(state, -1)) {
        lua_pop(state, 1);
        return LUA_NOREF;
    }
    luaL_argexpected(state, lua_isfunction(state, -1), table, "function field");
    const int ref = lua_ref(state, -1);
    lua_pop(state, 1);
    return ref;
}

int ref_required_function(lua_State* state, int table, const char* field) {
    const int ref = ref_optional_function(state, table, field);
    if (ref == LUA_NOREF) {
        luaL_error(state, "field '%s' is required", field);
    }
    return ref;
}

Callback& retain_callback(Vm& vm) {
    auto callback = std::make_unique<Callback>();
    callback->vm = &vm;
    callback->refs.fill(LUA_NOREF);
    vm.callbacks.push_back(std::move(callback));
    return *vm.callbacks.back();
}

bool call_ref(Vm& vm, int ref, int argumentCount, int resultCount,
    std::chrono::steady_clock::duration budget, std::string& outError) {
    lua_State* state = vm.state;
    lua_getref(state, ref);
    if (!lua_isfunction(state, -1)) {
        lua_pop(state, 1);
        outError = "callback is no longer a function";
        return false;
    }
    if (argumentCount != 0) {
        lua_insert(state, -argumentCount - 1);
    }
    return protected_call(state, vm, argumentCount, resultCount, budget, outError);
}

void fail_callback(Vm& vm, std::string_view callbackName, std::string_view error) {
    const std::string message = std::string{callbackName} + ": " + std::string{error};
    if (svc_host != nullptr) {
        svc_host->fail(vm.subject, MOD_ERROR, message.c_str());
    } else if (svc_log != nullptr) {
        svc_log->write(vm.subject, LOG_LEVEL_ERROR, message.c_str());
    }
}

ModResult set_error(ModError* outError, ModResult result, std::string_view message) {
    if (outError != nullptr && outError->struct_size >= sizeof(ModError)) {
        outError->code = result;
        const size_t size = std::min(message.size(), sizeof(outError->message) - 1);
        std::memcpy(outError->message, message.data(), size);
        outError->message[size] = '\0';
    }
    return result;
}

void create_handle_metatable(
    lua_State* state, const char* name, const luaL_Reg* methods, const char* typeName) {
    luaL_newmetatable(state, name);
    luaL_register(state, nullptr, methods);
    lua_pushvalue(state, -1);
    lua_setfield(state, -2, "__index");
    lua_pushstring(state, typeName);
    lua_setfield(state, -2, "__type");
    lua_setreadonly(state, -1, true);
    lua_pop(state, 1);
}

ScriptHandle& check_handle(lua_State* state, int index, const char* metatable, HandleKind kind) {
    auto* handle = static_cast<ScriptHandle*>(luaL_checkudata(state, index, metatable));
    if (handle == nullptr || handle->kind != kind || handle->vm == nullptr || handle->value == 0) {
        luaL_argerror(state, index, "stale handle");
    }
    return *handle;
}

void push_handle(lua_State* state, Vm& vm, uint64_t value, HandleKind kind, const char* metatable,
    ConfigVarType configType) {
    auto* handle = static_cast<ScriptHandle*>(lua_newuserdata(state, sizeof(ScriptHandle)));
    *handle = ScriptHandle{.vm = &vm, .value = value, .kind = kind, .configType = configType};
    luaL_getmetatable(state, metatable);
    lua_setmetatable(state, -2);
}

}  // namespace luau_runtime

extern "C" {
MOD_EXPORT ModResult mod_initialize(ModError*) {
    FFlag::LuauIntegerType2.value = true;
    FFlag::LuauIntegerLibrary.value = true;
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    luau_runtime::s_vms.clear();
    return MOD_OK;
}
}
