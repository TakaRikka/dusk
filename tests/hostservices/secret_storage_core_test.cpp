#include "dusk/mods/svc/secret_storage_core.hpp"

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>
#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using dusk::mods::svc::SecretStorageBackend;
using dusk::mods::svc::SecretStorageCore;

struct TestMod { std::string id; };
struct ModContext { TestMod* mod; };

extern "C" SecretStorageResult secret_storage_c_testmod_roundtrip(
    const SecretStorageService*, ModContext*, const char*);

namespace {

class FakeBackend final : public SecretStorageBackend {
public:
    SecretStorageResult nextGet = SECRET_STORAGE_OK;
    std::vector<uint8_t> errorPayload;
    std::unordered_map<std::string, std::vector<uint8_t>> values;

    SecretStorageResult get(
        std::string_view modId, std::string_view key, std::vector<uint8_t>& value) override {
        if (nextGet != SECRET_STORAGE_OK) {
            value = errorPayload;
            return nextGet;
        }
        auto found = values.find(std::string(modId) + "/" + std::string(key));
        if (found == values.end()) return SECRET_STORAGE_NOT_FOUND;
        value = found->second;
        return SECRET_STORAGE_OK;
    }
    SecretStorageResult put(
        std::string_view modId, std::string_view key, const uint8_t* data, size_t size) override {
        values[std::string(modId) + "/" + std::string(key)] = {data, data + size};
        return SECRET_STORAGE_OK;
    }
    SecretStorageResult remove(std::string_view modId, std::string_view key) override {
        return values.erase(std::string(modId) + "/" + std::string(key)) == 0 ?
            SECRET_STORAGE_NOT_FOUND : SECRET_STORAGE_OK;
    }
};

SecretStorageCore* core = nullptr;
SecretStorageResult tableGet(ModContext* context, const char* key, ResourceBuffer* out) {
    return core->get(context, key, out);
}
void tableFree(ModContext* context, ResourceBuffer* buffer) { core->free(context, buffer); }
SecretStorageResult tablePut(ModContext* context, const char* key, const void* data, size_t size) {
    return core->put(context, key, data, size);
}
SecretStorageResult tableRemove(ModContext* context, const char* key) {
    return core->remove(context, key);
}
constexpr SecretStorageService service{
    .header = SERVICE_HEADER(SecretStorageService, SECRET_STORAGE_SERVICE_MAJOR, SECRET_STORAGE_SERVICE_MINOR),
    .get = tableGet,
    .free = tableFree,
    .put = tablePut,
    .remove = tableRemove,
};

struct Fixture {
    TestMod first{"com.example_first"};
    TestMod second{"com.example_second"};
    ModContext firstContext{&first};
    ModContext secondContext{&second};
    FakeBackend* backend;
    SecretStorageCore storage;

