#include "mainwindow.h"
#include "asteroid.h"
#include "saucer.h"
#include "ship.h"


#include <QMenuBar>
#include <QApplication>
#include <QtGui>
#include <QPointer>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplashScreen>
#include <QXmlStreamReader>

// Define main window size
#define mwWidth 800
#define mwHeight 600

// Define horizontal bar size
#define hbWidth 780
#define hbHeight 40

// Define scene size
#define scWidth 750
#define scHeight 500

// Set standard button size
#define butWidth 200
#define butHeight 40
// Setting standard buttons offset to their height + 10 so they do not touch each other
#define standardButtonsOffset butHeight + 10


// Set save / load buttons size
#define btnSLWidth 300
#define btnSLHeight 40

// Set first save / load buttons vertical position
#define btnSLVPos 120
// Setting load / save buttons offset
#define btnSaveLoadOffset 38


// Define max levels
#define maxLevels   10

// Define maxSaves - using 11 instead of 10 for easier relation between saves (1 to 1 instead of 0 to 1)
#define maxSaves    11

// Set the number of top scores
#define maxTopScores 10

// Define structure that will be used when creating new level to identify the number of asteroids and saucers to be spawned
struct levelDef
{
    int levelNum;
    int asteroidsNum;
    int saucerNum;
};

// Define the array used to spawn the saucers and asteroids

levelDef levelDescr[maxLevels] = {
    { 1,  1, 0},
    { 2,  2, 1},
    { 3,  3, 1},
    { 4,  4, 2},
    { 5,  5, 2},
    { 6,  6, 2},
    { 7,  7, 2},
    { 8, 10, 3},
    { 9, 11, 3},
    {10, 12, 3}
};

// Define structure that will be used to parse the saves.xml

struct parsedXML
{
    QString SSavename;
    int SScore;
    int SLives;
    int SLevel;
};

// Define array that will hold the saves after we read them

parsedXML xmlGames[maxSaves];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    // Set main window fixed size
    this->setFixedSize(mwWidth,mwHeight);

    // Create widget to use for CentralWidget in the window
    border = new QWidget( this );
    setCentralWidget( border );


    // Create Vertical Box Layout where we will place the scene and the labels
    borderLayout = new QVBoxLayout( border );
    borderLayout->addStretch( 1 );

    // Create Horizontal Boz layout where we will place the labels for score, level, etc
    topWin = new QWidget( border );
    topWin->setFixedSize(hbWidth, hbHeight);
    horizontalBox = new QHBoxLayout ( topWin );

    // set initial lives, scores and level
    score = 0;
    lives = 2;
    level = 1;


    // Add labels for score, lives and level
    // Set font
    QFont smallFont( "helvetica", 14 );

    labelScore = new QLabel( tr("SCORE"), topWin );
    labelScore->setFont( smallFont );
    labelScore->setAlignment(Qt::AlignLeft);
    horizontalBox->addWidget( labelScore );

    labelCurScore = new QLabel(topWin);
    labelCurScore -> setText(QString::number(score));
    labelCurScore ->setFont( smallFont );
    labelCurScore -> setAlignment(Qt::AlignRight);
    horizontalBox->addWidget( labelCurScore );


    labelEmptyPos1 = new QLabel(topWin);
    labelEmptyPos1 -> setFixedWidth(15);
    horizontalBox->addWidget( labelEmptyPos1 );

    labelLevel = new QLabel( tr("LEVEL"), topWin );
    labelLevel->setFont( smallFont );
    labelLevel->setAlignment(Qt::AlignLeft);
    horizontalBox->addWidget( labelLevel );

    labelCurLevel = new QLabel( topWin );
    labelCurLevel -> setText(QString::number(level));
    labelCurLevel -> setFont( smallFont );
    labelCurLevel -> setAlignment(Qt::AlignRight);
    horizontalBox->addWidget( labelCurLevel );

    labelEmptyPos2 = new QLabel(topWin);
    labelEmptyPos2 -> setFixedWidth(15);
    horizontalBox->addWidget( labelEmptyPos2 );

    labelLives = new QLabel( tr("LIVES"), topWin );
    labelLives->setFont( smallFont );
    labelLives->setAlignment(Qt::AlignLeft);
    horizontalBox->addWidget( labelLives );

    labelCurLives = new QLabel( topWin );
    labelCurLives -> setText(QString::number(lives));
    labelCurLives->setFont( smallFont );
    labelCurLives-> setAlignment(Qt::AlignRight);
    horizontalBox->addWidget( labelCurLives );
    borderLayout->addWidget( topWin);

    // Create scene
    scene = new QGraphicsScene( border );

    // Set scene coordinates
    // We need the center of the scene to be 0,0 so we set the top left corner to be (-scWidth/2, -scHeight/2)
    scene->setSceneRect(-scWidth/2, -scHeight/2, scWidth, scHeight);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);


    // Set random background for the scene

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int randBackGround = qrand() % 2 + 1;
    QPixmap bckGround (QString(":/background/resource/backGround%1.jpg").arg(randBackGround));
    scene->setBackgroundBrush(bckGround);

    // Create QGraphicsView that will be nested as CentralWidget in the MainWindow

    view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene->setStickyFocus(true);
    borderLayout->addWidget(view);
    borderLayout->addStretch( 1 );

    /* Initially used to add top menu to the window
    !!!No longer needed!!!

    // Create the menus
    createActions();
    createMenus();
    */

    // Create pointer for the Top Score window
    topScore = new topScoreWin(this);

    //borderLayout->addWidget(test);


    // Set window title
    setWindowTitle(tr("Asteroids"));

    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), scene, SLOT(advance()));
    timer->start(1000 / 33);

    // Prepare the sound file - explosion
    explosionEffect = new QSoundEffect(this);
    explosionEffect->setSource(QUrl::fromLocalFile(":FX/resource/explosion.wav"));
    explosionEffect->setVolume(0.5f);

    // Prepare the sound file - laser
    laserEffect = new QSoundEffect(this);
    laserEffect->setSource(QUrl::fromLocalFile(":FX/resource/laser.wav"));
    laserEffect->setVolume(0.5f);

    // Call addButtons in order to add the buttons to the window
    addButtons();

}

