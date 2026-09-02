#pragma once

#include <mods/svc/file.h>

#include <array>
#include <cstdint>
#include <functional>
#include <span>
#include <string>
#include <utility>
#include <vector>

namespace mods::file {

class File {
public:
    File() = default;
    File(FileStreamHandle handle, ModResult result) : mHandle{handle}, mResult{result} {}
    ~File() { reset(); }
    File(const File&) = delete;
    File& operator=(const File&) = delete;
    File(File&& other) noexcept { *this = std::move(other); }
    File& operator=(File&& other) noexcept {
        if (this != &other) {
            reset();
            mHandle = std::exchange(other.mHandle, 0);
            mResult = other.mResult;
        }
        return *this;
    }

    explicit operator bool() const { return mResult == MOD_OK && mHandle != 0; }
    ModResult result() const { return mResult; }
    FileStreamHandle handle() const { return mHandle; }

    uint64_t size() const {
        uint64_t value = 0;
        if (mHandle != 0 && svc_file != nullptr) {
            svc_file->size(mod_ctx, mHandle, &value);
        }
        return value;
    }

    uint64_t read(void* buffer, uint64_t length) {
        uint64_t value = 0;
        if (mHandle == 0 || svc_file == nullptr) {
            mResult = MOD_UNAVAILABLE;
        } else {
            mResult = svc_file->read(mod_ctx, mHandle, buffer, length, &value);
        }
        return value;
    }

    bool seek(uint64_t offset) {
        mResult = mHandle != 0 && svc_file != nullptr ? svc_file->seek(mod_ctx, mHandle, offset) :
                                                        MOD_UNAVAILABLE;
        return mResult == MOD_OK;
    }

    bool write(const void* buffer, uint64_t length) {
        if (mHandle == 0 || svc_file == nullptr) {
            mResult = MOD_UNAVAILABLE;
        } else {
            mResult = svc_file->write(mod_ctx, mHandle, buffer, length);
        }
        return mResult == MOD_OK;
    }

    bool write(std::span<const uint8_t> bytes) { return write(bytes.data(), bytes.size()); }

    bool flush() {
        if (mHandle == 0 || svc_file == nullptr) {
            mResult = MOD_UNAVAILABLE;
        } else {
            mResult = svc_file->flush(mod_ctx, mHandle);
        }
        return mResult == MOD_OK;
    }

    bool close() {
        if (mHandle == 0) {
            return mResult == MOD_OK;
        }
        mResult = svc_file != nullptr ? svc_file->close(mod_ctx, mHandle) : MOD_UNAVAILABLE;
        mHandle = 0;
        return mResult == MOD_OK;
    }

    void reset() { (void)close(); }

private:
    FileStreamHandle mHandle = 0;
    ModResult mResult = MOD_UNAVAILABLE;
};

inline File open(const std::string& location, FileOpenMode mode = FILE_OPEN_READ) {
    if (svc_file == nullptr) {
        return {0, MOD_UNAVAILABLE};
    }
    FileStreamHandle handle = 0;
    const auto result = svc_file->open(mod_ctx, location.c_str(), mode, &handle);
    return {handle, result};
}

class Buffer {
public:
    Buffer() = default;
    Buffer(FileBuffer buffer, ModResult result) : mBuffer{buffer}, mResult{result} {}
    ~Buffer() { reset(); }
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&& other) noexcept { *this = std::move(other); }
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            reset();
            mBuffer = other.mBuffer;
            mResult = other.mResult;
            other.mBuffer = FILE_BUFFER_INIT;
        }
        return *this;
    }

    explicit operator bool() const { return mResult == MOD_OK; }
    ModResult result() const { return mResult; }
    std::span<const uint8_t> bytes() const {
        return {static_cast<const uint8_t*>(mBuffer.data), mBuffer.size};
    }
    void reset() {
        if (mBuffer.data != nullptr && svc_file != nullptr) {
            svc_file->free(mod_ctx, &mBuffer);
        }
        mBuffer.data = nullptr;
        mBuffer.size = 0;
    }

private:
    FileBuffer mBuffer = FILE_BUFFER_INIT;
    ModResult mResult = MOD_UNAVAILABLE;
};

inline Buffer read_all(const std::string& location) {
    FileBuffer buffer = FILE_BUFFER_INIT;
    const auto result = svc_file != nullptr ?
                            svc_file->read_all(mod_ctx, location.c_str(), &buffer) :
                            MOD_UNAVAILABLE;
    return {buffer, result};
}

inline ModResult check(const std::string& location) {
    return svc_file != nullptr ? svc_file->check(mod_ctx, location.c_str()) : MOD_UNAVAILABLE;
}

inline std::string display_name(const std::string& location) {
    if (svc_file == nullptr) {
        return {};
    }
    std::array<char, 1024> buffer{};
    return svc_file->display_name(mod_ctx, location.c_str(), buffer.data(),
               static_cast<uint32_t>(buffer.size())) == MOD_OK ?
               std::string{buffer.data()} :
               std::string{};
}

