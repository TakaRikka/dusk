#include <cstdio>
#include <filesystem>

#include "dusk/io.hpp"

using namespace dusk::io;

#if _WIN32
#define MODE_TYPE wchar_t
#define MODE(val) L##val
#define ftell(file) _ftelli64(file)
#define fseek(a, b, c) _fseeki64(a, b, c)
#else
#define MODE_TYPE char
#if _MSVC_VER
#define MODE(val) ##val
#else
#define MODE(val) val
#endif
#define _SH_DENYNO 0
#define _SH_DENYWR 0
#endif

static FILE* ThrowIfNotOpen(const FileStream& file) {
    if (!file.GetFileHandle()) {
        throw std::runtime_error("Invalid file handle!");
    }

    return static_cast<FILE*>(file.GetFileHandle());
}

[[noreturn]] static void ThrowForError(int code) {
    throw std::system_error(std::make_error_code(static_cast<std::errc>(code)));
}

static FILE* OpenCore(const std::filesystem::path& path, const MODE_TYPE* mode, int shareFlag) {
    FILE* file;

    int err;
    errno = 0;
#if _WIN32
    static_assert(std::is_same_v<std::filesystem::path::value_type, wchar_t>);
    file = _wfsopen(path.c_str(), mode, shareFlag);
#else
    static_assert(std::is_same_v<std::filesystem::path::value_type, char>);
    file = fopen(path.c_str(), mode);
#endif
    err = errno;

    if (!file) {
        ThrowForError(err);
    }

    return file;
}

static FILE* OpenCore(const char* path, const MODE_TYPE* mode, int shareFlag) {
    return OpenCore(reinterpret_cast<const char8_t*>(path), mode, shareFlag);
}

FileStream::FileStream() noexcept : file(nullptr) {
}

FileStream::FileStream(FILE* file) : file(file) {
    if (!file) {
        CRASH("Invalid file handle");
    }
}

FileStream::FileStream(FileStream&& other) noexcept {
    file = other.file;
    other.file = nullptr;
}

FileStream::~FileStream() {
    if (file)
        fclose(static_cast<FILE*>(file));
}

FileStream FileStream::OpenRead(const char* utf8Path) {
    return FileStream(OpenCore(utf8Path, MODE("rb"), _SH_DENYWR));
}

FileStream FileStream::OpenRead(const std::filesystem::path& path) {
    return FileStream(OpenCore(path, MODE("rb"), _SH_DENYWR));
}

FileStream FileStream::Create(const char* utf8Path) {
    return FileStream(OpenCore(utf8Path, MODE("wb"), _SH_DENYWR));
}

FileStream FileStream::Create(const std::filesystem::path& path) {
    return FileStream(OpenCore(path, MODE("wb"), _SH_DENYWR));
}

std::vector<u8> FileStream::ReadFull() {
    const auto fileHandle = ThrowIfNotOpen(*this);

    std::vector<u8> result;

    const auto startPos = ftell(fileHandle);
    if (startPos == -1) {
        ThrowForError(errno);
    }

    auto seekRes = fseek(fileHandle, 0, SEEK_END);
    if (seekRes != 0) {
        ThrowForError(errno);
    }

    const auto endPos = ftell(fileHandle);
    if (endPos == -1) {
        ThrowForError(errno);
    }

    seekRes = fseek(fileHandle, startPos, SEEK_SET);
    if (seekRes != 0) {
        ThrowForError(errno);
    }

    result.resize(endPos - startPos);

    if (result.empty()) {
        return result;
    }

    auto ret = fread(result.data(), 1, result.size(), fileHandle);
    if (ret < result.size()) {
        int err = errno;
        // Error or EOF
        if (feof(fileHandle)) {
            result.resize(ret);
        } else {
            ThrowForError(err);
        }
    }

    return result;
}

std::vector<u8> FileStream::ReadAllBytes(const char* utf8Path) {
    return ReadAllBytes(reinterpret_cast<const char8_t*>(utf8Path));
}

std::vector<u8> FileStream::ReadAllBytes(const std::filesystem::path& path) {
    auto handle = OpenRead(path);
    return std::move(handle.ReadFull());
}

void FileStream::Write(const char* data, size_t dataLen) {
    FILE* fileHandle = ThrowIfNotOpen(*this);

    const auto ret = fwrite(data, 1, dataLen, fileHandle);
    if (ret < dataLen) {
        ThrowForError(errno);
    }
}

void FileStream::WriteAllText(const char* utf8Path, const std::string_view text) {
    WriteAllText(reinterpret_cast<const char8_t*>(utf8Path), text);
}

void FileStream::WriteAllText(const std::filesystem::path& path, const std::string_view text) {
    auto handle = Create(path);
    handle.Write(text.data(), text.size());
}

FILE* FileStream::ToInner() {
    auto handle = file;
    file = nullptr;
    return handle;
}