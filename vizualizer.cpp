#include "vizualizer.h"

Vizualizer::Vizualizer(QObject * parent)
    : QObject(parent)
{
    size = 8;
    max_size = 64;
    min_size = 8;
    width = 1280;
    height = 1080;
    max_width = 1000;
    max_height = 1000;
    current_pos_x = 0;
    current_pos_y = 0;

    cell_types.assign(max_width, vector<Type>(max_height, UNDEFINED));
    init_types();
}


void Vizualizer::onKeyPressed(long long code)
{
    if (code == 43 || code == 61)
        increase_size();
    else if (code == 45)
        decrease_size();
    else if (code == 16777234) {
        current_pos_x = max(0, current_pos_x - width / size / 3);
    } else if (code == 16777235) {
        current_pos_y = max(0, current_pos_y - height / size / 3);
    } else if (code == 16777236) {
        current_pos_x = min(max_width - width / size, current_pos_x + width / size / 3);
    } else if (code == 16777237) {
        current_pos_y = min(max_height - height / size, current_pos_y + height / size / 3);
    }
}

QImage Vizualizer::onPaint()
{
	QImage q(width, height, QImage::Format_RGB32);
    q.fill(Qt::white);
    for (int i = 0; i < width / size; ++i) {
        for (int j = 0; j < height / size; ++j) {
            int x = current_pos_x + i;
            int y = current_pos_y + j;

            if (x < 0 || y < 0 || x >= max_width || y >= max_height)
                continue;

            drawCell(i * size, j * size, x, y, q);
        }
    }
    return q;
}

void Vizualizer::drawCell(int x, int y, int cell_x, int cell_y, QImage & q) {
    const vector<vector<QRgb> > & rgbs = cell(cell_x, cell_y);
    int cnt = size / min_size;
    for (int k = 0; k < size; ++k) {
        for (int l = 0; l < size; ++l) {
            q.setPixel(x + k, y + l, rgbs[k / cnt][l / cnt]);
        }
    }
}

vector<vector<QRgb>> const & Vizualizer::cell(int x, int y) {
	return cells[cell_types[x][y]];
}

void Vizualizer::init_types() {
    add_ally();
    add_lava();
    add_undefined();
    add_none();
}

void Vizualizer::add_ally() {
    vector<vector<QRgb> > rgbs(size, vector<QRgb> (size, GREEN));
    rgbs[2][2] = rgbs[1][3] = rgbs[5][2] = rgbs[6][3] = BLACK;
    rgbs[3][6] = rgbs[4][6] = BLACK;
    cells.push_back(rgbs);
}

void Vizualizer::add_lava() {
    vector<vector<QRgb> > rgbs(size, vector<QRgb> (size, RED));
    cells.push_back(rgbs);
}

void Vizualizer::add_none() {
    vector<vector<QRgb> > rgbs(size, vector<QRgb> (size, WHITE));
    cells.push_back(rgbs);
}

void Vizualizer::add_undefined() {
    vector<vector<QRgb> > rgbs(size, vector<QRgb> (size, BLACK));
    cells.push_back(rgbs);
}


void Vizualizer::increase_size() {
    if (size >= max_size)
        size = max_size;
    else
        size <<= 1;
}

void Vizualizer::decrease_size() {
    if (size <= min_size)
        size = min_size;
    else
        size >>= 1;
}
