#include "prepatch.hpp"

#include <atomic>
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "aurora/lib/logging.hpp"

#if DUSK_HAS_PREPATCH
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#endif

namespace dusk::mods::prepatch {
namespace {

aurora::Module Log("dusk::mods::prepatch");

constexpr std::string_view kSiteMagic = "PS01";
constexpr size_t kSiteHeaderSize = 12;
constexpr size_t kGatewaySize = 28;
constexpr size_t kOriginalStubOffset = 20;

constexpr uint32_t kBranchMask = 0xfc000000u;
constexpr uint32_t kBranch = 0x14000000u;
constexpr uint32_t kLdarX16X16 = 0xc8dffe10u;
constexpr uint32_t kCbzX16Stub = 0xb4000050u;
constexpr uint32_t kBrX16 = 0xd61f0200u;
constexpr uint32_t kBtiC = 0xd503245fu;

struct SiteHeader {
    char magic[4];
    int32_t targetDelta;
    int32_t slotDelta;
};
static_assert(sizeof(SiteHeader) == kSiteHeaderSize);

struct Range {
    uintptr_t begin = 0;
    uintptr_t end = 0;

    bool contains(uintptr_t address, size_t size) const {
        return address >= begin && address <= end && size <= end - address;
    }
};

struct State {
    std::vector<Range> executableRanges;
    std::vector<Range> writableRanges;
    std::string failureReason = "not initialized";
    bool loaded = false;
    bool initialized = false;
};
State s_state;

void fail(std::string reason) {
    s_state.failureReason = std::move(reason);
    Log.error("prepatch backend unavailable: {}", s_state.failureReason);
}

void unavailable(std::string reason) {
    s_state.failureReason = std::move(reason);
    Log.info("prepatch backend unavailable: {}", s_state.failureReason);
}

bool add_delta(uintptr_t address, int64_t delta, uintptr_t& out) {
    if (delta >= 0) {
        const auto offset = static_cast<uintptr_t>(delta);
        if (address > std::numeric_limits<uintptr_t>::max() - offset) {
            return false;
        }
        out = address + offset;
    } else {
        const auto offset = static_cast<uintptr_t>(-(delta + 1)) + 1;
        if (address < offset) {
            return false;
        }
        out = address - offset;
    }
    return true;
}

bool branch_target(uintptr_t address, uint32_t instruction, uintptr_t& out) {
    if ((instruction & kBranchMask) != kBranch) {
        return false;
    }
    int64_t words = instruction & 0x03ffffffu;
    if ((words & 0x02000000) != 0) {
        words -= int64_t{1} << 26;
    }
    return add_delta(address, words * 4, out);
}

const Range* containing(const std::vector<Range>& ranges, uintptr_t address, size_t size) {
    for (const auto& range : ranges) {
        if (range.contains(address, size)) {
            return &range;
        }
    }
    return nullptr;
}

#if DUSK_HAS_PREPATCH
bool slide_address(intptr_t slide, uint64_t vmaddr, uintptr_t& out) {
    if (vmaddr > std::numeric_limits<uintptr_t>::max()) {
        return false;
    }
    return add_delta(static_cast<uintptr_t>(vmaddr), slide, out);
}
#endif

}  // namespace

void initialize() {
    if (s_state.initialized) {
        return;
    }
    s_state.initialized = true;

#if DUSK_HAS_PREPATCH
    const auto* imageHeader = reinterpret_cast<const mach_header_64*>(_dyld_get_image_header(0));
    if (imageHeader == nullptr || imageHeader->magic != MH_MAGIC_64 ||
        imageHeader->cputype != CPU_TYPE_ARM64 ||
        (imageHeader->cpusubtype & ~CPU_SUBTYPE_MASK) == CPU_SUBTYPE_ARM64E ||
        (imageHeader->cpusubtype & CPU_SUBTYPE_ARM64_PTR_AUTH_MASK) != 0)
    {
        fail("main image is not a supported 64-bit arm64 Mach-O image");
        return;
    }

    const intptr_t slide = _dyld_get_image_vmaddr_slide(0);
    const auto* commands = reinterpret_cast<const uint8_t*>(imageHeader + 1);
    size_t commandOffset = 0;
    for (uint32_t i = 0; i < imageHeader->ncmds; ++i) {
        if (commandOffset > imageHeader->sizeofcmds ||
            imageHeader->sizeofcmds - commandOffset < sizeof(load_command))
        {
            fail("main image has malformed load commands");
            return;
        }
        const auto* command = reinterpret_cast<const load_command*>(commands + commandOffset);
        if (command->cmdsize < sizeof(load_command) ||
            command->cmdsize > imageHeader->sizeofcmds - commandOffset)
        {
            fail("main image has malformed load commands");
            return;
        }
        if (command->cmd == LC_SEGMENT_64) {
            if (command->cmdsize < sizeof(segment_command_64)) {
                fail("main image has a truncated segment command");
                return;
            }
            const auto* segment = reinterpret_cast<const segment_command_64*>(command);
            const uint64_t vmEnd = segment->vmaddr + segment->vmsize;
            if (vmEnd < segment->vmaddr) {
                fail("main image segment range overflows");
                return;
            }
            uintptr_t begin = 0;
            uintptr_t end = 0;
            if (!slide_address(slide, segment->vmaddr, begin) ||
                !slide_address(slide, vmEnd, end) || end < begin)
            {
                fail("main image runtime segment range overflows");
                return;
            }
            constexpr vm_prot_t kRx = VM_PROT_READ | VM_PROT_EXECUTE;
            constexpr vm_prot_t kRw = VM_PROT_READ | VM_PROT_WRITE;
            if (segment->initprot == kRx && segment->maxprot == kRx) {
                s_state.executableRanges.push_back({begin, end});
            } else if (segment->initprot == kRw && segment->maxprot == kRw) {
                s_state.writableRanges.push_back({begin, end});
            }
        }
        commandOffset += command->cmdsize;
    }
    if (commandOffset != imageHeader->sizeofcmds || s_state.executableRanges.empty() ||
        s_state.writableRanges.empty())
    {
        fail("main image has no usable executable or writable segments");
        return;
    }

    s_state.failureReason.clear();
    s_state.loaded = true;
#else
    unavailable("prepatch support is not available in this build");
#endif
}

bool available() {
    return s_state.loaded;
}

const char* unavailable_reason() {
    return s_state.failureReason.c_str();
}

std::optional<Site> lookup(void* runtimeTarget) {
    if (!s_state.loaded || runtimeTarget == nullptr) {
        return std::nullopt;
    }
    const auto target = reinterpret_cast<uintptr_t>(runtimeTarget);
    if (containing(s_state.executableRanges, target, sizeof(uint32_t)) == nullptr) {
        return std::nullopt;
    }

    uint32_t entry = 0;
    std::memcpy(&entry, reinterpret_cast<const void*>(target), sizeof(entry));
    uintptr_t gateway = 0;
    if (!branch_target(target, entry, gateway) || gateway < kSiteHeaderSize) {
        return std::nullopt;
    }
    const uintptr_t headerAddress = gateway - kSiteHeaderSize;
    if (containing(s_state.executableRanges, headerAddress, kSiteHeaderSize + kGatewaySize) ==
        nullptr)
    {
        return std::nullopt;
    }

    SiteHeader header{};
    std::memcpy(&header, reinterpret_cast<const void*>(headerAddress), sizeof(header));
    if (std::memcmp(header.magic, kSiteMagic.data(), kSiteMagic.size()) != 0) {
        return std::nullopt;
    }
    uintptr_t recordedTarget = 0;
    uintptr_t slot = 0;
    if (!add_delta(gateway, header.targetDelta, recordedTarget) || recordedTarget != target ||
        !add_delta(gateway, header.slotDelta, slot) || (slot & (alignof(void*) - 1)) != 0 ||
        containing(s_state.writableRanges, slot, sizeof(void*)) == nullptr)
    {
        return std::nullopt;
    }

    uint32_t instructions[7]{};
    std::memcpy(instructions, reinterpret_cast<const void*>(gateway), sizeof(instructions));
    uintptr_t original = 0;
    if (instructions[2] != kLdarX16X16 || instructions[3] != kCbzX16Stub ||
        instructions[4] != kBrX16 || instructions[5] != kBtiC ||
        target > std::numeric_limits<uintptr_t>::max() - 4 ||
        !branch_target(gateway + 24, instructions[6], original) || original != target + 4)
    {
        return std::nullopt;
    }

    return Site{
        reinterpret_cast<void**>(slot), reinterpret_cast<void*>(gateway + kOriginalStubOffset)};
}

void publish(const Site& site, void* trampoline) {
    std::atomic_ref slot{*site.slot};
    slot.store(trampoline, std::memory_order_release);
}

}  // namespace dusk::mods::prepatch
