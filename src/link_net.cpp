#include "link_net.h"
#include <iostream>
#include <vector>
#include <cstring>

// Platform specific includes
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib") 
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <netdb.h>
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

namespace SysNet {

    void init() {
        #ifdef _WIN32
            WSADATA wsaData;
            WSAStartup(MAKEWORD(2, 2), &wsaData);
        #endif
    }

    void cleanup() {
        #ifdef _WIN32
            WSACleanup();
        #endif
    }

    int createSocket() {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) return -1;
        
        // [OPTIONAL] Set SO_REUSEADDR agar port bisa langsung dipakai ulang setelah stop
        int opt = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
        
        return sock;
    }

    bool connectSocket(int sock, const std::string& ip, int port) {
        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &server.sin_addr) <= 0) return false;
        if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) return false;
        return true;
    }

    bool bindAndListen(int sock, int port) {
        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY; 
        server.sin_port = htons(port);

        if (bind(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
            return false; 
        }
        if (listen(sock, 5) < 0) {
            return false;
        }
        return true;
    }

    int acceptClient(int serverSock) {
        struct sockaddr_in client;
        socklen_t c = sizeof(struct sockaddr_in);
        
        int clientSock = accept(serverSock, (struct sockaddr*)&client, &c);
        
        if (clientSock == INVALID_SOCKET) return -1;
        return clientSock;
    }

    bool sendData(int sock, const std::string& data) {
        if (send(sock, data.c_str(), data.size(), 0) == SOCKET_ERROR) return false;
        return true;
    }

    std::string receiveData(int sock, int bufferSize) {
        std::vector<char> buffer(bufferSize);
        int bytesReceived = recv(sock, buffer.data(), bufferSize, 0);
        if (bytesReceived > 0) {
            return std::string(buffer.data(), bytesReceived);
        }
        return "";
    }

    void closeSocket(int sock) {
        closesocket(sock);
    }
}
