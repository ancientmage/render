#ifndef CLIENTSOCK_H
#define CLIENTSOCK_H

#include <iostream>
#include <future>
#include <functional>

#include <errno.h>
#include <string.h>
#include <WinSock2.h>

#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

class ClientSock {
public:
    ClientSock(string host, unsigned int port);
    ClientSock();
    ClientSock(int sock);
    ~ClientSock();

    int connect(string host, unsigned int port);
    int reconnect();
    int disconnect();

    int write(const string & mesg);
    string read();

    string host;
    unsigned int port;
    bool connected;

private:
    int socket;
    struct sockaddr_in servaddr;
    struct hostent* server;
};

class Client {
    ClientSock socket;

public:
    Client() = delete;
    Client(const std::string & host, unsigned int port) { socket.connect(host, port); }
    ~Client() { socket.disconnect(); };

    int write(const std::string & buffer) {
        return socket.write(buffer + "\n");
    }

    std::string read() { return socket.read(); }
};

#endif // CLIENTSOCK_H
