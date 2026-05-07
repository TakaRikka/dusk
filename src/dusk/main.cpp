#if _WIN32
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>
#endif

#include <aurora/main.h>
#include "dusk/main.h"

#include <algorithm>
#include <array>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#if !defined(_WIN32)
#include <unistd.h>
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif
#endif

int game_main(int argc, char* argv[]);

namespace {

bool RestartProcess(int argc, char* argv[]) {
#if defined(__ANDROID__) || (defined(TARGET_OS_IOS) && TARGET_OS_IOS) ||                           \
    (defined(TARGET_OS_TV) && TARGET_OS_TV)
    (void)argc;
    (void)argv;
    return false;
#elif _WIN32
    std::wstring commandLine = GetCommandLineW();
    STARTUPINFOW startupInfo{};
    startupInfo.cb = sizeof(startupInfo);
    PROCESS_INFORMATION processInfo{};
    if (!CreateProcessW(nullptr, commandLine.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr,
            &startupInfo, &processInfo))
    {
        fprintf(stderr, "Failed to restart Dusk: CreateProcessW error %lu\n", GetLastError());
        return false;
    }

    CloseHandle(processInfo.hThread);
    CloseHandle(processInfo.hProcess);
    return true;
#else
    std::filesystem::path executablePath;

#if defined(__APPLE__)
    uint32_t pathSize = 0;
    _NSGetExecutablePath(nullptr, &pathSize);
    if (pathSize > 0) {
        std::string path(pathSize, '\0');
        if (_NSGetExecutablePath(path.data(), &pathSize) == 0) {
            path.resize(std::strlen(path.c_str()));
            std::error_code ec;
            executablePath = std::filesystem::weakly_canonical(path, ec);
            if (ec) {
                executablePath = path;
            }
        }
    }
#elif defined(__linux__)
    std::array<char, 4096> path{};
    const ssize_t len = readlink("/proc/self/exe", path.data(), path.size() - 1);
    if (len > 0) {
        path[static_cast<size_t>(len)] = '\0';
        executablePath = path.data();
    }
#endif

    if (executablePath.empty() && argc > 0 && argv[0] != nullptr && argv[0][0] != '\0') {
        std::error_code ec;
        executablePath = std::filesystem::absolute(argv[0], ec);
        if (ec) {
            executablePath = argv[0];
        }
    }

    if (executablePath.empty()) {
        fprintf(stderr, "Failed to restart Dusk: unable to resolve executable path\n");
        return false;
    }

    std::vector<std::string> args;
    args.reserve(static_cast<size_t>(std::max(argc, 1)));
    args.push_back(executablePath.string());
    for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i] != nullptr ? argv[i] : "");
    }

    std::vector<char*> execArgv;
    execArgv.reserve(args.size() + 1);
    for (auto& arg : args) {
        execArgv.push_back(arg.data());
    }
    execArgv.push_back(nullptr);

    execv(executablePath.c_str(), execArgv.data());
    fprintf(stderr, "Failed to restart Dusk: execv failed: %s\n", std::strerror(errno));
    return false;
#endif
}

#if _WIN32
bool ShouldShowWindowsConsole(int argc, char* argv[]) {
    if (const auto* env = std::getenv("DUSK_CONSOLE")) {
        if (env[0] != '\0' && env[0] != '0') {
            return true;
        }
    }

    for (int i = 1; i < argc; ++i) {
        if (std::string_view(argv[i]) == "--console") {
            return true;
        }
    }

    return false;
}

void SetupWindowsConsoleStreams() {
    FILE* stream = nullptr;
    freopen_s(&stream, "CONIN$", "r", stdin);
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
}

void WindowsSetupConsole(bool showConsole) {
    if (!showConsole) {
        return;
    }

    if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
        AllocConsole();
    }

    SetupWindowsConsoleStreams();
    SetConsoleOutputCP(CP_UTF8);

    if (const HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        stdoutHandle != INVALID_HANDLE_VALUE && stdoutHandle != nullptr)
    {
        DWORD consoleMode = 0;
        if (GetConsoleMode(stdoutHandle, &consoleMode)) {
            SetConsoleMode(stdoutHandle,
                consoleMode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
}

int DuskMain(int argc, char* argv[]) {
    WindowsSetupConsole(ShouldShowWindowsConsole(argc, argv));
    const int result = game_main(argc, argv);
    if constexpr (dusk::SupportsProcessRestart) {
        if (dusk::RestartRequested) {
            return RestartProcess(argc, argv) ? 0 : result;
        }
    }
    return result;
}

std::vector<std::string> WideArgsToUtf8(int argc, wchar_t** argv) {
    std::vector<std::string> utf8Args;
    utf8Args.reserve(argc);

    for (int i = 0; i < argc; ++i) {
        const int requiredSize =
            WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, nullptr, 0, nullptr, nullptr);
        if (requiredSize <= 0) {
            utf8Args.emplace_back();
            continue;
        }

        std::vector<char> utf8Buffer(static_cast<size_t>(requiredSize));
        WideCharToMultiByte(
            CP_UTF8, 0, argv[i], -1, utf8Buffer.data(), requiredSize, nullptr, nullptr);
        utf8Args.emplace_back(utf8Buffer.data());
    }

    return utf8Args;
}

int RunWindowsGuiEntryPoint() {
    int argc = 0;
    wchar_t** wideArgv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (wideArgv == nullptr) {
        return DuskMain(__argc, __argv);
    }

    std::vector<std::string> utf8Args = WideArgsToUtf8(argc, wideArgv);
    LocalFree(wideArgv);

    std::vector<char*> argv;
    argv.reserve(utf8Args.size());
    for (auto& arg : utf8Args) {
        argv.push_back(arg.data());
    }

    return DuskMain(argc, argv.data());
}
#else
int DuskMain(int argc, char* argv[]) {
    const int result = game_main(argc, argv);
    if (dusk::RestartRequested && RestartProcess(argc, argv)) {
        return 0;
    }
    return result;
}
#endif

}  // namespace

int main(int argc, char* argv[]) {
    return DuskMain(argc, argv);
}

#if _WIN32
int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    return RunWindowsGuiEntryPoint();
}
#endif
