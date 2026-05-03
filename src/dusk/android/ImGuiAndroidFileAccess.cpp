#include <dusk/android/ImGuiAndroidFileAccess.h>

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_iostream.h>

ImFileHandle ImFileOpen(const char* filename, const char* mode) {
    return SDL_IOFromFile(filename, mode);
}

bool ImFileClose(ImFileHandle f) {
    if (f) {
        SDL_CloseIO((SDL_IOStream*)f);
    }
    return true;
}

ImU64 ImFileGetSize(ImFileHandle f) {
    return SDL_GetIOSize((SDL_IOStream*)f);
}

ImU64 ImFileRead(void* data, ImU64 sz, ImU64 count, ImFileHandle f) {
    if (f == nullptr)
        return 0;
    auto stream = (SDL_IOStream*)f;

    size_t total = 0;
    size_t length = sz * count;
    auto* dst = static_cast<uint8_t*>(data);
    while (total < length) {
        const size_t read = SDL_ReadIO(stream, dst + total, length - total);
        if (read == 0) {
            if (SDL_GetIOStatus(stream) != SDL_IO_STATUS_EOF) {
                SDL_CloseIO(stream);
                return total;
            }else {
                return length;
            }
        }
        total += read;
    }

    return total;
}

ImU64 ImFileWrite(const void* data, ImU64 sz, ImU64 count, ImFileHandle f) {
    if (f == nullptr) {
        return false;
    }
    auto stream = (SDL_IOStream*)f;

    size_t total = 0;
    size_t length = sz * count;
    auto* src = static_cast<const uint8_t*>(data);
    while (total < length) {
        const size_t written = SDL_WriteIO(stream, src + total, length - total);
        if (written == 0) {
            SDL_CloseIO(stream);
            return false;
        }
        total += written;
    }

    return total;
 }