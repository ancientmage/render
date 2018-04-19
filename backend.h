#ifndef BACKEND_H
#define BACKEND_H

#include <QApplication>
#include "main_window.h"

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(MainWindow * w);

    void clientThread();
private:
    void redraw();

    MainWindow * window;
public slots:
    void process();
signals:
    void redrawUI();
};

#endif // BACKEND_H
