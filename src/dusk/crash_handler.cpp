#if !defined(_WIN32) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif

#include "dusk/crash_handler.h"

#include "dusk/logging.h"
#include "version.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>

#if defined(_WIN32)

#include <windows.h>

#include <io.h>

#if defined(DUSK_CRASH_DBGHELP)
#include <dbghelp.h>
#endif

#else

#include <csignal>
#include <cstdlib>
#include <dlfcn.h>
#include <sys/ucontext.h>
#include <unistd.h>
#include <unwind.h>

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#else
#include <elf.h>
#include <link.h>
#ifndef NT_GNU_BUILD_ID
#define NT_GNU_BUILD_ID 3
#endif
#endif

#endif

#ifndef DUSK_ARCH
#define DUSK_ARCH "unknown"
#endif

namespace dusk::crash_handler {
namespace {

constexpr int kStderrFd = 2;
constexpr int kMaxFrames = 128;
constexpr char kHexDigits[] = "0123456789abcdef";

struct CrashContext {
    uintptr_t moduleBase = 0;
    char modulePath[1024] = {};
    uint8_t buildId[64] = {};
    unsigned buildIdLen = 0;
    unsigned pdbAge = 0;
};
CrashContext g_ctx;

struct ModuleInfo {
    uintptr_t base = 0;
    uintptr_t size = 0;
    char path[1024] = {};
    uint8_t buildId[64] = {};
    unsigned buildIdLen = 0;
    unsigned pdbAge = 0;
};

void rawWrite(int fd, const char* data, size_t len) {
    if (fd < 0) {
        return;
    }
#if defined(_WIN32)
    _write(fd, data, static_cast<unsigned int>(len));
#else
    while (len > 0) {
        const ssize_t written = ::write(fd, data, len);
        if (written <= 0) {
            return;
        }
        data += written;
        len -= static_cast<size_t>(written);
    }
#endif
}

void writeStr(int fd, const char* s) {
    if (s != nullptr) {
        rawWrite(fd, s, std::strlen(s));
    }
}

void writeHex(int fd, unsigned long long value) {
    char buf[2 + 16];
    size_t o = sizeof(buf);
    do {
        buf[--o] = kHexDigits[value & 0xF];
        value >>= 4;
    } while (value != 0);
    buf[--o] = 'x';
    buf[--o] = '0';
    rawWrite(fd, buf + o, sizeof(buf) - o);
}

void writeDec(int fd, unsigned int value) {
    char buf[10];
    size_t o = sizeof(buf);
    do {
        buf[--o] = static_cast<char>('0' + value % 10);
        value /= 10;
    } while (value != 0);
    rawWrite(fd, buf + o, sizeof(buf) - o);
}

void writeHexBytes(int fd, const uint8_t* data, unsigned len) {
    char buf[2];
    for (unsigned i = 0; i < len; ++i) {
        buf[0] = kHexDigits[data[i] >> 4];
        buf[1] = kHexDigits[data[i] & 0xF];
        rawWrite(fd, buf, 2);
    }
}

void writeHexByte(int fd, uint8_t value) {
    char buf[2];
    buf[0] = kHexDigits[value >> 4];
    buf[1] = kHexDigits[value & 0xF];
    rawWrite(fd, buf, 2);
}

void writeQuoted(int fd, const char* s) {
    writeStr(fd, "\"");
    if (s != nullptr) {
        for (const char* p = s; *p != '\0'; ++p) {
            if (*p == '"' || *p == '\\') {
                rawWrite(fd, "\\", 1);
            }
            rawWrite(fd, p, 1);
        }
    }
    writeStr(fd, "\"");
}

const char* baseName(const char* path) {
    const char* name = path;
    for (const char* p = path; p != nullptr && *p != '\0'; ++p) {
        if (*p == '/' || *p == '\\') {
            name = p + 1;
        }
    }
    return name[0] != '\0' ? name : "(unknown)";
}

void writeBuildId(int fd, const uint8_t* buildId, unsigned buildIdLen, unsigned pdbAge) {
    if (buildIdLen == 0) {
        writeStr(fd, "(unavailable)");
        return;
    }
#if defined(_WIN32)
    if (buildIdLen == 16) {
        writeHexByte(fd, buildId[3]);
        writeHexByte(fd, buildId[2]);
        writeHexByte(fd, buildId[1]);
        writeHexByte(fd, buildId[0]);
        writeStr(fd, "-");
        writeHexByte(fd, buildId[5]);
        writeHexByte(fd, buildId[4]);
        writeStr(fd, "-");
        writeHexByte(fd, buildId[7]);
        writeHexByte(fd, buildId[6]);
        writeStr(fd, "-");
        writeHexByte(fd, buildId[8]);
        writeHexByte(fd, buildId[9]);
        writeStr(fd, "-");
        writeHexBytes(fd, buildId + 10, 6);
        if (pdbAge != 0) {
            writeStr(fd, "-");
            writeDec(fd, pdbAge);
        }
        return;
    }
#else
    (void)pdbAge;
#endif
    writeHexBytes(fd, buildId, buildIdLen);
}

const char* symbolFor(uintptr_t pc, unsigned long long* disp) {
#if defined(_WIN32) && defined(DUSK_CRASH_DBGHELP)
    alignas(SYMBOL_INFO) static char storage[sizeof(SYMBOL_INFO) + 512];
    auto* sym = reinterpret_cast<SYMBOL_INFO*>(storage);
    sym->SizeOfStruct = sizeof(SYMBOL_INFO);
    sym->MaxNameLen = 511;
    DWORD64 d = 0;
    if (SymFromAddr(GetCurrentProcess(), pc, &d, sym)) {
        *disp = d;
        return sym->Name;
    }
    return nullptr;
#elif defined(_WIN32)
    (void)pc;
    (void)disp;
    return nullptr;
#else
    Dl_info info;
    if (dladdr(reinterpret_cast<void*>(pc), &info) != 0 && info.dli_sname != nullptr) {
        const auto base = reinterpret_cast<uintptr_t>(info.dli_saddr);
        *disp = pc >= base ? pc - base : 0;
        return info.dli_sname;
    }
    return nullptr;
#endif
}

void fallbackModuleInfo(ModuleInfo& info) {
    info = {};
    info.base = g_ctx.moduleBase;
    std::strncpy(info.path, g_ctx.modulePath, sizeof(info.path) - 1);
    if (g_ctx.buildIdLen > sizeof(info.buildId)) {
        info.buildIdLen = sizeof(info.buildId);
    } else {
        info.buildIdLen = g_ctx.buildIdLen;
    }
    if (info.buildIdLen != 0) {
        std::memcpy(info.buildId, g_ctx.buildId, info.buildIdLen);
    }
    info.pdbAge = g_ctx.pdbAge;
}

bool findModuleInfo(uintptr_t pc, ModuleInfo& info);

void emitAddressDetail(int fd, uintptr_t pc) {
    ModuleInfo info;
    findModuleInfo(pc, info);
    const uintptr_t rva = pc >= info.base ? pc - info.base : 0ull;
    writeHex(fd, pc);
    writeStr(fd, " module_base=");
    writeHex(fd, info.base);
    if (info.size != 0) {
        writeStr(fd, " image_size=");
        writeHex(fd, info.size);
    }
    writeStr(fd, " rva=");
    writeHex(fd, rva);
    writeStr(fd, " module=");
    writeQuoted(fd, info.path[0] != '\0' ? info.path : baseName(g_ctx.modulePath));
    writeStr(fd, " build_id=");
    writeBuildId(fd, info.buildId, info.buildIdLen, info.pdbAge);
}

void emitFrame(int fd, int index, uintptr_t pc) {
    ModuleInfo info;
    findModuleInfo(pc, info);
    const uintptr_t rva = pc >= info.base ? pc - info.base : 0ull;

    writeStr(fd, "#");
    if (index < 10) {
        writeStr(fd, "0");
    }
    writeDec(fd, static_cast<unsigned int>(index));
    writeStr(fd, " abs=");
    writeHex(fd, pc);
    writeStr(fd, " module_base=");
    writeHex(fd, info.base);
    if (info.size != 0) {
        writeStr(fd, " image_size=");
        writeHex(fd, info.size);
    }
    writeStr(fd, " rva=");
    writeHex(fd, rva);
    writeStr(fd, " module=");
    writeQuoted(fd, info.path[0] != '\0' ? info.path : baseName(g_ctx.modulePath));
    writeStr(fd, " build_id=");
    writeBuildId(fd, info.buildId, info.buildIdLen, info.pdbAge);
    unsigned long long disp = 0;
    const char* sym = symbolFor(pc, &disp);
    if (sym != nullptr && sym[0] != '\0') {
        writeStr(fd, " ");
        writeStr(fd, sym);
        writeStr(fd, "+");
        writeHex(fd, disp);
    }
    writeStr(fd, "\n");
}

void emitHeader(int fd, const char* reason, unsigned long long code, bool hasCode,
    uintptr_t faultAddr, uintptr_t crashPc, bool crashPcKnown) {
    writeStr(fd, "\n==================== DUSKLIGHT CRASHED ====================\n");
    writeStr(fd, "Build:       " DUSK_WC_DESCRIBE " (" DUSK_WC_BRANCH ")\n");
    writeStr(fd, "Revision:    " DUSK_WC_REVISION "  Date: " DUSK_WC_DATE
                 "  Type: " DUSK_BUILD_TYPE "\n");
    writeStr(fd, "Platform:    " DUSK_PLATFORM_NAME " / " DUSK_ARCH "\n");
    writeStr(fd, "Module:      ");
    writeStr(fd, g_ctx.modulePath[0] != '\0' ? g_ctx.modulePath : "(unknown)");
    writeStr(fd, "\nModule base: ");
    writeHex(fd, g_ctx.moduleBase);
    writeStr(fd, "\nBuild-ID:    ");
    writeBuildId(fd, g_ctx.buildId, g_ctx.buildIdLen, g_ctx.pdbAge);
    writeStr(fd, "\nReason:      ");
    writeStr(fd, reason);
    if (hasCode) {
        writeStr(fd, " (");
        writeHex(fd, code);
        writeStr(fd, ")");
    }
    writeStr(fd, "\nFault addr:  ");
    writeHex(fd, faultAddr);
    writeStr(fd, "\nCrash PC:    ");
    if (crashPcKnown) {
        emitAddressDetail(fd, crashPc);
    } else {
        writeStr(fd, "(unavailable on this platform)");
    }
    writeStr(fd, "\n");
    writeStr(fd, "Backtrace:\n");
}

void emitFooter(int fd) {
    writeStr(fd, "========================================================\n");
}

#if defined(_WIN32)

LONG g_inHandler = 0;
LPTOP_LEVEL_EXCEPTION_FILTER g_prevFilter = nullptr;

bool readPeModuleInfo(uintptr_t moduleBase, ModuleInfo& info) {
    const auto* base = reinterpret_cast<const uint8_t*>(moduleBase);
    if (base == nullptr) {
        return false;
    }
    const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
        return false;
    }
    const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) {
        return false;
    }
    info.base = moduleBase;
    info.size = nt->OptionalHeader.SizeOfImage;
    const IMAGE_DATA_DIRECTORY& dir =
        nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG];
    if (dir.VirtualAddress == 0 || dir.Size == 0) {
        return true;
    }
    const auto* dbg = reinterpret_cast<const IMAGE_DEBUG_DIRECTORY*>(base + dir.VirtualAddress);
    const unsigned count = dir.Size / sizeof(IMAGE_DEBUG_DIRECTORY);
    for (unsigned i = 0; i < count; ++i) {
        if (dbg[i].Type != IMAGE_DEBUG_TYPE_CODEVIEW) {
            continue;
        }
        const auto* cv = base + dbg[i].AddressOfRawData;
        if (std::memcmp(cv, "RSDS", 4) != 0) {
            continue;
        }
        std::memcpy(info.buildId, cv + 4, sizeof(GUID));
        info.buildIdLen = sizeof(GUID);
        std::memcpy(&info.pdbAge, cv + 4 + sizeof(GUID), sizeof(info.pdbAge));
        break;
    }
    return true;
}