// Function used to generate random values when required
int MainWindow::randInt(int low, int high) {
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

// Function that adds the ship to the game
void MainWindow::addShip()
{
    // Add the ship and set permanent focus on it
    theShip = new Ship(this);
    scene->addItem(theShip);
    theShip->setFocus();
    connect(theShip, SIGNAL(shipDestroyed()), this, SLOT(updateLives()));
}

// Function used to create the new level
// Adds the asteroids and saucers, call the addShip to add new ship
void MainWindow::newLevel()
{
    // Remove the old Ship if any
    if(theShip) { scene->removeItem(theShip); theShip->deleteLater(); }

    setObjectCounter(0); // setting the objectCounter to 0 before creating the level

    // Add the ship
    addShip();


    /*
    // Add n asteroids
    int total = 2;
    */
    // Add asteroids according to the level, substracting 1 from the level because we start from 0 in the array
    int astTotal = levelDescr[level - 1].asteroidsNum;
    for(int i = 0; i < astTotal; i++) {
        //QPointer<Asteroid> aSmallAsteroid = new Asteroid(1, this);
        //QPointer<Asteroid> aMediumAsteroid = new Asteroid(2, this);
        QPointer<Asteroid> aBigAsteroid = new Asteroid(3, this);

        // Possible asteroids coordinates for start are always 50 points away from the borders of the scene
        int margin = 50;
        int astCoordX = scWidth/2 - margin;
        int astCoordY = scHeight/2 - margin;
        /*aSmallAsteroid->setPos(randInt(-astCoordX,astCoordX), randInt(-astCoordY,astCoordY));
        scene->addItem(aSmallAsteroid);

        aMediumAsteroid->setPos(randInt(-astCoordX,astCoordX), randInt(-astCoordY,astCoordY));
        scene->addItem(aMediumAsteroid);*/

        aBigAsteroid->setPos(randInt(-astCoordX,astCoordX), randInt(-astCoordY,astCoordY));
        scene->addItem(aBigAsteroid);
    }


    // Add the saucers according to the level, substracting 1 from the level because we start from 0 in the array
    int saucerTotal = levelDescr[level - 1].saucerNum;
    for(int i = 0; i < saucerTotal; i++) {
        // Possible asteroids coordinates for start are always 50 points away from the borders of the scene
        int margin = 50;
        int saucerCoordX = scWidth/2 - margin;;
        int saucerCoordY = scHeight/2 - margin;;
        QPointer<Saucer> aSmallSaucer = new Saucer(1, this);
        aSmallSaucer->setPos(randInt(-saucerCoordX,saucerCoordX), randInt(-saucerCoordY,saucerCoordY));
        scene->addItem(aSmallSaucer);

        QPointer<Saucer> aBigSaucer = new Saucer(2, this);
        aBigSaucer->setPos(randInt(-saucerCoordX,saucerCoordX), randInt(-saucerCoordY,saucerCoordY));
        scene->addItem(aBigSaucer);
    }
}


// Function called by clicking newGame - used to reset some counters (lives, level, score) and hide buttons
void MainWindow::newGame()
{
    setBtnVisibility(5);

    lives = 2;
    labelCurLives -> setText(QString::number(lives));
    level = 1;
    labelCurLevel -> setText(QString::number(level));
    score = 0;
    labelCurScore -> setText(QString::number(score));

    scene->clear(); // Clear the scene

    newLevel();
}

// Function called when a load game is selected. It will set lives, level and score counters to the once from the save and start new level from this point
void MainWindow::loadGame(int loadGameID)
{
    lives = xmlGames[loadGameID].SLives;
    labelCurLives -> setText(QString::number(lives));
    level = xmlGames[loadGameID].SLevel;
    labelCurLevel -> setText(QString::number(level));
    score = xmlGames[loadGameID].SScore;
    labelCurScore -> setText(QString::number(score));
    scene->clear(); // Clear the scene
    setBtnVisibility(5);
    newLevel();
}


// Function called when you click save game. Asks if you want to replace existing save.
// If confirmation is received OR no save is replaced (using empty slot) the game is saved by calling saveCurGame() and we are returned back to Continue / Save options
void MainWindow::saveGame(int saveGameSlotID)
{
    int answer = 1;
    if(xmlGames[saveGameSlotID].SScore > 0)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Asteroids", "Do you want to replace this save?",
                                        QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes){
            answer = 1;
        }
        else answer = 0;
    }

    if(answer == 0)
    {
        setBtnVisibility(5);
        setBtnVisibility(1);
    }

    if(answer == 1)
    {
        saveCurGame(saveGameSlotID);
    }
}

