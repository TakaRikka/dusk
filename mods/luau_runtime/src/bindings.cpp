#include "runtime.hpp"

#include <cstring>
#include <string>
#include <vector>

namespace luau_runtime {
namespace {

constexpr char kOverlayMetatable[] = "dusklight.overlay_handle";
constexpr char kTextureMetatable[] = "dusklight.texture_handle";

uint64_t get_hash_field(lua_State* state, int table, const char* field, bool required) {
    lua_getfield(state, table, field);
    if (lua_isnil(state, -1)) {
        lua_pop(state, 1);
        if (required) {
            luaL_error(state, "field '%s' is required", field);
        }
        return 0;
    }
    if (!lua_isinteger64(state, -1)) {
        luaL_error(state, "field '%s' must be an integer", field);
    }
    const uint64_t value = static_cast<uint64_t>(lua_tointeger64(state, -1, nullptr));
    lua_pop(state, 1);
    return value;
}

int log_write(lua_State* state, LogLevel level, int messageIndex) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_log == nullptr) {
        service_unavailable(state, "LogService");
    }
    size_t length = 0;
    const char* message = luaL_checklstring(state, messageIndex, &length);
    const std::string copy{message, length};
    svc_log->write(vm.subject, level, copy.c_str());
    return 0;
}

int log_trace(lua_State* state) {
    return log_write(state, LOG_LEVEL_TRACE, 1);
}

int log_debug(lua_State* state) {
    return log_write(state, LOG_LEVEL_DEBUG, 1);
}

int log_info(lua_State* state) {
    return log_write(state, LOG_LEVEL_INFO, 1);
}

int log_warn(lua_State* state) {
    return log_write(state, LOG_LEVEL_WARN, 1);
}

int log_error(lua_State* state) {
    return log_write(state, LOG_LEVEL_ERROR, 1);
}

int log_write_level(lua_State* state) {
    static constexpr const char* kLevels[] = {"trace", "debug", "info", "warn", "error", nullptr};
    const int level = luaL_checkoption(state, 1, nullptr, kLevels);
    return log_write(state, static_cast<LogLevel>(level), 2);
}

int host_mod_id(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_host == nullptr) {
        service_unavailable(state, "HostService");
    }
    lua_pushstring(state, svc_host->mod_id(vm.subject));
    return 1;
}

int host_mod_name(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_host == nullptr) {
        service_unavailable(state, "HostService");
    }
    lua_pushstring(state, svc_host->mod_name(vm.subject));
    return 1;
}

int host_mod_version(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_host == nullptr) {
        service_unavailable(state, "HostService");
    }
    lua_pushstring(state, svc_host->mod_version(vm.subject));
    return 1;
}

int host_mod_dir(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_host == nullptr) {
        service_unavailable(state, "HostService");
    }
    lua_pushstring(state, svc_host->mod_dir(vm.subject));
    return 1;
}

int host_data_dir(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_host == nullptr || !SERVICE_HAS(svc_host, HostService, data_dir) ||
        svc_host->data_dir == nullptr)
    {
        service_unavailable(state, "HostService::data_dir");
    }
    const char* path = nullptr;
    check_result(state, svc_host->data_dir(vm.subject, &path), "host.data_dir");
    lua_pushstring(state, path != nullptr ? path : "");
    return 1;
}

int host_fail(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_host == nullptr) {
        service_unavailable(state, "HostService");
    }
    const char* message = luaL_checkstring(state, 1);
    svc_host->fail(vm.subject, MOD_ERROR, message);
    luaL_error(state, "%s", message);
}

int register_host_callback(lua_State* state, std::vector<int>& callbacks) {
    luaL_checktype(state, 1, LUA_TFUNCTION);
    lua_pushvalue(state, 1);
    callbacks.push_back(lua_ref(state, -1));
    lua_pop(state, 1);
    return 0;
}

int host_on_update(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    return register_host_callback(state, vm.updateRefs);
}

int host_on_shutdown(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    return register_host_callback(state, vm.shutdownRefs);
}

int resource_load(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_resource == nullptr) {
        service_unavailable(state, "ResourceService");
    }
    const char* path = luaL_checkstring(state, 1);
    ResourceBuffer buffer = RESOURCE_BUFFER_INIT;
    check_result(state, svc_resource->load(vm.subject, path, &buffer), "resource.load");
    const auto* data = buffer.data != nullptr ? static_cast<const char*>(buffer.data) : "";
    const std::string copy{data, buffer.size};
    svc_resource->free(vm.subject, &buffer);
    lua_pushlstring(state, copy.data(), copy.size());
    return 1;
}

int overlay_remove(lua_State* state) {
    auto& handle = check_handle(state, 1, kOverlayMetatable, HandleKind::Overlay);
    if (svc_overlay == nullptr) {
        service_unavailable(state, "OverlayService");
    }
    check_result(state, svc_overlay->remove(handle.vm->subject, handle.value), "overlay.remove");
    handle.value = 0;
    return 0;
}

int overlay_add_file(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_overlay == nullptr) {
        service_unavailable(state, "OverlayService");
    }
    const char* discPath = luaL_checkstring(state, 1);
    const char* bundlePath = luaL_checkstring(state, 2);
    OverlayHandle handle = 0;
    check_result(state, svc_overlay->add_file(vm.subject, discPath, bundlePath, &handle),
        "overlay.add_file");
    push_handle(state, vm, handle, HandleKind::Overlay, kOverlayMetatable);
    return 1;
}

int overlay_add_buffer(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_overlay == nullptr) {
        service_unavailable(state, "OverlayService");
    }
    const char* discPath = luaL_checkstring(state, 1);
    size_t size = 0;
    const char* data = luaL_checklstring(state, 2, &size);
    OverlayHandle handle = 0;
    check_result(state, svc_overlay->add_buffer(vm.subject, discPath, data, size, &handle),
        "overlay.add_buffer");
    push_handle(state, vm, handle, HandleKind::Overlay, kOverlayMetatable);
    return 1;
}