void captureBuildId() {
    ModuleInfo info;
    if (!readPeModuleInfo(g_ctx.moduleBase, info)) {
        return;
    }
    g_ctx.buildIdLen = info.buildIdLen;
    if (g_ctx.buildIdLen != 0) {
        std::memcpy(g_ctx.buildId, info.buildId, g_ctx.buildIdLen);
    }
    g_ctx.pdbAge = info.pdbAge;
}

bool findModuleInfo(uintptr_t pc, ModuleInfo& info) {
    fallbackModuleInfo(info);
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(reinterpret_cast<LPCVOID>(pc), &mbi, sizeof(mbi)) == 0 ||
        mbi.AllocationBase == nullptr) {
        return false;
    }
    const auto moduleBase = reinterpret_cast<uintptr_t>(mbi.AllocationBase);
    info = {};
    info.base = moduleBase;
    GetModuleFileNameA(reinterpret_cast<HMODULE>(moduleBase), info.path,
        static_cast<DWORD>(sizeof(info.path) - 1));
    readPeModuleInfo(moduleBase, info);
    return true;
}

const char* exceptionName(DWORD code) {
    switch (code) {
    case EXCEPTION_ACCESS_VIOLATION:
        return "EXCEPTION_ACCESS_VIOLATION";
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        return "EXCEPTION_ILLEGAL_INSTRUCTION";
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        return "EXCEPTION_INT_DIVIDE_BY_ZERO";
    case EXCEPTION_STACK_OVERFLOW:
        return "EXCEPTION_STACK_OVERFLOW";
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        return "EXCEPTION_DATATYPE_MISALIGNMENT";
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
    default:
        return "EXCEPTION";
    }
}

