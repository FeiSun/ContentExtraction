﻿#include <QtGui/QApplication>
#include "window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;

    window.show();

    return app.exec();
}
