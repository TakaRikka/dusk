#if _WIN32
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#endif

int game_main(int argc, char* argv[]);

void WindowsSetupConsole();

int main(int argc, char* argv[]) {
    WindowsSetupConsole();
    return game_main(argc, argv);
}

void WindowsSetupConsole() {
#if _WIN32
    SetConsoleOutputCP(CP_UTF8);

    auto stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(stdout, &consoleMode);
    SetConsoleMode(stdout, consoleMode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}