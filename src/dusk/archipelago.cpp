// Dusklight Archipelago bridge module.
//
// Native replacement for the GC randomizer REL + dolphin_memory_engine path:
//   1. tiny localhost TCP text server (Dusk = server, AP Python client = client),
//   2. read/write window over the live dSv_info_c (so the apworld client reads
//      location flags / health / name / stage at the same offsets, unchanged),
//   3. each frame drains the item queue the client writes into dSv reserve (off
//      0x8F0) and grants each item via the decomp's execItemGet() (the REL's job),
//   4. a native "safe to give?" gate.
//
// Socket boilerplate mirrors src/dusk/livesplit.cpp. Protocol + offset map: see
// Dusklight-AP/DESIGN.md.

#if _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    using socket_t = SOCKET;
    static void closeSocket(socket_t s) { closesocket(s); }
    static bool wouldBlock() { return WSAGetLastError() == WSAEWOULDBLOCK; }
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    using socket_t = int;
    static const socket_t INVALID_SOCKET = -1;
    static void closeSocket(socket_t s) { close(s); }
    static bool wouldBlock() { return errno == EAGAIN || errno == EWOULDBLOCK; }
#endif

#include <cstdio>
#include <cstring>
#include <string>

#include "dusk/archipelago.h"
#include "d/d_save.h"             // dSv_info_c
#include "d/d_com_inf_game.h"     // dComIfGs_getSaveInfo()
#include "d/d_item.h"             // execItemGet()

