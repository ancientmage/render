#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <QGraphicsWidget>
#include <QPainter>

#include "main.h"
#include "ui_main.h"
#include "client_sock.h"

using namespace std;

QApplication * app;
MainWindow * window;

void client_thread() {
    Client client("tr.contest.pizza", 10000);
    //Sleep(10000);
    string s = client.read();
    //Sleep(10000);
    client.write("team42");

    s = client.read();
    client.write("9e1b5878f4");

    s = client.read();
    int current_pos_x = 15;
    int current_pos_y = 15;

    window->cell_types[current_pos_x][current_pos_y] = MainWindow::ALLY;

    while(true) {
        client.write("SCAN");
        s = client.read();
        if (s[0] != 'O')
            break;

        s = client.read();
        const int n = stoi(s);
        std::cout << "n = " << n << std::endl;
        vector<pair<int, int> > free;
        for (int i = 0; i < n; ++i) {
            //s = client.read();
            string type;
            int x, y;
            type = client.read();
            x = stoi(client.read());
            y = stoi(client.read());
            if (window->cell_types[current_pos_x + x][current_pos_y + y] != MainWindow::UNDEFINED)
                continue;
            //if ((type == "E" || type == "F") && (abs(x) + abs(y) == 1))
            //    free.push_back(make_pair(x, y));
            if (type == "S") {
                window->cell_types[current_pos_x + x][current_pos_y + y] = MainWindow::ALLY;
            } else if (type == "F" || type == "E") {
                window->cell_types[current_pos_x + x][current_pos_y + y] = MainWindow::NONE;
            } else if (type == "W") {
                window->cell_types[current_pos_x + x][current_pos_y + y] = MainWindow::LAVA;
            } else if (isdigit(type[0])) {
                window->cell_types[current_pos_x + x][current_pos_y + y] = MainWindow::NONE;
            }
        }
        window->cell_types[current_pos_x][current_pos_y] = MainWindow::NONE;
        if (window->cell_types[current_pos_x + 1][current_pos_y] != MainWindow::LAVA &&
            window->cell_types[current_pos_x + 1][current_pos_y] != MainWindow::UNDEFINED)
            free.push_back(make_pair(1, 0));

        if (window->cell_types[current_pos_x - 1][current_pos_y] != MainWindow::LAVA &&
            window->cell_types[current_pos_x - 1][current_pos_y] != MainWindow::UNDEFINED)
            free.push_back(make_pair(-1, 0));

        if (window->cell_types[current_pos_x][current_pos_y+1] != MainWindow::LAVA &&
            window->cell_types[current_pos_x][current_pos_y+1] != MainWindow::UNDEFINED)
            free.push_back(make_pair(0, 1));

        if (window->cell_types[current_pos_x][current_pos_y - 1] != MainWindow::LAVA &&
            window->cell_types[current_pos_x][current_pos_y - 1] != MainWindow::UNDEFINED)
            free.push_back(make_pair(0, -1));

        int r = rand() % free.size();
        stringstream sss;
        sss << "MOVE " << free[r].first << " " << free[r].second;

        current_pos_x += free[r].first;
        current_pos_y += free[r].second;

        window->cell_types[current_pos_x][current_pos_y] = MainWindow::ALLY;

        //sss << "MOVE " << -1 << " " << 0;
        client.write(sss.str());
        bool suc = QMetaObject::invokeMethod(window, "keyPressEvent", Qt::QueuedConnection, Q_ARG(QKeyEvent *, nullptr));
        s = client.read();
        if (s[0] != 'O')
            break;

        client.write("WAIT");
        s = client.read();
        s = client.read();
    }

}

int main(int argc, char * argv[]) {
    app = new QApplication(argc, argv);
    window = new MainWindow();
    window->show();

    srand(time(0));

    std::thread client(client_thread);
    client.detach();

    return app->exec();
}
