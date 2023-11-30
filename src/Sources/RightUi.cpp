#include "Headers/RightUi.h"
#include "Headers/RoleUnit.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QPlainTextEdit>
#include <iostream>

rightUi::rightUi(QWidget* parent) : QWidget(parent)
{
    // Font
    LabelFont = new QFont("Bahnschrift Light", 16);
    LargeButtonFont = new QFont("Calibri Regular", 18);
    SmallButtonFont = new QFont("Calibri Regular", 12);

    // Title
    initTitleBox();
    // info
    initDataBox();
    // battle record
    initBattleRecord();
    // turn controller
    initTurnController();

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gameTitleLayout);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(infoLayout);
    mainLayout->addSpacing(80);
    mainLayout->addLayout(battleRecordLayout);
    mainLayout->addSpacing(100);
    mainLayout->addLayout(turnControllerLayout);
    setLayout(mainLayout);

    connect(btnAttack, &QPushButton::clicked, [=](){ emit sendRoleAction("ATTACK"); });
    connect(btnMove, &QPushButton::clicked, [=](){ emit sendRoleAction("MOVE"); });
    connect(btnClearBattleRecord, &QPushButton::clicked, [=](){ this->battleRecordBox->clear(); });
    connect(btnEndTurn, &QPushButton::clicked, [=](){ emit sendTurnEnding(); }); // to BattleSystem::RoundEnd
    connect(this, &rightUi::sendRoleAction, &rightUi::RoleActionSelectionChanged);
}

rightUi::~rightUi()
{
    delete mainLayout;
}


// init function
void rightUi::initTitleBox()
{
    gameTitle = new QLabel("My TBS Game");
    gameTitle->setFont(QFont("Audiowide", 32));
    gameTitleLayout = new QVBoxLayout;
    gameTitleLayout->addWidget(gameTitle);
}

void rightUi::initDataBox()
{


    // info
    roleTeamLabel = new QLabel("Team:");
    roleClassLabel = new QLabel("Class:");
    roleHPLabel = new QLabel("HP:");
    roleATKLabel = new QLabel("ATK:");
    roleActionLabel = new QLabel("Current action:\tMove");
    roleActionLabel->setFont(*LabelFont);
    roleActionLabel->setStyleSheet("QLabel { background-color : darkGreen; color : white; }");
    roleActionLabel->setMinimumHeight(60);
    roleActionLabel->setAlignment(Qt::AlignCenter);
    roleTeamLabel->setFont(*LabelFont);
    roleClassLabel->setFont(*LabelFont);
    roleHPLabel->setFont(*LabelFont);
    roleATKLabel->setFont(*LabelFont);

    // button
    btnAttack = new QPushButton("Attack!");
    btnMove = new QPushButton("Move!");
    btnLayout = new QHBoxLayout;
    btnMove->setFixedSize(120, 60);
    btnAttack->setFixedSize(120, 60);
    btnMove->setFont(*LargeButtonFont);
    btnAttack->setFont(*LargeButtonFont);
    btnLayout->setAlignment(Qt::AlignCenter);
    btnLayout->addWidget(btnAttack);
    btnLayout->addWidget(btnMove);

    infoLayout = new QGridLayout;
    infoLayout->addWidget(roleClassLabel, 1, 1);
    infoLayout->addWidget(roleTeamLabel, 1, 2);
    infoLayout->addWidget(roleHPLabel, 2, 1);
    infoLayout->addWidget(roleATKLabel, 2, 2);
    infoLayout->addWidget(roleActionLabel, 3, 1, 1, 2);
    infoLayout->setVerticalSpacing(20);
    // infoLayout->addSpacing(10);
    infoLayout->addLayout(btnLayout, 4, 1, 1, 2);
}

void rightUi::initBattleRecord()
{
    battleRecordTitle = new QLabel("Battle Record:");
    battleRecordTitle->setFont(*LabelFont);

    battleRecordBox = new QPlainTextEdit;
    battleRecordBox->setFont(QFont("Calibri", 12));
    battleRecordBox->setFixedHeight(120);
    battleRecordBox->setReadOnly(true);

    btnClearBattleRecord = new QPushButton("Clear");
    btnClearBattleRecord->setFont(*SmallButtonFont);
    btnClearBattleRecord->setFixedSize(80, 50);

    battleRecordLayout = new QVBoxLayout;
    battleRecordLayout->addWidget(battleRecordTitle);
    battleRecordLayout->addWidget(battleRecordBox);
    battleRecordLayout->addWidget(btnClearBattleRecord);
}

void rightUi::initTurnController()
{
    TurnsNumberLabel = new QLabel;
    TurnsNumberLabel->setFont(*LabelFont);
    WhoseTurnLabel = new QLabel;
    WhoseTurnLabel->setFont(*LabelFont);
    btnEndTurn = new QPushButton("End This Turn");
    btnEndTurn->setFont(*LargeButtonFont);
    btnEndTurn->setFixedSize(220, 70);
    turnControllerLayout = new QVBoxLayout;
    turnControllerLayout->addWidget(TurnsNumberLabel);
    turnControllerLayout->addWidget(WhoseTurnLabel);
    turnControllerLayout->setSpacing(20);
    turnControllerLayout->addWidget(btnEndTurn);
    // TurnControllerLayout->setAlignment(Qt::AlignBottom);
}

//slots
void rightUi::RoleActionSelectionChanged(std::string status)
{
    // printf("Ui: received.\n");
    if(status == "ATTACK")
    {
        roleActionLabel->setText("Current action:\tAttack");
        roleActionLabel->setStyleSheet("QLabel { background-color : darkRed; color : white; }");
    }
    else if(status == "MOVE")
    {
        roleActionLabel->setText("Current action:\tMove");
        roleActionLabel->setStyleSheet("QLabel { background-color : darkGreen; color : white; }");
    }
}

void rightUi::RoleOnFocusChanged(RoleUnit* role)
{
    // printf("Ui: recieved role on focus.\n");
    if(role == nullptr)
    {
        roleTeamLabel->setText("Team:");
        roleClassLabel->setText("Class:");
        roleHPLabel->setText("HP:");
        roleATKLabel->setText("ATK:");
    }
    else
    {
        roleTeamLabel->setText("Team:  " + role->getTeam());
        roleClassLabel->setText("Class:  " + role->getClass());
        roleHPLabel->setText("HP:  " + QString::number(role->getHP()) + "/" + QString::number(role->getMaxHP()));
        std::cout << "HP:  " << role->getHP() << "/" << role->getMaxHP() << std::endl;
        roleATKLabel->setText("ATK:  " + QString::number(role->getATK()));
    }
}

void rightUi::BattleSystemMessageReceived(QString message)
{
    battleRecordBox->appendPlainText(message);
}

void rightUi::RoundChanged(QString team, int round) // from BattleSystem::RoundEnd(sendRoundStart)
{
    printf("\tUi: RoundChanged\n");
    WhoseTurnLabel->setText(team + "'s turn now.");
    TurnsNumberLabel->setText(QString("Round %1").arg(round));
}

//