int texture_unregister(lua_State* state) {
    auto& handle = check_handle(state, 1, kTextureMetatable, HandleKind::Texture);
    if (svc_texture == nullptr) {
        service_unavailable(state, "TextureService");
    }
    check_result(
        state, svc_texture->unregister(handle.vm->subject, handle.value), "texture.unregister");
    handle.value = 0;
    return 0;
}

int texture_register_file(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_texture == nullptr) {
        service_unavailable(state, "TextureService");
    }
    const char* path = luaL_checkstring(state, 1);
    TextureReplacementHandle handle = 0;
    check_result(
        state, svc_texture->register_file(vm.subject, path, &handle), "texture.register_file");
    push_handle(state, vm, handle, HandleKind::Texture, kTextureMetatable);
    return 1;
}

int texture_register_data(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_texture == nullptr) {
        service_unavailable(state, "TextureService");
    }
    luaL_checktype(state, 1, LUA_TTABLE);
    luaL_checktype(state, 2, LUA_TTABLE);

    TextureKey key = TEXTURE_KEY_INIT;
    key.kind = TEXTURE_KEY_SOURCE;
    key.texture_hash = get_hash_field(state, 1, "texture_hash", true);
    key.tlut_hash = get_hash_field(state, 1, "tlut_hash", false);
    key.width = static_cast<uint32_t>(get_optional_int(state, 1, "width", 0));
    key.height = static_cast<uint32_t>(get_optional_int(state, 1, "height", 0));
    key.gx_format = static_cast<uint32_t>(get_optional_int(state, 1, "gx_format", 0));
    key.has_tlut = get_optional_bool(state, 1, "has_tlut", false);

    lua_getfield(state, 2, "data");
    size_t size = 0;
    const char* bytes = luaL_checklstring(state, -1, &size);
    TextureData data = TEXTURE_DATA_INIT;
    data.data = bytes;
    data.size = size;
    data.width = static_cast<uint32_t>(get_optional_int(state, 2, "width", key.width));
    data.height = static_cast<uint32_t>(get_optional_int(state, 2, "height", key.height));
    data.mip_count = static_cast<uint32_t>(get_optional_int(state, 2, "mip_count", 1));
    data.gx_format = static_cast<uint32_t>(get_optional_int(state, 2, "gx_format", key.gx_format));

    TextureReplacementHandle handle = 0;
    const ModResult result = svc_texture->register_data(vm.subject, &key, &data, &handle);
    lua_pop(state, 1);
    check_result(state, result, "texture.register_data");
    push_handle(state, vm, handle, HandleKind::Texture, kTextureMetatable);
    return 1;
}

}  // namespace

int open_log(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_log == nullptr) {
        service_unavailable(state, "LogService");
    }
    lua_newtable(state);
    set_function(state, vm, "write", log_write_level);
    set_function(state, vm, "trace", log_trace);
    set_function(state, vm, "debug", log_debug);
    set_function(state, vm, "info", log_info);
    set_function(state, vm, "warn", log_warn);
    set_function(state, vm, "error", log_error);
    lua_setreadonly(state, -1, true);
    return 1;
}

int open_host(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_host == nullptr) {
        service_unavailable(state, "HostService");
    }
    lua_newtable(state);
    lua_pushstring(state, svc_host->version != nullptr ? svc_host->version : "");
    lua_setfield(state, -2, "version");
    set_function(state, vm, "mod_id", host_mod_id);
    set_function(state, vm, "mod_name", host_mod_name);
    set_function(state, vm, "mod_version", host_mod_version);
    set_function(state, vm, "mod_dir", host_mod_dir);
    set_function(state, vm, "data_dir", host_data_dir);
    set_function(state, vm, "on_update", host_on_update);
    set_function(state, vm, "on_shutdown", host_on_shutdown);
    set_function(state, vm, "fail", host_fail);
    lua_setreadonly(state, -1, true);
    return 1;
}

int open_resource(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_resource == nullptr) {
        service_unavailable(state, "ResourceService");
    }
    lua_newtable(state);
    set_function(state, vm, "load", resource_load);
    lua_setreadonly(state, -1, true);
    return 1;
}

int open_overlay(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_overlay == nullptr) {
        service_unavailable(state, "OverlayService");
    }
    static const luaL_Reg kMethods[] = {{"remove", overlay_remove}, {nullptr, nullptr}};
    create_handle_metatable(state, kOverlayMetatable, kMethods, "OverlayHandle");
    lua_newtable(state);
    set_function(state, vm, "add_file", overlay_add_file);
    set_function(state, vm, "add_buffer", overlay_add_buffer);
    lua_setreadonly(state, -1, true);
    return 1;
}

int open_texture(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_texture == nullptr) {
        service_unavailable(state, "TextureService");
    }
    static const luaL_Reg kMethods[] = {{"unregister", texture_unregister}, {nullptr, nullptr}};
    create_handle_metatable(state, kTextureMetatable, kMethods, "TextureHandle");
    lua_newtable(state);
    set_function(state, vm, "register_file", texture_register_file);
    set_function(state, vm, "register_data", texture_register_data);
    lua_pushinteger64(state, static_cast<int64_t>(TEXTURE_HASH_WILDCARD));
    lua_setfield(state, -2, "hash_wildcard");
    lua_pushinteger64(state, static_cast<int64_t>(TEXTURE_TLUT_WILDCARD));
    lua_setfield(state, -2, "tlut_wildcard");
    lua_setreadonly(state, -1, true);
    return 1;
}

}  // namespace luau_runtime
