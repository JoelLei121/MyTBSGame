#include "Headers/MenuUi.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QSoundEffect>

MenuUi::MenuUi(QMainWindow *parent) : QMainWindow(parent)
{
    bgm = new QSoundEffect(this);
    bgm->setSource(QUrl::fromLocalFile(":/bgm/menu.wav"));
    bgm->setLoopCount(QSoundEffect::Infinite);
    bgm->setVolume(0.85);
    // setFixedSize(600, 400);
    scene = new QGraphicsScene(20, 20, 580, 380);
    view = new QGraphicsView(scene);

    setCentralWidget(view);
    image = new QImage(":/background/brick.png"); 
    if(image->isNull())
    {
        printf("Failed to load the image.\n");
    }
    else
    {
        brush = new QBrush(*image);
        view->setBackgroundBrush(*brush);
    }
    
    LabelFont = new QFont("Bahnschrift Light", 36);
    ButtonFont = new QFont("Calibri Regular", 18);

    WelcomeLabel = new QLabel("My TBS Game");
    WelcomeLabel->setAlignment(Qt::AlignCenter);
    WelcomeLabel->setFont(QFont("Audiowide", 36));
    // WelcomeLabel->setStyleSheet("QLabel { background-color : white; color : black; }");
    WelcomeLabel->setMinimumSize(300, 120);
    // WelcomeLabel->setFixedSize(QSize(320, 150));

    btnStartGame = new QPushButton("START GAME");
    btnTutorial = new QPushButton("TUTORIAL");
    btnExit = new QPushButton("EXIT");
    btnStartGame->setFont(*ButtonFont);
    btnTutorial->setFont(*ButtonFont);
    btnExit->setFont(*ButtonFont);
    btnStartGame->setMinimumSize(250, 60);
    btnTutorial->setMinimumSize(250, 60);
    btnExit->setMinimumSize(250, 60);


    layout = new QVBoxLayout;
    layout->addWidget(WelcomeLabel);
    layout->addSpacing(30);
    layout->addWidget(btnStartGame);
    layout->addWidget(btnTutorial);
    layout->addWidget(btnExit);
    layout->setSpacing(10);
    
    view->setLayout(layout);

    connect(btnStartGame, &QPushButton::clicked, [=](){ emit SelectLevel(); });
    connect(btnTutorial, &QPushButton::clicked, [=](){ emit Tutorial(); });
    connect(btnExit, &QPushButton::clicked, this, &MenuUi::close);
}

void MenuUi::backToMenu()
{
    this->show();
    if(!bgm->isPlaying())   bgm->play();
}
