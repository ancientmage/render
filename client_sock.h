#ifndef CLIENTSOCK_H
#define CLIENTSOCK_H

#include <iostream>
#include <future>
#include <functional>
#include <sstream>

#include <errno.h>
#include <string>
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
    std::stringstream stream;
    Client() = delete;
    Client(const std::string & host, unsigned int port) { socket.connect(host, port); }
    ~Client() { socket.disconnect(); };

    int write(const std::string & buffer) {
        std::cout << "write : " << buffer << std::endl;
        std::cout.flush();
        return socket.write(buffer + "\n");
    }

    std::string readToken() {
        std::string s;
        stream >> s;
        if (s == "")
            return read();
        std::cout << "try to read : \'" << s << "\'" << std::endl;
        return s;
    }

    std::string read() {
        if (stream.str() == "" || stream.eof()) {
            std::string new_buffer = socket.read();
            std::cout << "read : \'" << new_buffer << "\'" << std::endl;
            std::cout.flush();
            stream = stringstream(new_buffer);
        }
        return readToken();
    }
};



#endif // CLIENTSOCK_H