    Fixture() : backend(new FakeBackend()),
        storage([](ModContext* context) -> std::string_view {
            return context == nullptr || context->mod == nullptr ? std::string_view{} :
                std::string_view(context->mod->id);
        }, std::unique_ptr<SecretStorageBackend>(backend)) {
        core = &storage;
    }
    ~Fixture() { core = nullptr; }
};

void test_abi_table_and_validation() {
    static_assert(SECRET_STORAGE_OK == 0 && SECRET_STORAGE_NOT_FOUND == 1 &&
        SECRET_STORAGE_INVALID_ARGUMENT == 2 && SECRET_STORAGE_TOO_LARGE == 3 &&
        SECRET_STORAGE_UNAVAILABLE == 4 && SECRET_STORAGE_CORRUPT == 5 &&
        SECRET_STORAGE_KEY_INVALIDATED == 6 && SECRET_STORAGE_IO_ERROR == 7);
    static_assert(sizeof(SecretStorageResult) == sizeof(int));
    static_assert(offsetof(SecretStorageService, get) < offsetof(SecretStorageService, free));
    static_assert(offsetof(SecretStorageService, free) < offsetof(SecretStorageService, put));
    static_assert(offsetof(SecretStorageService, put) < offsetof(SecretStorageService, remove));
    Fixture fixture;
    assert(secret_storage_c_testmod_roundtrip(&service, &fixture.firstContext, "c.roundtrip") ==
        SECRET_STORAGE_OK);
    const char value[] = "x";
    const char* invalid[] = {"", ".x", "Upper", "x/y", "x ", "x\xc3\xa9"};
    for (const char* key : invalid) {
        assert(fixture.storage.put(&fixture.firstContext, key, value, 1) ==
            SECRET_STORAGE_INVALID_ARGUMENT);
    }
    fixture.first.id = "Com.example";
    assert(fixture.storage.put(&fixture.firstContext, "ok", value, 1) ==
        SECRET_STORAGE_INVALID_ARGUMENT);
    fixture.first.id = std::string(64, 'a') + "." + std::string(64, 'b') + "." +
        std::string(64, 'c') + "." + std::string(45, 'd');
    assert(fixture.first.id.size() == SECRET_STORAGE_MOD_ID_MAX_SIZE);
    assert(fixture.storage.put(&fixture.firstContext, "ok", value, 1) == SECRET_STORAGE_OK);
    fixture.first.id += "e";
    assert(fixture.storage.put(&fixture.firstContext, "ok", value, 1) ==
        SECRET_STORAGE_INVALID_ARGUMENT);
    fixture.first.id = std::string(65, 'a');
    assert(fixture.storage.put(&fixture.firstContext, "ok", value, 1) ==
        SECRET_STORAGE_INVALID_ARGUMENT);
}

void test_bounds_empty_and_copy() {
    Fixture fixture;
    assert(fixture.storage.put(&fixture.firstContext, "empty", nullptr, 0) == SECRET_STORAGE_OK);
    ResourceBuffer empty = RESOURCE_BUFFER_INIT;
    assert(fixture.storage.get(&fixture.firstContext, "empty", &empty) == SECRET_STORAGE_OK);
    assert(empty.data == nullptr && empty.size == 0);
    std::vector<uint8_t> max(SECRET_STORAGE_VALUE_MAX_SIZE, 7);
    assert(fixture.storage.put(&fixture.firstContext, "max", max.data(), max.size()) == SECRET_STORAGE_OK);
    max[0] = 9;
    assert(fixture.backend->values["com.example_first/max"][0] == 7);
    max.push_back(1);
    assert(fixture.storage.put(&fixture.firstContext, "large", max.data(), max.size()) ==
        SECRET_STORAGE_TOO_LARGE);
    ResourceBuffer result = RESOURCE_BUFFER_INIT;
    assert(fixture.storage.get(&fixture.firstContext, "max", &result) == SECRET_STORAGE_OK);
    assert(result.size == SECRET_STORAGE_VALUE_MAX_SIZE);
    fixture.storage.free(&fixture.firstContext, &result);
}

void test_fail_closed_owner_and_detach() {
    Fixture fixture;
    const char value[] = "owned";
    assert(fixture.storage.put(&fixture.firstContext, "owner", value, 5) == SECRET_STORAGE_OK);
    ResourceBuffer buffer = RESOURCE_BUFFER_INIT;
    assert(fixture.storage.get(&fixture.firstContext, "owner", &buffer) == SECRET_STORAGE_OK);
    void* allocation = buffer.data;
    fixture.storage.free(&fixture.secondContext, &buffer);
    assert(buffer.data == allocation && buffer.size == 5);
    buffer.size = 0;
    fixture.storage.free(&fixture.firstContext, &buffer);
    assert(buffer.data == nullptr && buffer.size == 0);

    assert(fixture.storage.get(&fixture.firstContext, "owner", &buffer) == SECRET_STORAGE_OK);
    buffer.size = SECRET_STORAGE_VALUE_MAX_SIZE + 1;
    fixture.storage.free(&fixture.firstContext, &buffer);
    assert(buffer.data == nullptr && buffer.size == 0);

    assert(fixture.storage.get(&fixture.firstContext, "owner", &buffer) == SECRET_STORAGE_OK);
    allocation = buffer.data;
    fixture.storage.modDetached(&fixture.firstContext);
    assert(buffer.data == allocation);  // Detach reclaimed and wiped the owned allocation.
    fixture.storage.free(&fixture.firstContext, &buffer);
    assert(buffer.data == allocation);  // It is no longer an owned buffer.

    ResourceBuffer failed = {.struct_size = sizeof(ResourceBuffer), .data = allocation, .size = 5};
    fixture.backend->nextGet = SECRET_STORAGE_IO_ERROR;
    fixture.backend->errorPayload = {1, 2, 3};
    assert(fixture.storage.get(&fixture.firstContext, "owner", &failed) == SECRET_STORAGE_IO_ERROR);
    assert(failed.data == nullptr && failed.size == 0);
}

class UnavailableBackend final : public SecretStorageBackend {
public:
    SecretStorageResult get(std::string_view, std::string_view, std::vector<uint8_t>&) override {
        return SECRET_STORAGE_UNAVAILABLE;
    }
    SecretStorageResult put(std::string_view, std::string_view, const uint8_t*, size_t) override {
        return SECRET_STORAGE_UNAVAILABLE;
    }
    SecretStorageResult remove(std::string_view, std::string_view) override {
        return SECRET_STORAGE_UNAVAILABLE;
    }
};

void test_unavailable_backend() {
    TestMod mod{"com.example"};
    ModContext context{&mod};
    SecretStorageCore storage([](ModContext* item) { return std::string_view(item->mod->id); },
        std::make_unique<UnavailableBackend>());
    ResourceBuffer out = RESOURCE_BUFFER_INIT;
    assert(storage.get(&context, "key", &out) == SECRET_STORAGE_UNAVAILABLE);
    assert(storage.put(&context, "key", nullptr, 0) == SECRET_STORAGE_UNAVAILABLE);
    assert(storage.remove(&context, "key") == SECRET_STORAGE_UNAVAILABLE);
}

}  // namespace

int main() {
    test_abi_table_and_validation();
    test_bounds_empty_and_copy();
    test_fail_closed_owner_and_detach();
    test_unavailable_backend();
    return 0;
}