// Function used to save the games to the file
void MainWindow::saveCurGame(int saveGameSlotID){

    parseSavesXML();

    QFile* file = new QFile("saves.xml");
    if (!file->open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::critical(this,
                              "ParseXML",
                              "Couldn't open Save Games file",
                              QMessageBox::Ok);
        return;
    }

    QXmlStreamWriter stream(file);

    // Setup and start the stream
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("saves");

    // Write the file - 10 identical elements
    for(int i = 1; i<=10; i++)
    {
        if(i == saveGameSlotID)
        {
            bool ok;
            QString savename = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                                              tr("User name:"), QLineEdit::Normal,
                                                              QDir::home().dirName(), &ok);
            if(ok)
            {
                xmlGames[i].SSavename = savename;
                xmlGames[i].SScore = score;
                xmlGames[i].SLives = lives;
                xmlGames[i].SLevel = level + 1; // Add 1 to current level so the game will start from next level as the current one is finished!
            }
        }
        // Write <save id= ? >
        stream.writeStartElement("save");
        stream.writeAttribute("id",QString::number(i));

        // Wite <savename>

        stream.writeStartElement("savename");
        stream.writeCharacters(xmlGames[i].SSavename);
        stream.writeEndElement();

        // Write <score>
        stream.writeStartElement("score");
        stream.writeCharacters(QString::number(xmlGames[i].SScore));
        stream.writeEndElement();

        // Write <lives>
        stream.writeStartElement("lives");
        stream.writeCharacters(QString::number(xmlGames[i].SLives));
        stream.writeEndElement();

        // Write <level>
        stream.writeStartElement("level");
        stream.writeCharacters(QString::number(xmlGames[i].SLevel));
        stream.writeEndElement();

        // Write </save>
        stream.writeEndElement();
    }
    // close the document
    stream.writeEndElement();
    stream.writeEndDocument();


    file->close();

    setBtnVisibility(5);
    setBtnVisibility(1);

}

// Function that parses the saves.xml. Used for both load and save game display.
void MainWindow::parseSavesXML()
{
    QFile* file = new QFile("saves.xml");
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this,
                              "ParseXML",
                              "Couldn't open Save Games file",
                              QMessageBox::Ok);
        return;
    }
    QXmlStreamReader xml(file);
    while(!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartElement)
        {
            if(xml.name() == "saves")
            {
                            continue;
            }
            if(xml.name() == "save")
            {
                QXmlStreamAttributes attributes = xml.attributes();
                int tempID = attributes.value("id").toInt();
                if(attributes.hasAttribute("id"))
                {
                    if( tempID > 10)
                    {
                        break;
                    }
                }
                // Parse saves
                xml.readNext();
                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "save"))
                {
                    if(xml.name() == "savename" && xml.tokenType() == QXmlStreamReader::StartElement )
                    {
                        xml.readNext();
                        if(xml.text().isEmpty())
                        {
                            xmlGames[tempID].SSavename = "  _____  ";
                        }
                        else
                        {
                            xmlGames[tempID].SSavename = xml.text().toString();
                        }
                    }
                    if(xml.name() == "score" && xml.tokenType() == QXmlStreamReader::StartElement)
                    {
                        xml.readNext();
                        if(!xml.text().isNull())
                        {
                            xmlGames[tempID].SScore = xml.text().toInt();
                        }
                        else xmlGames[tempID].SScore = 0;
                    }
                    if(xml.name() == "lives" && xml.tokenType() == QXmlStreamReader::StartElement)
                    {
                        xml.readNext();
                        if(!xml.text().isNull())
                        {
                            xmlGames[tempID].SLives = xml.text().toInt();
                        }
                        else xmlGames[tempID].SLives = 0;
                    }
                    if(xml.name() == "level" && xml.tokenType() == QXmlStreamReader::StartElement)
                    {
                        xml.readNext();
                        if(!xml.text().isNull())
                        {
                            xmlGames[tempID].SLevel = xml.text().toInt();
                        }
                        else xmlGames[tempID].SLevel = 0;
                    }
                    xml.readNext();

                }
            }
        }
    }
    if(xml.hasError()) {
        QMessageBox::critical(this,
                              "ParseXML2",
                              xml.errorString(),
                              QMessageBox::Ok);
    }

    file->close();

}

