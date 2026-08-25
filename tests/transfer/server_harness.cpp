#include "dusk/transfer/server.hpp"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::fprintf(stderr, "usage: server_harness <discs-dir> <port>\n");
        return 2;
    }
    dusk::transfer::ServerConfig config;
    config.discsDir = argv[1];
    config.portFirst = static_cast<std::uint16_t>(std::atoi(argv[2]));
    config.portLast = config.portFirst;

    dusk::transfer::Server server{config, [](const std::filesystem::path&) {
        return dusk::transfer::ValidateOutcome{true, false, "accepted"};
    }};
    if (!server.start()) {
        std::fprintf(stderr, "failed to bind\n");
        return 1;
    }
    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
