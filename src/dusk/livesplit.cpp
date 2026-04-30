#if _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    using socket_t = SOCKET;
    static void closeSocket(socket_t s) { closesocket(s); }
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    using socket_t = int;
    static void closeSocket(socket_t s) { close(s); }
    #ifndef INVALID_SOCKET
        #define INVALID_SOCKET -1
    #endif
#endif

#include <cstdio>
#include "dusk/livesplit.h"
#include "f_op/f_op_overlap_mng.h"

namespace dusk::speedrun {

static bool     running           = false;
static uint64_t frameCount        = 0;
static socket_t sock              = INVALID_SOCKET;
static bool     wasLoading        = false;
static bool     connected         = false;
static bool     connectPending    = false;
static bool     disconnectPending = false;

static void sendCmd(const char* cmd) {
    if (sock == INVALID_SOCKET) {
        return;
    }

    char msg[64];
    int len = snprintf(msg, sizeof(msg), "%s\r\n", cmd);

    if (send(sock, msg, len, 0) >= 0) {
        if (!connected) {
            connected = connectPending = true;
        }

        return;
    }

#if _WIN32
    int err = WSAGetLastError();
    if (err == WSAEWOULDBLOCK || err == WSAENOTCONN) {
        return;
    }
#else
    if (errno == EAGAIN || errno == EWOULDBLOCK || errno == ENOTCONN) {
        return;
    }
#endif

    if (connected) disconnectPending = true;
    closeSocket(sock);
    sock = INVALID_SOCKET;
    connected = connectPending = false;
}

uint64_t getFrameCount() {
    return frameCount;
}

void onGameFrame() {
    if (!running) {
        return;
    }

    bool loading = fopOvlpM_IsDoingReq() != 0;

    if (loading != wasLoading) {
        sendCmd(loading ? "pausegametime" : "unpausegametime");
        wasLoading = loading;
    }

    if (!loading) {
        ++frameCount;
    }
}

void start() {
    if (running) {
        return;
    }
    
    running = true;
    frameCount = 0;
    wasLoading = false;
    sendCmd("initgametime");
    sendCmd("reset");
    sendCmd("starttimer");
}

void reset() {
    running = false;
    frameCount = 0;
    wasLoading = false;
    sendCmd("reset");
}

void connectLiveSplit(const char* host, int port) {
#if _WIN32
    WSADATA wd{}; WSAStartup(MAKEWORD(2, 2), &wd);
#endif

    if (sock != INVALID_SOCKET) {
        closeSocket(sock); sock = INVALID_SOCKET;
    }

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock == INVALID_SOCKET) {
        return;
    }

#if _WIN32
    u_long nb = 1;
    ioctlsocket(sock, FIONBIO, &nb);
#else
    fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);
#endif

    sockaddr_in addr{}; addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);
    inet_pton(AF_INET, host, &addr.sin_addr);
    connect(sock, (sockaddr*)&addr, sizeof(addr));
    sendCmd("initgametime");
}

void disconnectLiveSplit() {
    if (sock != INVALID_SOCKET) {
        closeSocket(sock);
        sock = INVALID_SOCKET;
        connected = false;
    }
}

bool consumeConnectedEvent()    { bool v = connectPending;    connectPending    = false; return v; }
bool consumeDisconnectedEvent() { bool v = disconnectPending; disconnectPending = false; return v; }

void updateLiveSplit() {
    if (sock == INVALID_SOCKET) {
        return;
    }

    if (!connected) {
        sendCmd("initgametime");
        return;
    }

    if (!running) {
        return;
    }

    const uint64_t totalMs  = frameCount * 1000 / 30;
    const uint64_t totalSec = totalMs / 1000;
    char cmd[32];
    
    snprintf(cmd, sizeof(cmd), "setgametime %u:%02u:%02u.%03u",
        (uint32_t)(totalSec / 3600),
        (uint32_t)((totalSec / 60) % 60),
        (uint32_t)(totalSec % 60),
        (uint32_t)(totalMs % 1000)
    );

    sendCmd(cmd);
}

void shutdown() {
    disconnectLiveSplit();
#if _WIN32
    WSACleanup();
#endif
}

}