// Function used to show the games that are possible for load. Locks buttons that have no saves on them.
void MainWindow::showLoadGames()
{


    parseSavesXML();

    setBtnVisibility(5);

    QString tempText;
    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[1].SScore).arg(xmlGames[1].SLives).arg(xmlGames[1].SSavename).arg(xmlGames[1].SLevel);
    btnLoad1->setText(tempText);
    if(xmlGames[1].SScore == 0)
    {
        btnLoad1->setEnabled(false);
    }
    else btnLoad1->setEnabled(true);

    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[2].SScore).arg(xmlGames[2].SLives).arg(xmlGames[2].SSavename).arg(xmlGames[2].SLevel);
    btnLoad2->setText(tempText);
    if(xmlGames[2].SScore == 0)
    {
        btnLoad2->setEnabled(false);
    }
    else btnLoad2->setEnabled(true);

    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[3].SScore).arg(xmlGames[3].SLives).arg(xmlGames[3].SSavename).arg(xmlGames[3].SLevel);
    btnLoad3->setText(tempText);
    if(xmlGames[3].SScore == 0)
    {
        btnLoad3->setEnabled(false);
    }
    else btnLoad3->setEnabled(true);

    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[4].SScore).arg(xmlGames[4].SLives).arg(xmlGames[4].SSavename).arg(xmlGames[4].SLevel);
    btnLoad4->setText(tempText);
    if(xmlGames[4].SScore == 0)
    {
        btnLoad4->setEnabled(false);
    }
    else btnLoad4->setEnabled(true);

    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[5].SScore).arg(xmlGames[5].SLives).arg(xmlGames[5].SSavename).arg(xmlGames[5].SLevel);
    btnLoad5->setText(tempText);
    if(xmlGames[5].SScore == 0)
    {
        btnLoad5->setEnabled(false);
    }
    else btnLoad5->setEnabled(true);

    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[6].SScore).arg(xmlGames[6].SLives).arg(xmlGames[6].SSavename).arg(xmlGames[6].SLevel);
    btnLoad6->setText(tempText);
    if(xmlGames[6].SScore == 0)
    {
        btnLoad6->setEnabled(false);
    }
    else btnLoad6->setEnabled(true);

    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[7].SScore).arg(xmlGames[7].SLives).arg(xmlGames[7].SSavename).arg(xmlGames[7].SLevel);
    btnLoad7->setText(tempText);
    if(xmlGames[7].SScore == 0)
    {
        btnLoad7->setEnabled(false);
    }
    else btnLoad7->setEnabled(true);

    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[8].SScore).arg(xmlGames[8].SLives).arg(xmlGames[8].SSavename).arg(xmlGames[8].SLevel);
    btnLoad8->setText(tempText);
    if(xmlGames[8].SScore == 0)
    {
        btnLoad8->setEnabled(false);
    }
    else btnLoad8->setEnabled(true);

    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[9].SScore).arg(xmlGames[9].SLives).arg(xmlGames[9].SSavename).arg(xmlGames[9].SLevel);
    btnLoad9->setText(tempText);
    if(xmlGames[9].SScore == 0)
    {
        btnLoad9->setEnabled(false);
    }
    else btnLoad9->setEnabled(true);

    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[10].SScore).arg(xmlGames[10].SLives).arg(xmlGames[10].SSavename).arg(xmlGames[10].SLevel);
    btnLoad10->setText(tempText);
    if(xmlGames[10].SScore == 0)
    {
        btnLoad10->setEnabled(false);
    }
    else btnLoad10->setEnabled(true);

    setBtnVisibility(3);

}

// Function that shows the save game slots
void MainWindow::showSavedGames()
{

    parseSavesXML();

    QString tempText;

    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[1].SScore).arg(xmlGames[1].SLives).arg(xmlGames[1].SSavename).arg(xmlGames[1].SLevel);
    btnSave1->setText(tempText);

    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[2].SScore).arg(xmlGames[2].SLives).arg(xmlGames[2].SSavename).arg(xmlGames[2].SLevel);
    btnSave2->setText(tempText);

    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[3].SScore).arg(xmlGames[3].SLives).arg(xmlGames[3].SSavename).arg(xmlGames[3].SLevel);
    btnSave3->setText(tempText);


    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[4].SScore).arg(xmlGames[4].SLives).arg(xmlGames[4].SSavename).arg(xmlGames[4].SLevel);
    btnSave4->setText(tempText);


    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[5].SScore).arg(xmlGames[5].SLives).arg(xmlGames[5].SSavename).arg(xmlGames[5].SLevel);
    btnSave5->setText(tempText);

    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[6].SScore).arg(xmlGames[6].SLives).arg(xmlGames[6].SSavename).arg(xmlGames[6].SLevel);
    btnSave6->setText(tempText);


    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[7].SScore).arg(xmlGames[7].SLives).arg(xmlGames[7].SSavename).arg(xmlGames[7].SLevel);
    btnSave7->setText(tempText);


    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[8].SScore).arg(xmlGames[8].SLives).arg(xmlGames[8].SSavename).arg(xmlGames[8].SLevel);
    btnSave8->setText(tempText);


    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[9].SScore).arg(xmlGames[9].SLives).arg(xmlGames[9].SSavename).arg(xmlGames[9].SLevel);
    btnSave9->setText(tempText);


    tempText = QString("NAME: %3  SCORE: %1  LIVES: %2 LEVEL: %4").arg(xmlGames[10].SScore).arg(xmlGames[10].SLives).arg(xmlGames[10].SSavename).arg(xmlGames[10].SLevel);
    btnSave10->setText(tempText);


    setBtnVisibility(5);
    setBtnVisibility(4);
}

