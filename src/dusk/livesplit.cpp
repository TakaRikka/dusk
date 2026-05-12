#if _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    using socket_t = SOCKET;
    static void closeSocket(socket_t s) {
        LINGER li{1, 0};
        setsockopt(s, SOL_SOCKET, SO_LINGER, reinterpret_cast<const char*>(&li), sizeof(li));
        closesocket(s);
    }
    static int socketError(socket_t s) {
        int err = 0; int len = sizeof(err);
        getsockopt(s, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&err), &len);
        return err;
    }
    static constexpr int kSendFlags = 0;
#else
    #include <sys/socket.h>
    #include <sys/select.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    using socket_t = int;
    static void closeSocket(socket_t s) {
        struct linger li{1, 0};
        setsockopt(s, SOL_SOCKET, SO_LINGER, &li, sizeof(li));
        close(s);
    }
    static int socketError(socket_t s) {
        int err = 0; socklen_t len = sizeof(err);
        getsockopt(s, SOL_SOCKET, SO_ERROR, &err, &len);
        return err;
    }
    #ifndef INVALID_SOCKET
        #define INVALID_SOCKET -1
    #endif

    #if defined(__APPLE__)
        static constexpr int kSendFlags = 0;
    #else
        static constexpr int kSendFlags = MSG_NOSIGNAL;
    #endif
#endif

#include <cstdio>
#include "dusk/livesplit.h"
#include "f_op/f_op_overlap_mng.h"

namespace dusk::speedrun {

static bool     running           = false;
static bool     startPending      = false;
static uint64_t frameCount        = 0;
static socket_t sock              = INVALID_SOCKET;
static bool     wasLoading        = false;
static bool     connected         = false;
static bool     connectPending    = false;
static bool     disconnectPending = false;
static uint32_t idleProbeCounter  = 0;
static uint32_t reconnectCounter  = 0;
static char     storedHost[64]    = "127.0.0.1";
static int      storedPort        = 16834;

static void sendCmd(const char* cmd) {
    if (sock == INVALID_SOCKET) {
        return;
    }

    char msg[64];
    const int len = snprintf(msg, sizeof(msg), "%s\r\n", cmd);
    if (len <= 0 || len >= static_cast<int>(sizeof(msg))) {
        return;
    }

    if (send(sock, msg, len, kSendFlags) >= 0) {
        if (!connected) {
            connected = connectPending = true;
        }
        return;
    }

#if _WIN32
    const int err = WSAGetLastError();
    if (err == WSAEWOULDBLOCK || err == WSAENOTCONN) {
        return;
    }
#else
    if (errno == EAGAIN || errno == EWOULDBLOCK || errno == ENOTCONN) {
        return;
    }
#endif

    if (connected) {
        disconnectPending = true;
    }
    closeSocket(sock);
    sock = INVALID_SOCKET;
    connected = connectPending = false;
    reconnectCounter = 0;
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
    startPending = true;
    frameCount = 0;
    wasLoading = false;
}

void reset() {
    running = false;
    startPending = false;
    frameCount = 0;
    wasLoading = false;
    sendCmd("reset");
}

static void reconnect() {
    if (sock != INVALID_SOCKET) {
        closeSocket(sock);
        sock = INVALID_SOCKET;
    }
    connected = connectPending = false;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        return;
    }

#if _WIN32
    u_long nb = 1;
    if (ioctlsocket(sock, FIONBIO, &nb) != 0) {
        closeSocket(sock);
        sock = INVALID_SOCKET;
        return;
    }
#else
    const int fl = fcntl(sock, F_GETFL, 0);
    if (fl < 0 || fcntl(sock, F_SETFL, fl | O_NONBLOCK) < 0) {
        closeSocket(sock);
        sock = INVALID_SOCKET;
        return;
    }
#endif

#if defined(__APPLE__)
    {
        int opt = 1;
        setsockopt(sock, SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof(opt));
    }
#endif

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(storedPort));
    if (inet_pton(AF_INET, storedHost, &addr.sin_addr) != 1) {
        closeSocket(sock);
        sock = INVALID_SOCKET;
        return;
    }

    const int cr = connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
#if _WIN32
    const bool connectPending_ = cr < 0 && WSAGetLastError() == WSAEWOULDBLOCK;
#else
    const bool connectPending_ = cr < 0 && errno == EINPROGRESS;
#endif
    if (cr != 0 && !connectPending_) {
        closeSocket(sock);
        sock = INVALID_SOCKET;
    }
}

void connectLiveSplit(const char* host, int port) {
#if _WIN32
    WSADATA wd{};
    WSAStartup(MAKEWORD(2, 2), &wd);
#endif
    snprintf(storedHost, sizeof(storedHost), "%s", host);
    storedPort = port;
    reconnect();
}

void disconnectLiveSplit() {
    if (sock != INVALID_SOCKET) {
        closeSocket(sock);
        sock = INVALID_SOCKET;
    }
    connected = connectPending = disconnectPending = false;
}

bool consumeConnectedEvent()    { bool v = connectPending;    connectPending    = false; return v; }
bool consumeDisconnectedEvent() { bool v = disconnectPending; disconnectPending = false; return v; }

void updateLiveSplit() {
    if (sock == INVALID_SOCKET) {
        if ((reconnectCounter++ % 30) == 0) {
            reconnect();
        }
        return;
    }

    if (!connected) {
        fd_set writefds, errorfds;
        FD_ZERO(&writefds);
        FD_ZERO(&errorfds);
        FD_SET(sock, &writefds);
        FD_SET(sock, &errorfds);
        timeval tv{0, 0};
#if _WIN32
        const int r = select(0, nullptr, &writefds, &errorfds, &tv);
#else
        const int r = select(sock + 1, nullptr, &writefds, &errorfds, &tv);
#endif
        if (r < 0 || FD_ISSET(sock, &errorfds) || socketError(sock) != 0) {
            closeSocket(sock);
            sock = INVALID_SOCKET;
            reconnectCounter = 0;
            return;
        }
        if (!FD_ISSET(sock, &writefds)) {
            return;
        }
        sendCmd("initgametime");
        return;
    }

    if (startPending) {
        startPending = false;
        sendCmd("initgametime");
        sendCmd("reset");
        sendCmd("starttimer");
    }

    if (!running) {
        if ((idleProbeCounter++ % 60) == 0) {
            char buf;
            const int r = recv(sock, &buf, 1, 0);
            if (r == 0
#if _WIN32
                || (r < 0 && WSAGetLastError() != WSAEWOULDBLOCK)
#else
                || (r < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
#endif
            ) {
                if (connected) {
                    disconnectPending = true;
                }
                closeSocket(sock);
                sock = INVALID_SOCKET;
                connected = connectPending = false;
                reconnectCounter = 0;
            }
        }
        return;
    }

    const uint64_t totalMs  = frameCount * 1000 / 30;
    const uint64_t totalSec = totalMs / 1000;
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "setgametime %u:%02u:%02u.%03u",
        static_cast<uint32_t>(totalSec / 3600),
        static_cast<uint32_t>((totalSec / 60) % 60),
        static_cast<uint32_t>(totalSec % 60),
        static_cast<uint32_t>(totalMs % 1000)
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