namespace dusk::archipelago {

namespace {

constexpr int kItemQueueOff = 0x8F0;  // dSv reserve: 8-slot item queue (client ITEM_WRITE_ADDR)
constexpr int kItemQueueLen = 8;
constexpr int kNodeOff      = 0x978;  // mDan.mStageNo (s8); 0xFF/-1 == not in a stage
constexpr int kNameOff      = 0x1B4;  // mPlayerInfo.mPlayerName
constexpr int kWindowMax    = static_cast<int>(sizeof(dSv_info_c));

socket_t    g_listen   = INVALID_SOCKET;
socket_t    g_client   = INVALID_SOCKET;
int         g_port     = 17354;
bool        g_initDone = false;
std::string g_rx;

inline u8* saveBase() { return reinterpret_cast<u8*>(dComIfGs_getSaveInfo()); }

inline bool inGame() {
    u8* b = saveBase();
    return b != nullptr && static_cast<s8>(b[kNodeOff]) != -1;
}

// Native equivalent of the client's _check_status(): the player actor exists and no
// event/demo/cutscene is running, so execItemGet() never fires mid-cutscene. Both
// accessors route through the always-constructed g_dComIfG_gameInfo.play, so they are
// safe to call even on the title/file-select screen (they return null / 0 there).
bool safeToGive() {
    return dComIfGp_getPlayer(0) != nullptr && !dComIfGp_event_runCheck();
}

// Grant a TP item by id. item ids match the apworld ITEM_TABLE (dItemNo_*), i.e.
// indices into the decomp's item_func table that execItemGet() dispatches.
void grantItem(u8 itemId) {
    if (itemId == 0x00) return;
    execItemGet(itemId);
}

bool windowRead(int off, int len, std::string& outHex) {
    if (off < 0 || len < 0 || off + len > kWindowMax) return false;
    u8* base = saveBase();
    if (!base) return false;
    static const char* hex = "0123456789abcdef";
    outHex.clear();
    outHex.reserve(static_cast<size_t>(len) * 2);
    for (int i = 0; i < len; ++i) {
        u8 b = base[off + i];
        outHex.push_back(hex[b >> 4]);
        outHex.push_back(hex[b & 0xF]);
    }
    return true;
}

int nib(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

bool windowWrite(int off, const char* hex) {
    int len = static_cast<int>(std::strlen(hex) / 2);
    if (off < 0 || off + len > kWindowMax) return false;
    u8* base = saveBase();
    if (!base) return false;
    for (int i = 0; i < len; ++i) {
        int hi = nib(hex[i * 2]), lo = nib(hex[i * 2 + 1]);
        if (hi < 0 || lo < 0) return false;
        base[off + i] = static_cast<u8>((hi << 4) | lo);
    }
    return true;
}

void sendLine(const std::string& s) {
    if (g_client == INVALID_SOCKET) return;
    std::string out = s;
    out.push_back('\n');
    send(g_client, out.data(), static_cast<int>(out.size()), 0);
}

void handleCommand(const std::string& line) {
    if (line.rfind("HELLO", 0) == 0) {
        std::string nameHex;
        windowRead(kNameOff, 16, nameHex);
        char buf[48];
        std::snprintf(buf, sizeof(buf), "OK E %d ", inGame() ? 1 : 0);  // region E = GZ2E01 (US)
        sendLine(std::string(buf) + nameHex);
        return;
    }
    if (line.rfind("READ ", 0) == 0) {
        int off = 0, len = 0;
        std::string hex;
        if (std::sscanf(line.c_str() + 5, "%d %d", &off, &len) == 2 && windowRead(off, len, hex)) {
            sendLine("OK " + hex);
            return;
        }
        sendLine("ERR");
        return;
    }
    if (line.rfind("WRITE ", 0) == 0) {
        int off = 0;
        char hexbuf[2048] = {0};
        if (std::sscanf(line.c_str() + 6, "%d %2047s", &off, hexbuf) == 2 && windowWrite(off, hexbuf)) {
            sendLine("OK");
            return;
        }
        sendLine("ERR");
        return;
    }
    if (line.rfind("SAFE", 0) == 0) {
        sendLine(std::string("OK ") + (safeToGive() ? "1" : "0"));
        return;
    }
    sendLine("ERR");
}

void setNonBlocking(socket_t s) {
#if _WIN32
    u_long nb = 1;
    ioctlsocket(s, FIONBIO, &nb);
#else
    int fl = fcntl(s, F_GETFL, 0);
    fcntl(s, F_SETFL, fl | O_NONBLOCK);
#endif
}

}  // namespace

void init(int port) {
    g_initDone = true;
    g_port = port;
#if _WIN32
    WSADATA wd{};
    WSAStartup(MAKEWORD(2, 2), &wd);
#endif
    g_listen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (g_listen == INVALID_SOCKET) return;
    int yes = 1;
    setsockopt(g_listen, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&yes), sizeof(yes));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(g_port));
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    if (bind(g_listen, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0 ||
        listen(g_listen, 1) != 0) {
        closeSocket(g_listen);
        g_listen = INVALID_SOCKET;
        return;
    }
    setNonBlocking(g_listen);
    std::printf("[AP] listening on 127.0.0.1:%d\n", g_port);
}

void onGameFrame() {
    if (!inGame() || !safeToGive()) return;
    u8* base = saveBase();
    if (!base) return;
    for (int i = 0; i < kItemQueueLen; ++i) {
        u8 id = base[kItemQueueOff + i];
        if (id != 0x00) {
            grantItem(id);
            base[kItemQueueOff + i] = 0x00;  // ack: clear slot (mirrors the REL)
        }
    }
}

void update() {
    if (!g_initDone) init(g_port);  // lazy-init the listener on first frame
    if (g_listen == INVALID_SOCKET) return;

    if (g_client == INVALID_SOCKET) {
        socket_t c = accept(g_listen, nullptr, nullptr);
        if (c != INVALID_SOCKET) {
            setNonBlocking(c);
            g_client = c;
            g_rx.clear();
            std::printf("[AP] client connected\n");
        }
        return;
    }

    char buf[2048];
    int n = recv(g_client, buf, sizeof(buf), 0);
    if (n == 0 || (n < 0 && !wouldBlock())) {
        closeSocket(g_client);
        g_client = INVALID_SOCKET;
        std::printf("[AP] client disconnected\n");
        return;
    }
    if (n > 0) {
        g_rx.append(buf, n);
        size_t nl;
        while ((nl = g_rx.find('\n')) != std::string::npos) {
            std::string ln = g_rx.substr(0, nl);
            if (!ln.empty() && ln.back() == '\r') ln.pop_back();
            g_rx.erase(0, nl + 1);
            if (!ln.empty()) handleCommand(ln);
        }
    }
}

void shutdown() {
    if (g_client != INVALID_SOCKET) { closeSocket(g_client); g_client = INVALID_SOCKET; }
    if (g_listen != INVALID_SOCKET) { closeSocket(g_listen); g_listen = INVALID_SOCKET; }
#if _WIN32
    WSACleanup();
#endif
}

bool isListening()       { return g_listen != INVALID_SOCKET; }
bool isClientConnected() { return g_client != INVALID_SOCKET; }
int  listenPort()        { return g_port; }

}  // namespace dusk::archipelago
