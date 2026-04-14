#if _WIN32
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>
#endif

#include <aurora/main.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>

int game_main(int argc, char* argv[]);

namespace {

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
        stdoutHandle != INVALID_HANDLE_VALUE && stdoutHandle != nullptr) {
        DWORD consoleMode = 0;
        if (GetConsoleMode(stdoutHandle, &consoleMode)) {
            SetConsoleMode(stdoutHandle,
                           consoleMode | ENABLE_PROCESSED_OUTPUT
                                             | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
}

int DuskMain(int argc, char* argv[]) {
    WindowsSetupConsole(ShouldShowWindowsConsole(argc, argv));
    return game_main(argc, argv);
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
        WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, utf8Buffer.data(), requiredSize, nullptr,
                            nullptr);
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
    return game_main(argc, argv);
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
