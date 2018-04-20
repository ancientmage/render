#include "controller.h"

#include <QMainWindow>
#include <QPainter>
#include <QApplication>
#include <QThread>

#include <iostream>

Controller::Controller(MainWindow * parent)
	: QObject(0)
{
	p = parent;
    viz = new Vizualizer();

	QThread * thread = new QThread(this);
    Backend * updater = new Backend(viz);
    updater->moveToThread(thread);
    connect(updater, SIGNAL(redrawUI()), this, SLOT(upd()));
    connect(thread, SIGNAL(destroyed()), updater, SLOT(deleteLater()));
    connect(thread, SIGNAL (started()), updater, SLOT (process()));

    thread->start();

}

void Controller::onKeyPressed(long long code)
{
	back->onKeyPressed(code);
	viz->onKeyPressed(code);
}

void Controller::upd()
{
	p->upd();
}

QImage Controller::onPaint()
{
	return viz->onPaint();
}
