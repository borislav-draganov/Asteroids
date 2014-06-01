#include <QWidget>
#include <QtGui>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QFile>
#include "topscorewin.h"

#define scWidth 300
#define scHeight 440
#define maxTopScores 10


QLabel * scoreLabels[maxTopScores];

struct topScoreStruct
{
    QString name;
    int score;
};

topScoreStruct topScores[maxTopScores];

topScoreWin::topScoreWin(QWidget *parent) :
    QWidget(parent)
{

    // remove borders, close and minimize buttons
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowStaysOnTopHint);
    this->setFixedSize(scWidth,scHeight);

    // Create vertical box layout that will hold labes
    topScoreOverlay = new QVBoxLayout(this);

    // Read the current top scores
    readTopScores();

    // Set the standart font we use
    QFont smallFont( "helvetica", 14 );

    // Add Top Scores label
    topScore = new QLabel( tr("TOP SCORES"), this );
    topScore->setFont( smallFont );
    topScore->setAlignment(Qt::AlignCenter);
    topScoreOverlay->addWidget( topScore );

    createLabels();

    // Add close button
    closeButton = new QPushButton(tr("&Close"));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    topScoreOverlay->addWidget(closeButton);
}

void topScoreWin::createLabels()
{
    // Set the standart font we use
    QFont smallFont( "helvetica", 14 );

    // Add labels with scores
    for(int i = 0; i< maxTopScores; i++)
    {
        QString tStr = topScores[i].name + "      " + QString::number(topScores[i].score);
        scoreLabels[i] = new QLabel( this );
        scoreLabels[i]->setText(tStr);
        scoreLabels[i]->setFont( smallFont );
        topScoreOverlay->addWidget( scoreLabels[i] );
    }

}


void topScoreWin::updateLabels()
{
    readTopScores();

    for(int i = 0; i< maxTopScores; i++)
    {
        QString tStr = topScores[i].name + "      " + QString::number(topScores[i].score);
        scoreLabels[i]->setText(tStr);
    }
}

void topScoreWin::readTopScores()
{
    // Set which file we will open
    QFile file("topscore.txt");

    // Try to open the file for reading
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             return;

    // Reading the file:
    // Either read until end or read until 10 rows
    int counter = 0;
    while (!file.atEnd() and counter < maxTopScores) {
        // Read line
        QString line = file.readLine();

        // Process line
        // We need to have Name (w/o spaces) and Scores on this line. They must be delimited by space

        // Setting delimiter - will be empty space
        QString delimiterPattern(" ");

        // Splitting the string
        QStringList splitLine = line.split(delimiterPattern);

        topScores[counter].name = splitLine[0]; // We will take the first element from the line we split and put as name
        topScores[counter].score = splitLine[1].toInt(); // We will take the second element from the line we split and put as name

        // Raise the counter
        ++counter;
    }

    // We might not nave top scores yet or the list might not be full -> Do a check and set the topScores as empty
    if(counter < maxTopScores-1)
    {
        for(int i = counter; i < maxTopScores; i++)
        {
            topScores[i].name = "EmptyEmptyEmp"; // Set as empty
            topScores[i].score = 99999999; // Set as 0
        }
    }

    file.close();
}