int captureBacktraceWin(CONTEXT ctx, uintptr_t* out, int cap) {
    int n = 0;
    while (n < cap) {
#if defined(_M_X64)
        const DWORD64 ip = ctx.Rip;
#elif defined(_M_ARM64)
        const DWORD64 ip = ctx.Pc;
#else
        const DWORD64 ip = 0;
#endif
        if (ip == 0) {
            break;
        }
        out[n++] = static_cast<uintptr_t>(ip);
#if defined(_M_X64) || defined(_M_ARM64)
        DWORD64 imageBase = 0;
        PRUNTIME_FUNCTION fn = RtlLookupFunctionEntry(ip, &imageBase, nullptr);
        if (fn != nullptr) {
            PVOID handlerData = nullptr;
            DWORD64 establisherFrame = 0;
            RtlVirtualUnwind(UNW_FLAG_NHANDLER, imageBase, ip, fn, &ctx, &handlerData,
                &establisherFrame, nullptr);
            continue;
        }
#if defined(_M_X64)
        if (ctx.Rsp == 0) {
            break;
        }
        ctx.Rip = *reinterpret_cast<const DWORD64*>(ctx.Rsp);
        ctx.Rsp += sizeof(DWORD64);
#else
        if (ctx.Lr == 0 || ctx.Lr == ip) {
            break;
        }
        ctx.Pc = ctx.Lr;
        ctx.Lr = 0;
#endif
#else
        break;
#endif
    }
    return n;
}

