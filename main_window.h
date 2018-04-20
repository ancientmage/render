#ifndef MAIN_H
#define MAIN_H

#include <QMainWindow>
#include <QPainter>
#include <QApplication>

#include "controller.h"

class Controller;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget * parent = 0);

    ~MainWindow();
    void keyPressEvent(QKeyEvent * e);
    void paintEvent(QPaintEvent *e);

public:
    void upd();

private:
    Ui::MainWindow * ui;
    Controller * c;
};


#endif // MAIN_H
