#include "main_window.h"
#include "ui_main.h"

#include <QPainter>
#include <QThread>
#include <iostream>
#include <QGraphicsWidget>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    c = new Controller(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete c;
}

void MainWindow::paintEvent(QPaintEvent * e) {
    QPainter painter(this);
    painter.drawImage(0, 0, c->onPaint());
}

void MainWindow::keyPressEvent(QKeyEvent * e) {
    
    if (e == nullptr) {
        return;
    }
    if (e->key() == 81) {
        this->close();
        return;
    }

    c->onKeyPressed(e->key());
    upd();
}

void MainWindow::upd()
{
    this->repaint();
}