void emit(int fd, EXCEPTION_POINTERS* ep) {
    if (fd < 0) {
        return;
    }

    const DWORD code = ep->ExceptionRecord->ExceptionCode;
    const uintptr_t pc = reinterpret_cast<uintptr_t>(ep->ExceptionRecord->ExceptionAddress);
    uintptr_t faultAddr = 0;
    if (code == EXCEPTION_ACCESS_VIOLATION && ep->ExceptionRecord->NumberParameters >= 2) {
        faultAddr = static_cast<uintptr_t>(ep->ExceptionRecord->ExceptionInformation[1]);
    }

    emitHeader(fd, exceptionName(code), code, true, faultAddr, pc, true);

    uintptr_t frames[kMaxFrames];
    const int frameCount = captureBacktraceWin(*ep->ContextRecord, frames, kMaxFrames);
    for (int i = 0; i < frameCount; ++i) {
        emitFrame(fd, i, frames[i]);
    }

    emitFooter(fd);
}

LONG WINAPI windowsHandler(EXCEPTION_POINTERS* ep) {
    if (InterlockedCompareExchange(&g_inHandler, 1, 0) != 0) {
        return EXCEPTION_CONTINUE_SEARCH;
    }
    emit(kStderrFd, ep);
    const int logFd = dusk::GetLogFileDescriptor();
    if (logFd >= 0) {
        emit(logFd, ep);
    }
    if (g_prevFilter != nullptr) {
        return g_prevFilter(ep);
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

#else

constexpr int kSignals[] = {SIGSEGV, SIGBUS, SIGABRT, SIGILL, SIGFPE};
constexpr int kSignalCount = static_cast<int>(sizeof(kSignals) / sizeof(kSignals[0]));
constexpr int kAltStackSize = 128 * 1024;

volatile std::sig_atomic_t g_inHandler = 0;
char g_altStack[kAltStackSize];
struct sigaction g_prev[kSignalCount];
std::terminate_handler g_prevTerminate = nullptr;

void crashRegs(void* ucv, uintptr_t& pc, uintptr_t& lr, uintptr_t& fp) {
    pc = 0;
    lr = 0;
    fp = 0;
    if (ucv == nullptr) {
        return;
    }
    auto* uc = static_cast<ucontext_t*>(ucv);
#if defined(__APPLE__)
#if defined(__aarch64__) || defined(__arm64__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext->__ss.__pc);
    lr = static_cast<uintptr_t>(uc->uc_mcontext->__ss.__lr);
    fp = static_cast<uintptr_t>(uc->uc_mcontext->__ss.__fp);
#elif defined(__x86_64__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext->__ss.__rip);
    fp = static_cast<uintptr_t>(uc->uc_mcontext->__ss.__rbp);
#endif
#elif defined(__ANDROID__)
#if defined(__aarch64__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext.pc);
    lr = static_cast<uintptr_t>(uc->uc_mcontext.regs[30]);
    fp = static_cast<uintptr_t>(uc->uc_mcontext.regs[29]);
#elif defined(__x86_64__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_RIP]);
    fp = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_RBP]);
#elif defined(__arm__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext.arm_pc);
    lr = static_cast<uintptr_t>(uc->uc_mcontext.arm_lr);
    fp = static_cast<uintptr_t>(uc->uc_mcontext.arm_fp);
#elif defined(__i386__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_EIP]);
    fp = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_EBP]);
