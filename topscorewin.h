#ifndef TOPSCOREWIN_H
#define TOPSCOREWIN_H

#include <QWidget>
#include <QLayout>
#include <QtGui>
#include <QLabel>
#include <QPushButton>

class topScoreWin : public QWidget
{
    Q_OBJECT
public:
    explicit topScoreWin(QWidget *parent = 0);
    void updateLabels();

signals:

public slots:



private:
    QVBoxLayout * topScoreOverlay;
    QPushButton * closeButton;
    QLabel * topScore;

    void createLabels();
    void readTopScores();
};

#endif // TOPSCOREWIN_H
