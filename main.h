#ifndef MAIN_H
#define MAIN_H

#include <QMainWindow>
#include <QPainter>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    enum Type {
        ALLY,
        ENEMY,
        LAVA,
        NONE
    };

    enum Color {
        DARK_RED = (255 << 16),
        RED = (128 << 16),
        BLACK = 0,
        WHITE = (255 << 16) + (255 << 8) + 255,
        GREEN = (255 << 8)
    };

    /*nt DARK_RED = (255 << 16);
    int RED = (128 << 16);
    int BLACK = 0;
    int WHITE = (255 << 16) + (255 << 8) + 255;
    int GREEN = (255 << 8);*/

    int current_pos_x;
    int current_pos_y;
    int max_width;
    int max_height;
    int size;
    int max_size;
    int min_size;
    int width;
    int height;
    int cell_size;
    vector<vector<vector<QRgb> > > cells;
    vector<vector<Type> > cell_types;

    int type_to_ind(Type type) {
        if (type == ALLY)
            return 0;
        if (type == ENEMY)
            return 1;
        if (type == LAVA)
            return 2;
        if (type == NONE)
            return 3;
    }

    ~MainWindow();
    void keyPressEvent(QKeyEvent * e);
    void paintEvent(QPaintEvent *e);
    void drawCell(int x, int y, int type_ind, QImage & q);

    void increase_size() {
        if (size >= max_size)
            size = max_size;
        else
            size <<= 1;
    }

    void decrease_size() {
        if (size <= min_size)
            size = min_size;
        else
            size >>= 1;
    }

    void add_ally();
    void add_enemy();
    void add_lava();
    void add_none();
    void init_types();

private:
    Ui::MainWindow *ui;
};

#endif // MAIN_H
