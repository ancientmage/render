#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

#include "client_sock.h"


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
    //disconnect();
}

int ClientSock::connect(string host, unsigned int port) {
    ClientSock::host = host;
    ClientSock::port = port;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    server = gethostbyname(host.data());
    bcopy((char*) server->h_addr, (char*) &servaddr.sin_addr.s_addr, server->h_length);
    servaddr.sin_port = htons(port);

    if(connected)
        disconnect();

    socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    enable_keepalive(socket);

    for(size_t i = 0; i < 3; i++) { //try to connect 3 times
        if(::connect(socket, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
            cerr << "Error on connecting: " << errno << "  " << strerror(errno) << endl;
        else {
            connected = true;
            return 0;
        }
    }

    connected = false;
    return 1;
}

bool ClientSock::hasError() {
    if(socket == -1)
        return true;

    int error = 0;
    socklen_t len = sizeof(error);
    int retval = getsockopt(socket, SOL_SOCKET, SO_ERROR, &error, &len);

    if(retval != 0 || error != 0)
        return true;
    else
        return false;
}

int ClientSock::enable_keepalive(int sock) {
    int yes = 1;

    if(setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) == -1)
        return -1;

    int idle = 1;

    if(setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(int)) == -1)
        return -1;

    int interval = 1;

    if(setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(int)) == -1)
        return -1;

    int maxpkt = 10;

    if(setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &maxpkt, sizeof(int)) == -1)
        return -1;

    return 0;
}

int ClientSock::disconnect() {
    if(!connected)
        return -1;

    close(socket);
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
    if(!connected)
        return "";

    int result = ::recv(socket, buffer, 1024, 0);
    if (result == -1)
        throw std::runtime_error("read failed");

    return std::string(buffer, result);
}

string ClientSock::readAll() {
    string full = read();

    while(full.find("END") == string::npos)
        full += read();

    full = full.substr(0, full.find("END"));

    return full;
}