#endif
#elif defined(__linux__)
#if defined(__x86_64__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_RIP]);
    fp = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_RBP]);
#elif defined(__aarch64__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext.pc);
    lr = static_cast<uintptr_t>(uc->uc_mcontext.regs[30]);
    fp = static_cast<uintptr_t>(uc->uc_mcontext.regs[29]);
#elif defined(__i386__)
    pc = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_EIP]);
    fp = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_EBP]);
#endif
#endif
}

bool pcNearFunctionEntry(uintptr_t pc) {
    constexpr uintptr_t kPrologueWindow = 20;
    Dl_info info;
    if (dladdr(reinterpret_cast<void*>(pc), &info) == 0 || info.dli_saddr == nullptr) {
        return false;
    }
    const auto start = reinterpret_cast<uintptr_t>(info.dli_saddr);
    return pc >= start && pc - start <= kPrologueWindow;
}

int captureBacktraceFP(uintptr_t pc, uintptr_t lr, uintptr_t fp, uintptr_t* out, int cap) {
    int n = 0;
    if (pc != 0 && n < cap) {
        out[n++] = pc;
    }
    bool dedupeLr = false;
    if (lr != 0 && lr != pc && n < cap && pcNearFunctionEntry(pc)) {
        out[n++] = lr;
        dedupeLr = true;
    }
    uintptr_t cur = fp;
    uintptr_t prev = 0;
    constexpr uintptr_t kMaxFrameSpan = 16u << 20;
    while (n < cap) {
        if (cur == 0 || (cur & (sizeof(uintptr_t) - 1)) != 0 || cur <= prev) {
            break;
        }
        const auto* slot = reinterpret_cast<const uintptr_t*>(cur);
        const uintptr_t next = slot[0];
        const uintptr_t ret = slot[1];
        if (ret == 0) {
            break;
        }
        const bool skip = dedupeLr && ret == lr;
        dedupeLr = false;
        if (!skip) {
            out[n++] = ret;
        }
        if (next != 0 && next > cur && next - cur > kMaxFrameSpan) {
            break;
        }
        prev = cur;
        cur = next;
    }
    return n;
}