// Function continue - used to continue to next level
void MainWindow::cntGame()
{
    setBtnVisibility(5);
    updateLevel();
    newLevel();
}

// Function used to update the score when an object is destroyed
void MainWindow::updateScore(int size)
{
    if      (size == 1) { score += 100; }
    else if (size == 2) { score += 50;  }
    else if (size == 3) { score += 20;  }
    else if (size == 4) { score += 1000;}
    else if (size == 5) { score += 500; }

    labelCurScore -> setText(QString::number(score));
}

// Function that is used to set (reset) the objects counter.
void MainWindow::setObjectCounter(int upd){
    objectCounter = upd;
}

// Function used to update the object counter. Also shows the proper menu if the objects get to 0
void MainWindow::updateObjectCounter(int upd)
{
    objectCounter+=upd;

    if(objectCounter==0){
        setBtnVisibility(1);
    }
}

// Function used to check the object counter value
int MainWindow::checkObjectCounter()
{
    return objectCounter;
}

// FUnction called when we have to change the value of the lives. Also checks if any are left and resets the ship if we still have lives.
void MainWindow::updateLives()
{

    // Check if any lives are left - if yes, add new ship
    if(lives>0)
    {
        addShip();
        // Lower the number of lives
        lives--;
        labelCurLives -> setText(QString::number(lives));
    }
    // If no more lives left display check the scores
    // If the game have result that goes for top Score then ask for name, save it and display top scores
    // If the game have result that does not affect the top score table display "Game Over"
    else
    {
        if(checkForTopScore())
        {
            updateTopScores();
            topScore->updateLabels();
            topScore->show();
            setBtnVisibility(2);

        }
        else
        {
            setBtnVisibility(2);
            displayGameOver();
        }
    }
}

// Check if result is clasified for top-score

bool MainWindow::checkForTopScore()
{

    bool result = false;
    // Set which file we will open
    QFile file("topscore.txt");

    // Try to open the file for reading
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
              file.open(QIODevice::ReadWrite | QIODevice::Text);

    // Reading the file:
    // Either read until end or read until 10 rows
    int counter = 0;
    while (!file.atEnd() and counter < maxTopScores)
    {
        // Read line
        QString line = file.readLine();

        // Process line
        // We need to have Name (w/o spaces) and Scores on this line. They must be delimited by space

        // Setting delimiter - will be empty space
        QString delimiterPattern(" ");

        // Splitting the string
        QStringList splitLine = line.split(delimiterPattern);

        // We need the second element of the split
        if(splitLine[1].toInt()<score)
        {
            result = true;
        }

        // Raise the counter
        counter++;
    }

    file.close();

    // We might still have empty slots in the top scores file. Check the counter

    if(counter < maxTopScores)
    {
        result = true;
    }

    return result;

}

void MainWindow::updateTopScores()
{
    bool ok;
    QString topScoreName = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                                      tr("Name:"), QLineEdit::Normal,
                                                      QDir::home().dirName(), &ok);

    if(ok)
    {
        // First we will read whole file, split the lines, then find the correct line that we want to replace or append
        // Second we write the file again

        // Set which file we will open
        QFile file("topscore.txt");

        // Try to open the file
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        // Read whole file
        QString allFile = file.readAll();

        // Close file
        file.close();

        // Setting delimiter - will be new line
        QString delimiterPatternNewLine("\n");

        // Splitting the file
        QStringList splitLine = allFile.split(delimiterPatternNewLine);

        // Check if we want to replace line or append (this is in case we still have empty top score slots)
        if(splitLine.size() < maxTopScores)
        {
            QString line = topScoreName + " " + QString::number(score);

            // Open file for append

            if (!file.open(QIODevice::Append | QIODevice::Text))
                     return;

            QTextStream outStream(&file);

            // Append the line
            outStream << line << "\n";

            // Close the file
            file.close();
        }
        // Else we run trough the list and find the line we want to replace
        else{
            for(int i = 0; i < maxTopScores; i++)
            {
                // We have to split the lines to check the top score
                QString delimiterPattern(" ");
                QStringList lineContent = splitLine[i].split(delimiterPattern);

                // Check if line top score is below the current score
                // If not - let the loop continue
                if(lineContent[1].toInt() < score)
                {
                    QString line = topScoreName + " " + QString::number(score);
                    splitLine.insert(i,line);
                    break;
                }
            }

            // Re-write the file
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                     return;
            QTextStream outStream(&file);
            for(int i = 0; i < maxTopScores; i++)
            {
                QString outline;
                if(i == maxTopScores - 1)
                {
                    outline = splitLine[i];
                }
                else
                {
                    outline = splitLine[i]+"\n";
                }
                outStream << outline;
            }

            file.close();

        }
    }
}

// Function called when changing the level
void MainWindow::updateLevel()
{

    level++;
    labelCurLevel -> setText(QString::number(level));
}


// Function that displays game over splash
// IMPORTANT: WILL BE REPLACED!!!
void MainWindow::displayGameOver()
{
    QPixmap pixmap(":/GameOver/resource/GameOver.jpg");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();
    qApp->processEvents();
}


