#include "Headers/BattleField.h"
#include "Headers/BattleSystem.h"
#include "Headers/Level.h"
#include "Headers/RightUi.h"
#include "Headers/RoleUnit.h"
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTimer>
#include <QSoundEffect>

Level::Level(QMainWindow *parent) : QMainWindow(parent)
{
    bgm = new QSoundEffect(this);
    bgm->setSource(QUrl::fromLocalFile(":/bgm/battle.wav"));
    bgm->setLoopCount(QSoundEffect::Infinite);
    bgm->setVolume(0.4);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(-400, -400, 800, 800);
    Field = new BattleField(*scene, this);
    Field->setScene(scene);
    Field->setBackgroundBrush(QPixmap(":/background/bg.png"));

    // QObject::connect(levelCreater, &LevelCreator::setField, Field, &BattleField::CreateBattleField);

    right = new rightUi(this);
    right->setFixedWidth(500);

    // role refresh
    QObject::connect(right, &rightUi::sendRoleAction, Field, &BattleField::RoleActionSelectionChange);
    QObject::connect(Field, &BattleField::sendRoleOnFocus, right, &rightUi::RoleOnFocusChanged);

    // send msg
    QObject::connect(Field, &BattleField::sendBattleSystemMessage, right, &rightUi::BattleSystemMessageReceived);
    QObject::connect(Field->m_BattleSystem, &BattleSystem::sendBattleSystemMessage, right, &rightUi::BattleSystemMessageReceived);

    // round controller
    QObject::connect(right, &rightUi::sendTurnEnding, Field->m_BattleSystem, &BattleSystem::RoundEnd);
    QObject::connect(Field->m_BattleSystem, &BattleSystem::sendRoundStart, right, &rightUi::RoundChanged);
    QObject::connect(Field->m_BattleSystem, &BattleSystem::sendRoundStart, Field, &BattleField::RoundChanged);

    // 
    QObject::connect(Field, &BattleField::WinOrLose, this, &Level::ExitGame);
    QObject::connect(Field, &BattleField::AIdone, right, rightUi::sendTurnEnding);
    QObject::connect(Field, &BattleField::freezeUser, this, &Level::StopEvent);

    // Field UI
    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(Field);
    mainLayout->addWidget(right);
    w = new QWidget(this);
    this->setCentralWidget(w);
    w->setLayout(mainLayout);
    
    QTimer::singleShot(1000, this, [&](){ bgm->play(); });
}

void Level::CreateBattleField(int player, int enemy, int map, int obstacle)
{
    Field->generateBattleField(map);
    Field->generatePlayer(player);
    Field->generateEnemy(enemy);
    Field->generateObstacle(obstacle);
    Field->m_BattleSystem->MapInit(Field);
}

void Level::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, tr("Exiting Level"),
                                                                tr("Exit?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        bgm->stop();
        this->hide();
        event->ignore();
        emit backToMenu();
    }
}

// slots
void Level::startLevel(Level* lv)
{
    if(lv == this)
    {
        this->show();
        Field->m_BattleSystem->startGame();
    }
}

void Level::ExitGame(QString msg)
{
    bgm->stop();
    bgm->setSource(QUrl::fromLocalFile(":/bgm/endgame.wav"));
    bgm->setLoopCount(QSoundEffect::Infinite);
    bgm->setVolume(0.9);
    QTimer::singleShot(200, this, [&](){ bgm->play(); });
    QMessageBox msgBox;
    msgBox.setStyleSheet("QLabel{min-width: 200px;}");
    msgBox.setText(msg);
    msgBox.setWindowTitle("Good Game!");
    msgBox.exec();
    StopEvent(true);
    this->close();
}