struct UnwindState {
    uintptr_t* pcs;
    int count;
    int cap;
    int skip;
};

_Unwind_Reason_Code unwindCb(struct _Unwind_Context* ctx, void* arg) {
    auto* s = static_cast<UnwindState*>(arg);
    const uintptr_t ip = static_cast<uintptr_t>(_Unwind_GetIP(ctx));
    if (ip == 0) {
        return _URC_END_OF_STACK;
    }
    if (s->skip > 0) {
        --s->skip;
        return _URC_NO_REASON;
    }
    if (s->count >= s->cap) {
        return _URC_END_OF_STACK;
    }
    s->pcs[s->count++] = ip;
    return _URC_NO_REASON;
}

int captureBacktrace(uintptr_t* pcs, int cap, int skip) {
    UnwindState s{pcs, 0, cap, skip};
    _Unwind_Backtrace(&unwindCb, &s);
    return s.count;
}

void prewarmUnwinder() {
    uintptr_t warm[4];
    captureBacktrace(warm, 4, 0);
}

#if defined(__APPLE__)

bool readMachBuildId(uintptr_t moduleBase, ModuleInfo& info) {
    const auto* header = reinterpret_cast<const struct mach_header_64*>(moduleBase);
    if (header == nullptr || header->magic != MH_MAGIC_64) {
        return false;
    }
    const auto* lc = reinterpret_cast<const struct load_command*>(
        reinterpret_cast<const char*>(header) + sizeof(struct mach_header_64));
    for (uint32_t i = 0; i < header->ncmds; ++i) {
        if (lc->cmd == LC_UUID) {
            const auto* uuid = reinterpret_cast<const struct uuid_command*>(lc);
            std::memcpy(info.buildId, uuid->uuid, sizeof(uuid->uuid));
            info.buildIdLen = sizeof(uuid->uuid);
            return true;
        }
        lc = reinterpret_cast<const struct load_command*>(
            reinterpret_cast<const char*>(lc) + lc->cmdsize);
    }
    return true;
}

void captureBuildId() {
    ModuleInfo info;
    if (!readMachBuildId(g_ctx.moduleBase, info)) {
        return;
    }
    g_ctx.buildIdLen = info.buildIdLen;
    if (g_ctx.buildIdLen != 0) {
        std::memcpy(g_ctx.buildId, info.buildId, g_ctx.buildIdLen);
    }
}

#else

bool segmentContains(const dl_phdr_info* info, uintptr_t addr) {
    for (int i = 0; i < info->dlpi_phnum; ++i) {
        const ElfW(Phdr)& ph = info->dlpi_phdr[i];
        if (ph.p_type != PT_LOAD) {
            continue;
        }
        const uintptr_t start = info->dlpi_addr + ph.p_vaddr;
        if (addr >= start && addr < start + ph.p_memsz) {
            return true;
        }
    }
    return false;
}

void readElfModuleInfo(const dl_phdr_info* info, ModuleInfo& module) {
    uintptr_t minAddr = ~static_cast<uintptr_t>(0);
    uintptr_t maxAddr = 0;
    for (int i = 0; i < info->dlpi_phnum; ++i) {
        const ElfW(Phdr)& ph = info->dlpi_phdr[i];
        if (ph.p_type != PT_LOAD) {
            continue;
        }
        const uintptr_t start = info->dlpi_addr + ph.p_vaddr;
        const uintptr_t end = start + ph.p_memsz;
        if (start < minAddr) {
            minAddr = start;
        }
        if (end > maxAddr) {
            maxAddr = end;
        }
    }
    if (minAddr <= maxAddr && maxAddr != 0) {
        module.base = minAddr;
        module.size = maxAddr - minAddr;
    }

    for (int i = 0; i < info->dlpi_phnum; ++i) {
        const ElfW(Phdr)& ph = info->dlpi_phdr[i];
        if (ph.p_type != PT_NOTE) {
            continue;
        }
        const auto* p = reinterpret_cast<const uint8_t*>(info->dlpi_addr + ph.p_vaddr);
        const uint8_t* end = p + ph.p_memsz;
        while (p + sizeof(ElfW(Nhdr)) <= end) {
            const auto* nh = reinterpret_cast<const ElfW(Nhdr)*>(p);
            const char* name = reinterpret_cast<const char*>(nh + 1);
            const uint8_t* desc =
                reinterpret_cast<const uint8_t*>(name + ((nh->n_namesz + 3) & ~3u));
            if (nh->n_type == NT_GNU_BUILD_ID && nh->n_namesz == 4 &&
                std::memcmp(name, "GNU", 4) == 0) {
                unsigned n = nh->n_descsz;
                if (n > sizeof(module.buildId)) {
                    n = sizeof(module.buildId);
                }
                std::memcpy(module.buildId, desc, n);
                module.buildIdLen = n;
                return;
            }
            p = desc + ((nh->n_descsz + 3) & ~3u);
        }
    }
}