// Function called when we have to play explosion sound
void MainWindow::playExplosionSound()
{
    explosionEffect->play();
}

// Function called when we have to play laser sound
void MainWindow::playLaserSound()
{
    laserEffect->play();
}

/* Initially used to add top menu to the window
!!!No longer needed!!!

void MainWindow::createActions()
{
     newGameAct = new QAction(tr("&New"), this);
     newGameAct->setShortcuts(QKeySequence::New);
     connect(newGameAct, SIGNAL(triggered()), this, SLOT(newGame()));

     loadGameAct = new QAction(tr("&Load"), this);
     loadGameAct->setShortcuts(QKeySequence::Open);
     connect(loadGameAct, SIGNAL(triggered()), this, SLOT(showLoadGames()));

     saveGameAct = new QAction(tr("&Save"), this);
     saveGameAct->setShortcuts(QKeySequence::Save);
     connect(saveGameAct, SIGNAL(triggered()), this, SLOT(showSavedGames()));

}


void MainWindow::createMenus()
{
    GameMenu = menuBar()->addMenu(tr("&Menu"));
    GameMenu->addAction(newGameAct);
    GameMenu->addAction(loadGameAct);
    GameMenu->addAction(saveGameAct);
}
*/


// Function to set the buttons visiblity
// Options:
// 1 - Show btnSave and btnContinue
// 2 - Show btnStart and btnLoad
// 3 - Show Load buttons
// 4 - Show Save buttons
// 5 (default) - Hide all buttons


void MainWindow::setBtnVisibility(int temp)
{
    if(temp == 1)
    {
        btnSave->setVisible(true);
        btnContinue->setVisible(true);
    }
    else if(temp == 2)
    {
        btnStart->setVisible(true);
        btnLoad->setVisible(true);
        btnTopScore->setVisible(true);
    }
    else if(temp == 3)
    {
        btnLoad1->setVisible(true);
        btnLoad2->setVisible(true);
        btnLoad3->setVisible(true);
        btnLoad4->setVisible(true);
        btnLoad5->setVisible(true);
        btnLoad6->setVisible(true);
        btnLoad7->setVisible(true);
        btnLoad8->setVisible(true);
        btnLoad9->setVisible(true);
        btnLoad10->setVisible(true);
        btnLoadBack->setVisible(true);
    }
    else if(temp == 4)
    {
        btnSave1->setVisible(true);
        btnSave2->setVisible(true);
        btnSave3->setVisible(true);
        btnSave4->setVisible(true);
        btnSave5->setVisible(true);
        btnSave6->setVisible(true);
        btnSave7->setVisible(true);
        btnSave8->setVisible(true);
        btnSave9->setVisible(true);
        btnSave10->setVisible(true);
        btnSaveBack->setVisible(true);
    }
    else
    {
        btnSave->setVisible(false);
        btnStart->setVisible(false);
        btnTopScore->setVisible(false);
        btnContinue->setVisible(false);
        btnLoad->setVisible(false);

        btnSave1->setVisible(false);
        btnSave2->setVisible(false);
        btnSave3->setVisible(false);
        btnSave4->setVisible(false);
        btnSave5->setVisible(false);
        btnSave6->setVisible(false);
        btnSave7->setVisible(false);
        btnSave8->setVisible(false);
        btnSave9->setVisible(false);
        btnSave10->setVisible(false);
        btnSaveBack->setVisible(false);

        btnLoad1->setVisible(false);
        btnLoad2->setVisible(false);
        btnLoad3->setVisible(false);
        btnLoad4->setVisible(false);
        btnLoad5->setVisible(false);
        btnLoad6->setVisible(false);
        btnLoad7->setVisible(false);
        btnLoad8->setVisible(false);
        btnLoad9->setVisible(false);
        btnLoad10->setVisible(false);
        btnLoadBack->setVisible(false);
    }
}

