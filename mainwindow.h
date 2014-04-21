#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ship.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QmdiArea>
#include <QLayout>
#include <QPoint>

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static int randInt(int , int );

private slots:
     void newGame();
     void loadGame();
     void saveGame();

private:
    Ui::MainWindow *ui;
    void createActions();
    void createMenus();
    QMenu *GameMenu;
    QAction *newGameAct;
    QAction *loadGameAct;
    QAction *saveGameAct;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QWidget *border;
    QVBoxLayout *borderLayout;
    QHBoxLayout *horizontalBox;
    QLabel *label;
};

#endif // MAINWINDOW_H