int elfBuildIdCallback(dl_phdr_info* info, size_t, void* arg) {
    const auto self = *static_cast<const uintptr_t*>(arg);
    if (!segmentContains(info, self)) {
        return 0;
    }
    ModuleInfo module;
    readElfModuleInfo(info, module);
    g_ctx.buildIdLen = module.buildIdLen;
    if (g_ctx.buildIdLen != 0) {
        std::memcpy(g_ctx.buildId, module.buildId, g_ctx.buildIdLen);
    }
    return 1;
}

void captureBuildId() {
    uintptr_t self = reinterpret_cast<uintptr_t>(&install);
    dl_iterate_phdr(&elfBuildIdCallback, &self);
}

#endif

#if !defined(__APPLE__)
struct ElfModuleSearch {
    uintptr_t pc;
    ModuleInfo* module;
};

int elfModuleInfoCallback(dl_phdr_info* info, size_t, void* arg) {
    auto* search = static_cast<ElfModuleSearch*>(arg);
    if (!segmentContains(info, search->pc)) {
        return 0;
    }
    if (info->dlpi_name != nullptr && info->dlpi_name[0] != '\0') {
        std::strncpy(search->module->path, info->dlpi_name,
            sizeof(search->module->path) - 1);
    }
    readElfModuleInfo(info, *search->module);
    return 1;
}
#endif

bool findModuleInfo(uintptr_t pc, ModuleInfo& info) {
    fallbackModuleInfo(info);
    Dl_info moduleInfo;
    if (dladdr(reinterpret_cast<void*>(pc), &moduleInfo) == 0) {
        return false;
    }
    if (moduleInfo.dli_fbase != nullptr) {
        info.base = reinterpret_cast<uintptr_t>(moduleInfo.dli_fbase);
    }
    if (moduleInfo.dli_fname != nullptr && moduleInfo.dli_fname[0] != '\0') {
        info.path[0] = '\0';
        std::strncpy(info.path, moduleInfo.dli_fname, sizeof(info.path) - 1);
    }
    info.buildIdLen = 0;
    info.pdbAge = 0;
#if defined(__APPLE__)
    readMachBuildId(info.base, info);
#else
    ElfModuleSearch search{pc, &info};
    dl_iterate_phdr(&elfModuleInfoCallback, &search);
#endif
    return true;
}

const char* signalName(int sig) {
    switch (sig) {
    case SIGSEGV:
        return "SIGSEGV (segmentation fault)";
    case SIGBUS:
        return "SIGBUS (bus error)";
    case SIGABRT:
        return "SIGABRT (abort)";
    case SIGILL:
        return "SIGILL (illegal instruction)";
    case SIGFPE:
        return "SIGFPE (floating point exception)";
    default:
        return "unknown signal";
    }
}

void emit(int fd, int sig, siginfo_t* info, const uintptr_t* frames, int frameCount,
    uintptr_t pc) {
    if (fd < 0) {
        return;
    }
    const uintptr_t faultAddr =
        info != nullptr ? reinterpret_cast<uintptr_t>(info->si_addr) : 0;
    emitHeader(fd, signalName(sig), 0, false, faultAddr, pc, pc != 0);
    for (int i = 0; i < frameCount; ++i) {
        emitFrame(fd, i, frames[i]);
    }
    emitFooter(fd);
}

