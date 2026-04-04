#include "dusk/io.hpp"
#include <cstdio>
#include <filesystem>

using namespace dusk::io;

#if _WIN32
#define MODE_TYPE wchar_t
#define MODE(val) L##val
#define ftell(file) _ftelli64(file)
#define fseek(a, b, c) _fseeki64(a, b, c)
#else
#define MODE_TYPE char
#define MODE(val) ##val
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

static FILE* OpenCore(const std::filesystem::path& path, const MODE_TYPE* mode) {
    FILE* file;

    int err;
#if _WIN32
    static_assert(std::is_same_v<std::filesystem::path::value_type, wchar_t>);
    err = _wfopen_s(&file, path.c_str(), mode);
#else
    errno = 0;
    static_assert(std::is_same_v<std::filesystem::path::value_type, char>);
    file = fopen(path.c_str(), mode);
    err = errno;
#endif

    if (!file) {
        ThrowForError(err);
    }

    return file;
}

static FILE* OpenCore(const char* path, const MODE_TYPE* mode) {
    return OpenCore(reinterpret_cast<const char8_t*>(path), mode);
}

FileStream::FileStream() noexcept : file(nullptr) {
}

FileStream::FileStream(void* file) : file(file) {
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
    return FileStream(OpenCore(utf8Path, MODE("rb")));
}

FileStream FileStream::Create(const char* utf8Path) {
    return FileStream(OpenCore(utf8Path, MODE("wb")));
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
    auto handle = OpenRead(utf8Path);
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
    auto handle = Create(utf8Path);
    handle.Write(text.data(), text.size());
}
