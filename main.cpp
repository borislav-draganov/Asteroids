#include "mainwindow.h"
#include <QApplication>
#include "asteroid.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow asteroids;
    asteroids.show();
    return a.exec();
}
