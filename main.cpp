#include <QPainter>
#include <QGraphicsWidget>

#include <sstream>
#include <chrono>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "main.h"
#include "ui_main.h"
#include "client_sock.h"

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

using namespace std;

int argc;
char ** argv;

void start() {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    app.exec();
}

void clientThread() {
    ClientSock client("127.0.0.1", 8080);
    string s = client.read();
    client.write("hello from client");
    cout << "take from server : " << s << endl;
    client.disconnect();
}

int main(int _argc, char * _argv[]) {
    argc = _argc, argv = _argv;
    std::thread client_thread(clientThread);
    client_thread.detach();
    start();

    cout << "here" << endl;
}
