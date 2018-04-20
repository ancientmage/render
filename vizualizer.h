#ifndef VIZUALIZER_H
#define VIZUALIZER_H

#include <QObject>
#include <vector>
#include <QColor>
#include <QImage>

using namespace std;

class Vizualizer: public QObject
{
    Q_OBJECT
public:
    Vizualizer(QObject * parent = 0);

	virtual void onKeyPressed(long long code);
	virtual QImage onPaint();

    enum Type {
        ALLY = 0,
        LAVA,
        UNDEFINED,
        NONE,
        COUNT
    };

    enum Color {
        DARK_RED = (255 << 16),
        RED = (128 << 16),
        BLACK = 0,
        WHITE = (255 << 16) + (255 << 8) + 255,
        GREEN = (255 << 8)
    };

    vector<vector<Type> > cell_types;

protected:
    virtual vector<vector<QRgb> > const & cell(int x, int y);

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

private:
    void drawCell(int x, int y, int xx, int yy, QImage & q);

    void add_ally();
    void add_enemy();
    void add_lava();
    void add_none();
    void add_undefined();
    void init_types();

    void increase_size();
    void decrease_size();
};

#endif // VIZUALIZER_H

