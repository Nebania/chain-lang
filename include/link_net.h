#pragma once
#include <string>

namespace SysNet {
    void init();
    void cleanup();
    int createSocket();
    bool connectSocket(int sock, const std::string& ip, int port);
    bool bindAndListen(int sock, int port);
    int acceptClient(int serverSock); 
    bool sendData(int sock, const std::string& data);
    std::string receiveData(int sock, int bufferSize = 4096);
    void closeSocket(int sock);
}
