#include "main.h"
#include "ui_main.h"
#include <QPainter>
#include <iostream>
#include <QGraphicsWidget>
#include <unistd.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    size = 8;
    max_size = 64;
    min_size = 8;
    width = 1280;
    height = 1080;
    max_width = 1920;
    max_height = 1080;
    current_pos_x = 0;
    current_pos_y = 0;
    cell_types.assign(1920, vector<Type>(1080, NONE));

    cell_types[5][5] = ALLY;
    cell_types[10][10] = ALLY;
    cell_types[15][15] = ENEMY;
    cell_types[10][15] = LAVA;
    cell_types[15][10] = LAVA;
    init_types();
}

void MainWindow::init_types() {
    add_ally();
    add_enemy();
    add_lava();
    add_none();
}

void MainWindow::add_enemy() {
    vector<vector<QRgb> > rgbs(size, vector<QRgb> (size, DARK_RED));
    rgbs[1][2] = rgbs[2][3] = rgbs[6][2] = rgbs[5][3] = BLACK;
    rgbs[3][6] = rgbs[4][6] = Qt::black;
    cells.push_back(rgbs);
}

void MainWindow::add_ally() {
    vector<vector<QRgb> > rgbs(size, vector<QRgb> (size, GREEN));
    rgbs[2][2] = rgbs[1][3] = rgbs[5][2] = rgbs[6][3] = BLACK;
    rgbs[3][6] = rgbs[4][6] = Qt::black;
    cells.push_back(rgbs);
}

void MainWindow::add_lava() {
    vector<vector<QRgb> > rgbs(size, vector<QRgb> (size, RED));
    cells.push_back(rgbs);
}

void MainWindow::add_none() {
    vector<vector<QRgb> > rgbs(size, vector<QRgb> (size, WHITE));
    cells.push_back(rgbs);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawCell(int x, int y, int type_ind, QImage & q) {
    const vector<vector<QRgb> > & rgbs = cells[type_ind];
    int cnt = size / min_size;
    for (int k = 0; k < size; ++k) {
        for (int l = 0; l < size; ++l) {
            q.setPixel(x + k, y + l, rgbs[k / cnt][l / cnt]);
        }
    }
}

void MainWindow::paintEvent(QPaintEvent * e) {
    cout << "key" << endl;
    QPainter painter(this);
    QImage q(width, height, QImage::Format_RGB32);
    q.fill(Qt::white);
    for (int i = 0; i < width / size; ++i) {
        for (int j = 0; j < height / size; ++j) {
            int x = current_pos_x + i;
            int y = current_pos_y + j;

            if (x < 0 || y < 0 || x >= max_width || y >= max_height)
                continue;

            int type_ind = type_to_ind(cell_types[x][y]);

            drawCell(i * size, j * size, type_ind, q);
        }
    }
    painter.drawImage(0, 0, q);
}

void MainWindow::keyPressEvent(QKeyEvent * e) {
    cout << e->key() << endl;
    this->update();
    if (e->key() == 81) {
        this->close();
        return;
    }

    if (e->key() == 43 || e->key() == 61)
        increase_size();
    else if (e->key() == 45)
        decrease_size();
    else if (e->key() == 16777234) {
        current_pos_x = max(0, current_pos_x - width / size / 3);
    } else if (e->key() == 16777235) {
        current_pos_y = max(0, current_pos_y - height / size / 3);
    } else if (e->key() == 16777236) {
        current_pos_x = min(max_width - width / size, current_pos_x + width / size / 3);
    } else if (e->key() == 16777237) {
        current_pos_y = min(max_height - height / size, current_pos_y + height / size / 3);
    }
    /*
     * TODO left right up down buttons
     */
}



int main(int argc, char * argv[]) {
    /*const char * s = "asdv";
    char * const ss = "asdv";
    char * p = "123";
    s = p;
    ss = "123";*/

    srand(time(0));
    QApplication app(argc, argv);
    MainWindow window;

    window.show();

    return app.exec();
}