// Adding buttons to the window. All are set to hidden.
void MainWindow::addButtons()
{


    // Standard buttons

    // Highest placed standard button will be above the middle of the window
    int firstStandardButtonVPos = mwHeight/2 - ((standardButtonsOffset)*1.5) ;

    // Standard buttons top left X axis starts on same point
    int standardButtonHPos = (mwWidth-butWidth)/2;


    btnSave = new QPushButton(border);
    btnSave->setGeometry(QRect(standardButtonHPos,firstStandardButtonVPos,butWidth,butHeight));
    btnSave->setText("SAVE GAME");
    btnSave->setVisible(false);
    connect(btnSave, SIGNAL(released()), this, SLOT(showSavedGames()));

    btnContinue = new QPushButton(border);
    btnContinue->setGeometry(QRect(standardButtonHPos,firstStandardButtonVPos+standardButtonsOffset,butWidth,butHeight));
    btnContinue->setText("CONTINUE");
    btnContinue->setVisible(false);
    connect(btnContinue, SIGNAL(released()), this, SLOT(cntGame()));

    btnStart = new QPushButton(border);
    btnStart->setGeometry(QRect(standardButtonHPos,firstStandardButtonVPos,butWidth,butHeight));
    btnStart->setText("START NEW GAME");
    btnStart->setVisible(true);
    connect(btnStart, SIGNAL(released()), this, SLOT(newGame()));

    btnLoad = new QPushButton(border);
    btnLoad->setGeometry(QRect(standardButtonHPos,firstStandardButtonVPos+standardButtonsOffset,butWidth,butHeight));
    btnLoad->setText("LOAD SAVED GAME");
    btnLoad->setVisible(true);
    connect(btnLoad, SIGNAL(released()), this, SLOT(showLoadGames()));

    btnTopScore = new QPushButton(border);
    btnTopScore->setGeometry(QRect(standardButtonHPos,firstStandardButtonVPos+(standardButtonsOffset)*2,butWidth,butHeight));
    btnTopScore->setText("TOP SCORES");
    btnTopScore->setVisible(true);
    connect(btnTopScore, SIGNAL(released()), this, SLOT(pshdButton()));

    // Calculating save / load buttons horizontal position
    int saveLoadButtonsHPos = (mwWidth-btnSLWidth)/2;

    // Save game buttons

    btnSave1 = new QPushButton(border);
    btnSave1->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos,btnSLWidth,btnSLHeight));
    btnSave1->setText("SAVE GAME 1");
    btnSave1->setVisible(false);
    connect(btnSave1, SIGNAL(released()), this, SLOT(pshdButton()));

    btnSave2 = new QPushButton(border);
    btnSave2->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset,btnSLWidth,btnSLHeight));
    btnSave2->setText("SAVE GAME 2");
    btnSave2->setVisible(false);
    connect(btnSave2, SIGNAL(released()), this, SLOT(pshdButton()));

    btnSave3 = new QPushButton(border);
    btnSave3->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*2,btnSLWidth,btnSLHeight));
    btnSave3->setText("SAVE GAME 3");
    btnSave3->setVisible(false);
    connect(btnSave3, SIGNAL(released()), this, SLOT(pshdButton()));

    btnSave4 = new QPushButton(border);
    btnSave4->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*3,btnSLWidth,btnSLHeight));
    btnSave4->setText("SAVE GAME 4");
    btnSave4->setVisible(false);
    connect(btnSave4, SIGNAL(released()), this, SLOT(pshdButton()));

    btnSave5 = new QPushButton(border);
    btnSave5->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*4,btnSLWidth,btnSLHeight));
    btnSave5->setText("SAVE GAME 5");
    btnSave5->setVisible(false);
    connect(btnSave5, SIGNAL(released()), this, SLOT(pshdButton()));

    btnSave6 = new QPushButton(border);
    btnSave6->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*5,btnSLWidth,btnSLHeight));
    btnSave6->setText("SAVE GAME 6");
    btnSave6->setVisible(false);
    connect(btnSave6, SIGNAL(released()), this, SLOT(pshdButton()));

    btnSave7 = new QPushButton(border);
    btnSave7->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*6,btnSLWidth,btnSLHeight));
    btnSave7->setText("SAVE GAME 7");
    btnSave7->setVisible(false);
    connect(btnSave7, SIGNAL(released()), this, SLOT(pshdButton()));

    btnSave8 = new QPushButton(border);
    btnSave8->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*7,btnSLWidth,btnSLHeight));
    btnSave8->setText("SAVE GAME 8");
    btnSave8->setVisible(false);
    connect(btnSave8, SIGNAL(released()), this, SLOT(pshdButton()));

    btnSave9 = new QPushButton(border);
    btnSave9->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*8,btnSLWidth,btnSLHeight));
    btnSave9->setText("SAVE GAME 9");
    btnSave9->setVisible(false);
    connect(btnSave9, SIGNAL(released()), this, SLOT(pshdButton()));

    btnSave10 = new QPushButton(border);
    btnSave10->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*9,btnSLWidth,btnSLHeight));
    btnSave10->setText("SAVE GAME 10");
    btnSave10->setVisible(false);
    connect(btnSave10, SIGNAL(released()), this, SLOT(pshdButton()));

    btnSaveBack = new QPushButton(border);
    btnSaveBack->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*10,btnSLWidth,btnSLHeight));
    btnSaveBack->setText("BACK");
    btnSaveBack->setVisible(false);
    connect(btnSaveBack, SIGNAL(released()), this, SLOT(pshdButton()));




    // Load game buttons

    btnLoad1 = new QPushButton(border);
    btnLoad1->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos,btnSLWidth,btnSLHeight));
    btnLoad1->setText("LOAD GAME 1");
    btnLoad1->setVisible(false);
    connect(btnLoad1, SIGNAL(released()), this, SLOT(pshdButton()));

    btnLoad2 = new QPushButton(border);
    btnLoad2->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset,btnSLWidth,btnSLHeight));
    btnLoad2->setText("LOAD GAME 2");
    btnLoad2->setVisible(false);
    connect(btnLoad2, SIGNAL(released()), this, SLOT(pshdButton()));

    btnLoad3 = new QPushButton(border);
    btnLoad3->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*2,btnSLWidth,btnSLHeight));
    btnLoad3->setText("LOAD GAME 3");
    btnLoad3->setVisible(false);
    connect(btnLoad3, SIGNAL(released()), this, SLOT(pshdButton()));

    btnLoad4 = new QPushButton(border);
    btnLoad4->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*3,btnSLWidth,btnSLHeight));
    btnLoad4->setText("LOAD GAME 4");
    btnLoad4->setVisible(false);
    connect(btnLoad4, SIGNAL(released()), this, SLOT(pshdButton()));

    btnLoad5 = new QPushButton(border);
    btnLoad5->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*4,btnSLWidth,btnSLHeight));
    btnLoad5->setText("LOAD GAME 5");
    btnLoad5->setVisible(false);
    connect(btnLoad5, SIGNAL(released()), this, SLOT(pshdButton()));

    btnLoad6 = new QPushButton(border);
    btnLoad6->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*5,btnSLWidth,btnSLHeight));
    btnLoad6->setText("LOAD GAME 6");
    btnLoad6->setVisible(false);
    connect(btnLoad6, SIGNAL(released()), this, SLOT(pshdButton()));

    btnLoad7 = new QPushButton(border);
    btnLoad7->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*6,btnSLWidth,btnSLHeight));
    btnLoad7->setText("LOAD GAME 7");
    btnLoad7->setVisible(false);
    connect(btnLoad7, SIGNAL(released()), this, SLOT(pshdButton()));

    btnLoad8 = new QPushButton(border);
    btnLoad8->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*7,btnSLWidth,btnSLHeight));
    btnLoad8->setText("LOAD GAME 8");
    btnLoad8->setVisible(false);
    connect(btnLoad8, SIGNAL(released()), this, SLOT(pshdButton()));

    btnLoad9 = new QPushButton(border);
    btnLoad9->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*8,btnSLWidth,btnSLHeight));
    btnLoad9->setText("LOAD GAME 9");
    btnLoad9->setVisible(false);
    connect(btnLoad9, SIGNAL(released()), this, SLOT(pshdButton()));

    btnLoad10 = new QPushButton(border);
    btnLoad10->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*9,btnSLWidth,btnSLHeight));
    btnLoad10->setText("LOAD GAME 10");
    btnLoad10->setVisible(false);
    connect(btnLoad10, SIGNAL(released()), this, SLOT(pshdButton()));

    btnLoadBack = new QPushButton(border);
    btnLoadBack->setGeometry(QRect(saveLoadButtonsHPos,btnSLVPos+btnSaveLoadOffset*10,btnSLWidth,btnSLHeight));
    btnLoadBack->setText("BACK");
    btnLoadBack->setVisible(false);
    connect(btnLoadBack, SIGNAL(released()), this, SLOT(pshdButton()));
}

