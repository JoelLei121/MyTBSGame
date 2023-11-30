#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H

#include <QObject>
#include <QVector>
#include <QPair>
class BattleField;
class RoleUnit;

class BattleSystem : public QObject
{
    Q_OBJECT

public:
    BattleSystem(QObject* parent = 0);
    int getRound(){ printf("turn: %d\n", m_dTurn); return m_dTurn / 2 + 1; }
    QString getWhoseTurn(){ return WhoseTurn; }
    void StartRound();
    void StartTurn(QString team);
    void startGame();

    // about ai
    void AutoPlay(RoleUnit* role, QVector<RoleUnit*> &targetTeam);

    // about map
    bool **ObstacleMap;
    void MapInit(BattleField* field);
    void printMap();
    void updateMap();

    QPair<int, int> checkNearbyBlock(RoleUnit* target);

private:
    BattleField* Field;
    int m_dSize;
    int m_dTurn;
    QString WhoseTurn;
    
signals:
    void sendRoundStart(QString, int);
    void sendBattleSystemMessage(QString);


public slots:
    void RoundEnd();

};

#endif