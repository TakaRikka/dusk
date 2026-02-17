#include "dusk/dvd_emu.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include "dolphin/os.h"

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define PATH_SEP '\\'
#else
#include <sys/stat.h>
#include <unistd.h>
#define PATH_SEP '/'
#endif

namespace {
s32 g_nextEntryNum = 1;

// Lazy-init to avoid crash during DLL static initialization
std::string& g_basePath() {
    static std::string instance = "data";
    return instance;
}

std::unordered_map<s32, std::string>& getEntryPaths() {
    static std::unordered_map<s32, std::string> instance;
    return instance;
}
}  // namespace

namespace DvdEmu {

void setBasePath(const char* path) {
#ifdef _WIN32
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    // Get the directory of the .exe
    char* lastSlash = strrchr(exePath, '\\');
    if (lastSlash) {
        *lastSlash = '\0';
    }

    // exeDir + "/" + path
    g_basePath() = exePath;
    g_basePath() += PATH_SEP;
    g_basePath() += path;
#else
    g_basePath() = path;
#endif

    OSReport("[DvdEmu] Base path set to: %s\n", g_basePath().c_str());
}

const char* getBasePath() {
    return g_basePath().c_str();
}

std::string convertPath(const char* gcPath) {
    std::string result = g_basePath();

    // Skip leading slashes
    const char* p = gcPath;
    while (*p == '/' || *p == '\\')
        p++;

    result += PATH_SEP;

    // Append path, converting slashes
    while (*p) {
        if (*p == '/' || *p == '\\') {
            result += PATH_SEP;
        } else {
            result += *p;
        }
        p++;
    }

    return result;
}

bool fileExists(const char* gcPath) {
    std::string fullPath = convertPath(gcPath);

#ifdef _WIN32
    DWORD attrib = GetFileAttributesA(fullPath.c_str());
    bool exists = (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat st;
    bool exists = (stat(fullPath.c_str(), &st) == 0 && S_ISREG(st.st_mode));
#endif

    if (exists) {
        OSReport("[DvdEmu] FOUND: %s\n", gcPath);
    } else {
        OSReport("[DvdEmu] MISSING: %s\n", gcPath);
    }

    return exists;
}

u32 getFileSize(const char* gcPath) {
    std::string fullPath = convertPath(gcPath);
    FILE* f = fopen(fullPath.c_str(), "rb");
    if (!f)
        return 0;

    fseek(f, 0, SEEK_END);
    u32 size = (u32)ftell(f);
    fclose(f);
    return size;
}

void* loadFile(const char* gcPath, u32* outSize, void* heap) {
    std::string fullPath = convertPath(gcPath);

    OSReport("[DvdEmu] Loading request: '%s'\n", gcPath);

    FILE* f = fopen(fullPath.c_str(), "rb");
    if (!f) {
        OSReport("[DvdEmu] ERROR: Failed to open file at physical path: %s\n", fullPath.c_str());
        if (outSize)
            *outSize = 0;
        return nullptr;
    }

    fseek(f, 0, SEEK_END);
    u32 size = (u32)ftell(f);
    fseek(f, 0, SEEK_SET);

    // Allocate with 32-byte alignment (matching GameCube)
    void* data;
#ifdef _WIN32
    data = _aligned_malloc(size, 32);
#else
    data = aligned_alloc(32, (size + 31) & ~31);
#endif

    if (!data) {
        OSReport("[DvdEmu] FATAL: Failed to allocate %u bytes for %s\n", size, gcPath);
        fclose(f);
        if (outSize)
            *outSize = 0;
        return nullptr;
    }

    u32 bytesRead = (u32)fread(data, 1, size, f);
    fclose(f);

    if (bytesRead != size) {
        OSReport("[DvdEmu] WARNING: Read error: expected %u, got %u for %s\n", size, bytesRead,
                 gcPath);
    }

    if (outSize)
        *outSize = bytesRead;

    OSReport("[DvdEmu] SUCCESS: Loaded %s (%u bytes)\n", gcPath, bytesRead);
    return data;
}

u32 loadFileToBuffer(const char* gcPath, void* buffer, u32 bufferSize, u32 offset) {
    std::string fullPath = convertPath(gcPath);

    FILE* f = fopen(fullPath.c_str(), "rb");
    if (!f) {
        OSReport("[DvdEmu] Failed to open file for buffer load: %s\n", fullPath.c_str());
        return 0;
    }

    if (offset > 0) {
        fseek(f, offset, SEEK_SET);
    }

    u32 bytesRead = (u32)fread(buffer, 1, bufferSize, f);
    fclose(f);

    return bytesRead;
}

}  // namespace DvdEmu

// Entry-Number System (emulates DVD's entry system)

s32 DVDConvertPathToEntrynum_Emu(const char* path) {
    if (!DvdEmu::fileExists(path)) {
        OSReport("[DVD] Error: File not found for entrynum conversion: %s\n", path);
        return -1;
    }

    // Check if already registered
    for (const auto& pair : getEntryPaths()) {
        if (pair.second == path) {
            return pair.first;
        }
    }

    // Assign new entry number
    s32 entryNum = g_nextEntryNum++;
    getEntryPaths()[entryNum] = path;

    return entryNum;
}

void DVDRegisterPath(s32 entryNum, const char* path) {
    getEntryPaths()[entryNum] = path;
}

const char* DVDGetPathForEntry(s32 entryNum) {
    auto it = getEntryPaths().find(entryNum);
    if (it != getEntryPaths().end()) {
        return it->second.c_str();
    }
    return nullptr;
}