// We will use this slot as the QT buttons do not support buttonID

void MainWindow::pshdButton()
{
    QPushButton *clckedButton = (QPushButton *)sender();
    if(btnSave1 == clckedButton)
    {
        saveGame(1);
    }
    else if(btnSave2 == clckedButton)
    {
        saveGame(2);
    }
    else if(btnSave3 == clckedButton)
    {
        saveGame(3);
    }
    else if(btnSave4 == clckedButton)
    {
        saveGame(4);
    }
    else if(btnSave5 == clckedButton)
    {
        saveGame(5);
    }
    else if(btnSave6 == clckedButton)
    {
        saveGame(6);
    }
    else if(btnSave7 == clckedButton)
    {
        saveGame(7);
    }
    else if(btnSave8 == clckedButton)
    {
        saveGame(8);
    }
    else if(btnSave9 == clckedButton)
    {
        saveGame(9);
    }
    else if(btnSave10 == clckedButton)
    {
        saveGame(10);
    }
    else if(btnSaveBack == clckedButton)
    {
        setBtnVisibility(5);
        setBtnVisibility(1);
    }
    else if(btnLoad1 == clckedButton)
    {
        loadGame(1);
    }
    else if(btnLoad2 == clckedButton)
    {
        loadGame(2);
    }
    else if(btnLoad3 == clckedButton)
    {
        loadGame(3);
    }
    else if(btnLoad4 == clckedButton)
    {
        loadGame(4);
    }
    else if(btnLoad5 == clckedButton)
    {
        loadGame(5);
    }
    else if(btnLoad6 == clckedButton)
    {
        loadGame(6);
    }
    else if(btnLoad7 == clckedButton)
    {
        loadGame(7);
    }
    else if(btnLoad8 == clckedButton)
    {
        loadGame(8);
    }
    else if(btnLoad9 == clckedButton)
    {
        loadGame(9);
    }
    else if(btnLoad10 == clckedButton)
    {
        loadGame(10);
    }
    else if(btnLoadBack == clckedButton)
    {
        setBtnVisibility(5);
        setBtnVisibility(2);
    }
    else if(btnTopScore == clckedButton)
    {
        topScore->updateLabels();
        topScore->show();
    }

}

// Send the ship's coordinates - (0,0) if the ship is currently destroyed
QPointF MainWindow::getShipCoordinates() {
    if(!theShip.isNull()) {
        return QPointF(theShip->x(), theShip->y());
    } else {
        return QPointF(0, 0);
    }
}

MainWindow::~MainWindow()
{
}
