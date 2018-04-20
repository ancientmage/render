#include "backend.h"

#include <iostream>

#include <boost/asio.hpp>

Backend::Backend(Vizualizer * w) : QObject(0)
{
    window = w;
}

void Backend::process()
{
    boost::asio::ip::tcp::iostream connection;

    //std::cout << "Connecting to " << host << ":" << port << "..." << std::endl;
    connection.connect("tr.contest.pizza", "10000");

    boost::asio::ip::tcp::no_delay opt(true);
    connection.rdbuf()->set_option(opt);

    string s;
    connection >> s;
    connection << "team42" << endl;

    connection >> s;
    connection << "9e1b5878f4" << endl;

    connection >> s;
    int current_pos_x = 15;
    int current_pos_y = 15;

    window->cell_types[current_pos_x][current_pos_y] = Vizualizer::Type::ALLY;

    while(true) {
        connection << "SCAN" << endl;
        connection >> s;
        if (s[0] != 'O')
            break;

         connection >> s;
        const int n = stoi(s);
        vector<pair<int, int> > free;
        for (int i = 0; i < n; ++i) {
            //s = client.read();
            string type;
            int x, y;
            connection >> type;
            connection >> x >> y;
            if (window->cell_types[current_pos_x + x][current_pos_y + y] != Vizualizer::Type::UNDEFINED)
                continue;
            //if ((type == "E" || type == "F") && (abs(x) + abs(y) == 1))
            //    free.push_back(make_pair(x, y));
            if (type == "S") {
                window->cell_types[current_pos_x + x][current_pos_y + y] = Vizualizer::Type::ALLY;
            } else if (type == "F" || type == "E") {
                window->cell_types[current_pos_x + x][current_pos_y + y] = Vizualizer::Type::NONE;
            } else if (type == "W") {
                window->cell_types[current_pos_x + x][current_pos_y + y] = Vizualizer::Type::LAVA;
            } else if (isdigit(type[0])) {
                window->cell_types[current_pos_x + x][current_pos_y + y] = Vizualizer::Type::NONE;
            }
        }
        window->cell_types[current_pos_x][current_pos_y] = Vizualizer::Type::NONE;
        if (window->cell_types[current_pos_x + 1][current_pos_y] != Vizualizer::Type::LAVA &&
            window->cell_types[current_pos_x + 1][current_pos_y] != Vizualizer::Type::UNDEFINED)
            free.push_back(make_pair(1, 0));

        if (window->cell_types[current_pos_x - 1][current_pos_y] != Vizualizer::Type::LAVA &&
            window->cell_types[current_pos_x - 1][current_pos_y] != Vizualizer::Type::UNDEFINED)
            free.push_back(make_pair(-1, 0));

        if (window->cell_types[current_pos_x][current_pos_y+1] != Vizualizer::Type::LAVA &&
            window->cell_types[current_pos_x][current_pos_y+1] != Vizualizer::Type::UNDEFINED)
            free.push_back(make_pair(0, 1));

        if (window->cell_types[current_pos_x][current_pos_y - 1] != Vizualizer::Type::LAVA &&
            window->cell_types[current_pos_x][current_pos_y - 1] != Vizualizer::Type::UNDEFINED)
            free.push_back(make_pair(0, -1));

        int r = rand() % free.size();
        stringstream sss;
        sss << "MOVE " << free[r].first << " " << free[r].second;

        current_pos_x += free[r].first;
        current_pos_y += free[r].second;

        window->cell_types[current_pos_x][current_pos_y] = Vizualizer::Type::ALLY;

        redraw();

        //sss << "MOVE " << -1 << " " << 0;
        connection << sss.str() << endl;
        connection >> s;
        if (s[0] != 'O')
            break;

        connection << "WAIT" << endl;
        connection >> s;
        connection >> s;
    }

}

void Backend::onKeyPressed(long long code)
{
}

void Backend::redraw()
{
    emit redrawUI();
}
