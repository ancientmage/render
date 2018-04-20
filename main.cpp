#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <QGraphicsWidget>
#include <QPainter>

#include "main_window.h"
#include "ui_main.h"

using namespace std;

QApplication * app;
MainWindow * window;

int main(int argc, char * argv[]) {
    app = new QApplication(argc, argv);
    window = new MainWindow();
    window->show();

    srand(time(0));

    return app->exec();
}