inline ModResult join(
    const std::string& folder, const std::string& relativePath, std::string& outLocation) {
    outLocation.clear();
    if (svc_file == nullptr) {
        return MOD_UNAVAILABLE;
    }
    const char* location = nullptr;
    const auto result = svc_file->join(mod_ctx, folder.c_str(), relativePath.c_str(), &location);
    if (result == MOD_OK && location != nullptr) {
        outLocation = location;
    }
    return result;
}

inline ModResult create_child(
    const std::string& folder, const std::string& name, std::string& outLocation) {
    outLocation.clear();
    if (svc_file == nullptr) {
        return MOD_UNAVAILABLE;
    }
    const char* location = nullptr;
    const auto result = svc_file->create_child(mod_ctx, folder.c_str(), name.c_str(), &location);
    if (result == MOD_OK && location != nullptr) {
        outLocation = location;
    }
    return result;
}

inline ModResult write_all(const std::string& location, std::span<const uint8_t> bytes) {
    if (svc_file == nullptr) {
        return MOD_UNAVAILABLE;
    }
    return svc_file->write_all(mod_ctx, location.c_str(), bytes.data(), bytes.size());
}

struct Entry {
    std::string name;
    std::string location;
    bool isDirectory = false;
};

inline ModResult list(const std::string& folder, std::vector<Entry>& outEntries) {
    outEntries.clear();
    if (svc_file == nullptr) {
        return MOD_UNAVAILABLE;
    }
    return svc_file->list(
        mod_ctx, folder.c_str(),
        [](ModContext*, const FileEntry* entry, void* userData) {
            if (entry == nullptr) {
                return;
            }
            static_cast<std::vector<Entry>*>(userData)->push_back({
                .name = entry->name != nullptr ? entry->name : "",
                .location = entry->location != nullptr ? entry->location : "",
                .isDirectory = entry->is_directory,
            });
        },
        &outEntries);
}

struct Filter {
    std::string name;
    std::string pattern;
};

struct PickOptions {
    std::vector<Filter> filters;
    std::string defaultLocation;
};

struct PickResult {
    ModResult status = MOD_ERROR;
    std::vector<std::string> locations;
    std::string error;
};

namespace detail {

inline std::function<void(PickResult)> pickCallback;

inline void pick_trampoline(ModContext*, ModResult status, const char* const* locations,
    uint32_t locationCount, const char* error, void*) {
    PickResult result{.status = status, .error = error != nullptr ? error : ""};
    result.locations.reserve(locationCount);
    for (uint32_t i = 0; i < locationCount; ++i) {
        if (locations[i] != nullptr) {
            result.locations.emplace_back(locations[i]);
        }
    }
    auto callback = std::move(pickCallback);
    pickCallback = {};
    if (callback) {
        callback(std::move(result));
    }
}

inline ModResult pick(
    const PickOptions& options, std::function<void(PickResult)> callback, bool folder) {
    if (svc_file == nullptr) {
        return MOD_UNAVAILABLE;
    }
    if (!callback) {
        return MOD_INVALID_ARGUMENT;
    }
    if (pickCallback) {
        return MOD_CONFLICT;
    }
    std::vector<FileFilter> filters;
    filters.reserve(options.filters.size());
    for (const auto& filter : options.filters) {
        filters.push_back({filter.name.c_str(), filter.pattern.c_str()});
    }
    FilePickOptions raw = FILE_PICK_OPTIONS_INIT;
    raw.filters = filters.empty() ? nullptr : filters.data();
    raw.filter_count = static_cast<uint32_t>(filters.size());
    raw.default_location =
        options.defaultLocation.empty() ? nullptr : options.defaultLocation.c_str();
    pickCallback = std::move(callback);
    const auto result = folder ? svc_file->pick_folder(mod_ctx, &raw, pick_trampoline, nullptr) :
                                 svc_file->pick_file(mod_ctx, &raw, pick_trampoline, nullptr);
    if (result != MOD_OK) {
        pickCallback = {};
    }
    return result;
}

}  // namespace detail

inline ModResult pick_file(const PickOptions& options, std::function<void(PickResult)> callback) {
    return detail::pick(options, std::move(callback), false);
}

inline ModResult pick_folder(const PickOptions& options, std::function<void(PickResult)> callback) {
    return detail::pick(options, std::move(callback), true);
}

inline ModResult export_file(const std::string& sourceLocation, const std::string& suggestedName,
    std::function<void(PickResult)> callback) {
    if (svc_file == nullptr) {
        return MOD_UNAVAILABLE;
    }
    if (!callback) {
        return MOD_INVALID_ARGUMENT;
    }
    if (detail::pickCallback) {
        return MOD_CONFLICT;
    }
    detail::pickCallback = std::move(callback);
    const auto result = svc_file->export_file(
        mod_ctx, sourceLocation.c_str(), suggestedName.c_str(), detail::pick_trampoline, nullptr);
    if (result != MOD_OK) {
        detail::pickCallback = {};
    }
    return result;
}

}  // namespace mods::file
