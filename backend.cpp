#include "backend.h"
#include "client_sock.h"

Backend::Backend(MainWindow * w) : QObject(0)
{
    window = w;
}

void Backend::process()
{
    clientThread();
}

void Backend::clientThread() {
    Client client("tr.contest.pizza", 10000);
    string s = client.read();
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

        redraw();

        //sss << "MOVE " << -1 << " " << 0;
        client.write(sss.str());
        s = client.read();
        if (s[0] != 'O')
            break;

        client.write("WAIT");
        s = client.read();
        s = client.read();
    }

}

void Backend::redraw()
{
    emit redrawUI();
}
