#pragma once

#include "vizualizer.h"
#include "backend.h"
#include "main_window.h"

class MainWindow;

class Controller : public QObject
{
	Q_OBJECT
public:
    Controller(MainWindow * parent);
	void onKeyPressed(long long code);
	QImage onPaint();

public slots:
    void upd();

private:
	Backend * back;
	Vizualizer * viz;
    MainWindow * p;
};
