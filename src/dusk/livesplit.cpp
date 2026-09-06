#include "dusk/livesplit.h"

#include "borealis/net.hpp"

#include "f_op/f_op_overlap_mng.h"

#include <cstdio>
#include <memory>
#include <span>
#include <string>

namespace dusk::speedrun {
namespace {

bool running = false;
bool startPending = false;
uint64_t frameCount = 0;
bool wasLoading = false;
bool connected = false;
bool connectPending = false;
bool disconnectPending = false;
uint32_t reconnectCounter = 0;
std::string storedEndpoint = "tcp://127.0.0.1:16834";
std::unique_ptr<borealis::net::Context> netContext;
borealis::net::SocketId socketId = 0;

void send_cmd(const char* command) {
    if (!netContext || !connected || socketId == 0) {
        return;
    }

    char message[64];
    const int length = snprintf(message, sizeof(message), "%s\r\n", command);
    if (length <= 0 || length >= static_cast<int>(sizeof(message))) {
        return;
    }

    const auto chars = std::span<const char>{message, static_cast<size_t>(length)};
    netContext->send(socketId, std::as_bytes(chars));
}

void reconnect() {
    netContext.reset();
    netContext = std::make_unique<borealis::net::Context>();
    connected = false;
    connectPending = false;
    socketId = netContext->connect(storedEndpoint);
}

void poll_network() {
    if (!netContext) {
        return;
    }

    borealis::net::Event event;
    while (netContext->poll(event)) {
        if (event.id != socketId) {
            continue;
        }
        if (event.kind == borealis::net::Event::Kind::Connected) {
            connected = true;
            connectPending = true;
            send_cmd("initgametime");
        } else if (event.kind == borealis::net::Event::Kind::Closed) {
            if (connected) {
                disconnectPending = true;
            }
            connected = false;
            connectPending = false;
            socketId = 0;
            reconnectCounter = 0;
        }
    }
}

}  // namespace

uint64_t getFrameCount() {
    return frameCount;
}

void onGameFrame() {
    if (!running) {
        return;
    }

    const bool loading = fopOvlpM_IsDoingReq() != 0;
    if (loading != wasLoading) {
        send_cmd(loading ? "pausegametime" : "unpausegametime");
        wasLoading = loading;
    }

    if (!loading) {
        ++frameCount;
    }
}

void start() {
    if (g_speedrunInfo.m_isRunStarted || running) {
        return;
    }
    resetForSpeedrunMode();
    g_speedrunInfo.startRun();

    running = true;
    startPending = true;
    frameCount = 0;
    wasLoading = false;
}

void reset() {
    running = false;
    startPending = false;
    frameCount = 0;
    wasLoading = false;
    send_cmd("reset");
}

void connectLiveSplit(const char* host, int port) {
    std::string endpointHost = host;
    if (endpointHost.find(':') != std::string::npos &&
        !(endpointHost.starts_with('[') && endpointHost.ends_with(']')))
    {
        endpointHost = '[' + endpointHost + ']';
    }
    storedEndpoint = "tcp://" + endpointHost + ':' + std::to_string(port);
    reconnect();
}

void disconnectLiveSplit() {
    netContext.reset();
    socketId = 0;
    connected = false;
    connectPending = false;
    disconnectPending = false;
}

bool consumeConnectedEvent() {
    const bool value = connectPending;
    connectPending = false;
    return value;
}

bool consumeDisconnectedEvent() {
    const bool value = disconnectPending;
    disconnectPending = false;
    return value;
}

void updateLiveSplit() {
    poll_network();
    if (socketId == 0) {
        if ((reconnectCounter++ % 30) == 0) {
            reconnect();
        }
        return;
    }
    if (!connected) {
        return;
    }

    if (startPending) {
        startPending = false;
        send_cmd("initgametime");
        send_cmd("reset");
        send_cmd("starttimer");
    }

    if (!running) {
        return;
    }

    const uint64_t totalMs = frameCount * 1000 / 30;
    const uint64_t totalSec = totalMs / 1000;
    char command[32];
    snprintf(command, sizeof(command), "setgametime %u:%02u:%02u.%03u",
        static_cast<uint32_t>(totalSec / 3600), static_cast<uint32_t>((totalSec / 60) % 60),
        static_cast<uint32_t>(totalSec % 60), static_cast<uint32_t>(totalMs % 1000));
    send_cmd(command);
}

void shutdown() {
    disconnectLiveSplit();
}

}  // namespace dusk::speedrun
