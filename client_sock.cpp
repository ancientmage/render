#include <cstring>
#include <ws2tcpip.h>

#include "client_sock.h"

#pragma comment(lib, "ws2_32.lib")
#pragma warning( disable : 4996)

ClientSock::ClientSock(string host, unsigned int port) {
    connect(host, port);
}

ClientSock::ClientSock() {
    connected = false;
}

ClientSock::ClientSock(int sock) {
    socket = sock;
    connected = true;
}

ClientSock::~ClientSock() {
    disconnect();
    WSACleanup();
}

int ClientSock::connect(string host, unsigned int port) {
    auto wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    WSAStartup(wVersionRequested, &wsaData);
    ClientSock::host = host;
    ClientSock::port = port;

    socket = -1;
    return reconnect();
    /*socket = ::socket(AF_INET, SOCK_STREAM, 0);

    if (socket == -1) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        reconnect();
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    inet_pton(AF_INET, host.c_str(), &servaddr.sin_addr);

    for(int i = 0; i < 3; ++i) { //try to connect 3 times
        if (::connect(socket, (struct sockaddr*) &servaddr, sizeof(servaddr)) >= 0) {
            connected = true;
            return 0;
        }
    }
    if (!connected) {
        std::cerr << "Cant connect to server\nTry to reconnect" << std::endl;
        connect(host, port);
    }
    connected = false;
    return 1;*/
}

int ClientSock::reconnect() {
    if (connected)
        return 0;

    if (socket == -1)
        socket = ::socket(AF_INET, SOCK_STREAM, 0);

    if (socket == -1) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        Sleep(1000);
        return reconnect();
    }

    servaddr = { 0 };
    //memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    inet_pton(AF_INET, host.c_str(), &servaddr.sin_addr);

    if (::connect(socket, (struct sockaddr*) &servaddr, sizeof(servaddr)) >= 0) {
        connected = true;
        return 0;
    }

    if (!connected) {
        std::clog << "Cant connect to server\nTry to reconnect" << std::endl;
        Sleep(1000);
        return reconnect();
    }
    
    connected = true;
    return 0;
}

int ClientSock::disconnect() {
    if(!connected)
        return -1;

    closesocket(socket);
    connected = false;

    return 0;
}

int ClientSock::write(const string& buffer) {
    if(!connected)
        return 1;

    if (socket == -1)
        throw std::runtime_error("cant write buffer because of socket isnt initialized");

    int result = send(socket, buffer.c_str(), buffer.size() + 1, 0);

    if (result == -1)
        throw std::runtime_error("send failed");
    return 0;
}

string ClientSock::read() {
    if (!connected)
        return "";
    const unsigned int buffSize = 1000;

    char buffer[buffSize];
    int result = ::recv(socket, buffer, 1024, 0);
    if (result == -1)
        throw std::runtime_error("read failed");

    return std::string(buffer, result);
}
