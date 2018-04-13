#include <QPainter>
#include <QGraphicsWidget>

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "main.h"
#include "ui_main.h"
#include "client_sock.h"


using namespace std;

int argc;
char ** argv;

void start(int argc, char ** argv) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    app.exec();
}

void drawWindow() {
    start(argc, argv);
}

int main(int _argc, char * _argv[]) {
    argc = _argc, argv = _argv;
    std::thread drawThread(drawWindow);
    drawThread.detach();
    cout << "here" << endl;
    ClientSock client("192.168.1.1", 5080);
    cout << "here2" << endl;

}