void chainPrevious(int sig, siginfo_t* info, void* uc) {
    for (int i = 0; i < kSignalCount; ++i) {
        if (kSignals[i] != sig) {
            continue;
        }
        const struct sigaction& o = g_prev[i];
        if ((o.sa_flags & SA_SIGINFO) != 0) {
            if (o.sa_sigaction != nullptr) {
                o.sa_sigaction(sig, info, uc);
                return;
            }
        } else {
            if (o.sa_handler == SIG_IGN) {
                return;
            }
            if (o.sa_handler != SIG_DFL && o.sa_handler != nullptr) {
                o.sa_handler(sig);
                return;
            }
        }
        break;
    }
    ::signal(sig, SIG_DFL);
    ::raise(sig);
}

void handler(int sig, siginfo_t* info, void* ucv) {
    if (g_inHandler != 0) {
        _exit(128 + sig);
    }
    g_inHandler = 1;

    uintptr_t pc = 0;
    uintptr_t lr = 0;
    uintptr_t fp = 0;
    crashRegs(ucv, pc, lr, fp);
    uintptr_t frames[kMaxFrames];
    int frameCount = captureBacktraceFP(pc, lr, fp, frames, kMaxFrames);
    if (frameCount < 2) {
        frameCount = captureBacktrace(frames, kMaxFrames, 2);
    }

    emit(kStderrFd, sig, info, frames, frameCount, pc);
    const int logFd = dusk::GetLogFileDescriptor();
    if (logFd >= 0) {
        emit(logFd, sig, info, frames, frameCount, pc);
        ::fsync(logFd);
    }

    chainPrevious(sig, info, ucv);
}

void writeTerminateMessage(int fd, const char* body, const char* what) {
    writeStr(fd, "\nterminate: ");
    writeStr(fd, body);
    writeStr(fd, what);
    writeStr(fd, "\n");
}

void onTerminate() {
    const char* body = "unknown reason";
    const char* what = nullptr;
    if (std::exception_ptr ep = std::current_exception()) {
        try {
            std::rethrow_exception(ep);
        } catch (const std::exception& e) {
            body = "uncaught exception: ";
            what = e.what();
        } catch (...) {
            body = "uncaught non-std exception";
        }
    } else {
        body = "no active exception";
    }
    writeTerminateMessage(kStderrFd, body, what);
    writeTerminateMessage(dusk::GetLogFileDescriptor(), body, what);
    if (g_prevTerminate != nullptr) {
        g_prevTerminate();
    }
    std::abort();
}

#endif

}  // namespace

void install() {
#if defined(_WIN32)
    g_ctx.moduleBase = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    GetModuleFileNameA(nullptr, g_ctx.modulePath, sizeof(g_ctx.modulePath) - 1);
    captureBuildId();
#if defined(DUSK_CRASH_DBGHELP)
    SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
    SymInitialize(GetCurrentProcess(), nullptr, TRUE);
#endif
    g_prevFilter = SetUnhandledExceptionFilter(&windowsHandler);
#else
    Dl_info moduleInfo;
    if (dladdr(reinterpret_cast<void*>(&install), &moduleInfo) != 0) {
        g_ctx.moduleBase = reinterpret_cast<uintptr_t>(moduleInfo.dli_fbase);
        if (moduleInfo.dli_fname != nullptr) {
            std::strncpy(g_ctx.modulePath, moduleInfo.dli_fname,
                sizeof(g_ctx.modulePath) - 1);
        }
    }
    captureBuildId();
    prewarmUnwinder();

    static stack_t altStack;
    altStack.ss_sp = g_altStack;
    altStack.ss_size = sizeof(g_altStack);
    altStack.ss_flags = 0;
    sigaltstack(&altStack, nullptr);

    struct sigaction sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = &handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;

    for (int i = 0; i < kSignalCount; ++i) {
        sigaction(kSignals[i], &sa, &g_prev[i]);
    }

    g_prevTerminate = std::set_terminate(&onTerminate);
#endif
}

}  // namespace dusk::crash_handler
