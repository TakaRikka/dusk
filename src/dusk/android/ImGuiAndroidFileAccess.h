#pragma once

typedef void* ImFileHandle;
typedef unsigned long long ImU64;

ImFileHandle ImFileOpen(const char* filename, const char* mode);
bool         ImFileClose(ImFileHandle f);
ImU64        ImFileGetSize(ImFileHandle f);
ImU64        ImFileRead(void* data, ImU64 sz, ImU64 count, ImFileHandle f);
ImU64        ImFileWrite(const void* data, ImU64 sz, ImU64 count, ImFileHandle f);