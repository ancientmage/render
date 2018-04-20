#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include "vizualizer.h"

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(Vizualizer * w);

    virtual void onKeyPressed(long long code);

private:
    void redraw();

    Vizualizer * window;
public slots:
    void process();
signals:
    void redrawUI();
};

#endif // BACKEND_H
