#include "Headers/LevelUi.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QCloseEvent>

LevelButton::LevelButton(const QString &text, QWidget *parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setText(text);
}

QSize LevelButton::sizeHint() const 
{
    QSize size = QPushButton::sizeHint();
    size.rheight() = 50;
    size.rwidth() = 50;
    return size;
}

LevelUi::LevelUi(QWidget *parent) : QMainWindow(parent)
{
    scene = new QGraphicsScene(20, 20, 580, 380);
    view = new QGraphicsView(scene);

    setCentralWidget(view);
    image = new QImage(":/background/level.png"); 
    if(image->isNull())
    {
        printf("Failed to load the image.\n");
    }
    else
    {
        brush = new QBrush(*image);
        view->setBackgroundBrush(*brush);
    }

    connect(this, &LevelUi::setLevel, &LevelUi::createLevel);

    LabelFont = new QFont("Calisto MT", 48);
    ButtonFont = new QFont("Calibri Regular", 18);
    
    LevelLabel = new QLabel("Select Level");
    LevelLabel->setFont(*LabelFont);
    LevelLabel->setAlignment(Qt::AlignCenter);
    LevelLabel->setStyleSheet("QLabel { color : white; }");
    // labelLayout = new QVBoxLayout;
    // labelLayout->addWidget(LevelLabel);
    btnInit(6);
    // printf("widget added");
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(LevelLabel);
    // mainLayout->addSpacing(50);
    mainLayout->addLayout(btnLayout);

    setCentralWidget(view);
    view->setLayout(mainLayout);
}

void LevelUi::btnInit(int n)
{
    for(int i = 1; i <= n; ++i)
    {
        LevelButton *btn = new LevelButton(QString::number(i));
        btn->setFont(*ButtonFont);
        connect(btn, LevelButton::clicked, [this, i](){ emit setLevel(i); });
        btnLevel.push_back(btn);
    }
    btnLayout = new QGridLayout;
    for(int i = 0; i < n; ++i)
    {
        btnLayout->addWidget(btnLevel[i], i/4 + 1, i%4 + 1);
    }
}

void LevelUi::createLevel(int i)
{
    emit sendLevelCreationRequset(i);
    QTimer::singleShot(100, this, [this](){this->hide();});
}

void LevelUi::enteringLevelUi()
{
    this->show();
}

void LevelUi::closeEvent(QCloseEvent *event)
{
    this->hide();
    emit backToMenu();
    event->ignore();
}