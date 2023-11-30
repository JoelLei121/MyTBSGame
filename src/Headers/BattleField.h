#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "Headers/BattleSystem.h"
#include <QGraphicsView>
class GroundUnit;
class RoleUnit;
class BattleSystem;
class AnimationManager;


class BattleField : public QGraphicsView {
    Q_OBJECT
public:
    // about itself
    explicit BattleField(QGraphicsScene &scene, QWidget *parent = 0);
    void generateBattleField(const int size = 10);
    void generatePlayer(int n);
    void generateEnemy(int n);
    void generateObstacle(int type);
    void clearField();

    // battle system
    BattleSystem *m_BattleSystem;

    // about role
    void setRoleOnMap(RoleUnit* role, int x_idx, int y_idx);
    QVector<RoleUnit*> PlayerList;
    QVector<RoleUnit*> EnemyList;

    // about ground index
    QPointF getGPos(int x, int y);
    int getGCoorX(int x, int y);
    int getGCoorY(int x, int y);

protected:
    void keyPressEvent(QKeyEvent *event);
    // bool eventFilter(QObject *obj, QEvent *event);

private:
    // about ground unit
    GroundUnit** GroundArray;
    double m_BlockLenght;
    int m_MapSize;

    // about scene
    QGraphicsScene *m_Scene;

    // about role selecting
    std::string roleActionState; // ATTACK, MOVE
    bool checkIfActive(RoleUnit* role);
    void removeRole(RoleUnit* role);
    RoleUnit* roleOnFocus;
    GroundUnit* groundOnFocus;

    // about animation
    AnimationManager *RoleAnimationManager;

signals:
    void sendRoleOnFocus(RoleUnit*);
    void sendBattleSystemMessage(QString);

    void freezeUser(bool);

    void AIdone();

    // win and lose signal to battle system
    void playerAllDied();
    void enemyAllDied();
    void WinOrLose(QString msg);

public slots:
    // To get the index of selected ground unit
    void GroundSelectedChange(GroundUnit* block);
    void RoleSelectedChange(RoleUnit* role);
    void RoleActionSelectionChange(std::string status);
    void RoundChanged(QString team, int);

private slots:
    void setRoleRange(RoleUnit* role, std::string note);
    void RoleIsKilled(RoleUnit* target);

    friend class BattleSystem;
};

#endif