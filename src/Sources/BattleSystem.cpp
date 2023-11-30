#include "Headers/BattleSystem.h"
#include "Headers/BattleField.h"
#include "Headers/GroundUnit.h"
#include "Headers/RoleUnit.h"
#include "Headers/Algorithm.h"

#include <cstring>

extern void waiter(int time = 1000);

BattleSystem::BattleSystem(QObject* parent) : QObject(parent)
{
    WhoseTurn = "Player";
    m_dTurn = 0;
}

void BattleSystem::MapInit(BattleField* field)
{
    Field = field;
    m_dSize = field->m_MapSize;
    ObstacleMap = new bool*[m_dSize];
    for(int i = 0; i < m_dSize; ++i)
    {
        ObstacleMap[i] = new bool[m_dSize];
    }

    updateMap();
}

void BattleSystem::printMap()
{
    for(int i = 0; i < m_dSize; ++i)
    {
        for(int j = 0; j < m_dSize; ++j)
        {
            printf("%d\t", ObstacleMap[j][i]);
        }
        printf("\n\n");
    }
}

void BattleSystem::updateMap()
{
    if(Field != nullptr)
    {
        for(int i = 0; i < m_dSize; ++i)
        {
            ObstacleMap[i] = new bool[m_dSize];
            for(int j = 0; j < m_dSize; ++j)
            {
                ObstacleMap[i][j] = Field->GroundArray[i][j].isObstacle();
            }
        }

        for(auto item : Field->PlayerList)
        {
            int x = item->getCoorX(), y = item->getCoorY();
            ObstacleMap[x][y] = true;
        }
        for(auto item : Field->EnemyList)
        {
            int x = item->getCoorX(), y = item->getCoorY();
            ObstacleMap[x][y] = true;
        }
    }
}

void BattleSystem::startGame()
{
    emit sendBattleSystemMessage(QString("Game Start!"));
    emit sendRoundStart("Player", getRound());
}

void BattleSystem::AutoPlay(RoleUnit* role, QVector<RoleUnit*> &targetTeam)
{
    emit sendBattleSystemMessage("Enemy is moving.");

    RoleUnit* target = nullptr, *nearestTarget = nullptr;
    int MoveMin = 10e6, AtkMin = 10e6;
    for(auto item : targetTeam)
    {
        int range = role->getAtkRange();
        int dis = role->getDistance(item);
        printf("%d\t", dis);
        if(range >= role->getDistance(item))
        {
            if(dis < AtkMin)
            {
                AtkMin = dis;
                printf("%d %d\n", range, role->getDistance(item));
                target = item;
            }
        }
        if(dis < MoveMin)
        {
            MoveMin = dis;
            nearestTarget = item;
        }
    }
    printf("\n");
    // on target on range
    if(target == nullptr && nearestTarget != nullptr)
    {
        QPair<int, int> point = checkNearbyBlock(nearestTarget);
        if(point.first != -1 && point.second != -1)
        {
            // move to it
            printf("No target could attack; move.\n");
            emit role->statusGifRequest(role, "MOVE");
            PathfindingAlgorithm pf;
            pf.init(m_dSize, ObstacleMap);
            pf.setPoint(role->getCoorX(), role->getCoorY(), point.first, point.second);
            pf.FindPath();
            // pf.PrintPath();
            // going to near block
            for(int i = 0; i < ((role->getMoveRange() > pf.m_path.size()) ? pf.m_path.size() : role->getMoveRange()); ++i)
            {
                int x = pf.m_path[i].first;
                int y = pf.m_path[i].second;
                role->MoveTo(&(Field->GroundArray[x][y]));
                waiter(animation::TIME_PRE_BLOCK);
            }
            emit role->stopGif(role);
            role->setMoved(true);
        }
    }
    else
    {
        printf("could attack\n");
    }

    // attack
    for(auto item : targetTeam)
    {
        int range = role->getAtkRange();
        if(range >= role->getDistance(item))
        {
            printf("%d %d\n", range, role->getDistance(item));
            target = item;
        }
    }

    if(target != nullptr)
    {
        role->Attack(target);
    }
    Field->roleOnFocus = nullptr;
    emit Field->sendRoleOnFocus(Field->roleOnFocus);
    updateMap();
    // emit sendBattleSystemMessage("done");
}

QPair<int, int> BattleSystem::checkNearbyBlock(RoleUnit* target)
{
    // check if could go near
    for(int i = -1; i <=1 ; ++i)
    {
        for(int j = -1; j <= 1; ++j)
        {
            int x = target->getCoorX() + i;
            int y = target->getCoorY() + i;
            if(x >=0 && x < m_dSize && x >= 0 && x < m_dSize)
            {
                if(ObstacleMap[x][y] == false)
                {
                    //sucess
                    return QPair<int, int>(x, y);
                }
            }
        }
    }
    return QPair<int, int>(-1, -1);
}

// slots
void BattleSystem::RoundEnd()
{
    printf("\tBattleSystem: RoundEnd\n");
    ++m_dTurn;
    if(WhoseTurn == "Player")
    {
        WhoseTurn = "Enemy";
        emit sendBattleSystemMessage("Player's Turn End.");
        printf("Player's Turn End.");
        emit sendRoundStart("Enemy", getRound()); // to rightUi::RoundChanged, BattleField::RoundChanged
    }
    else if(WhoseTurn == "Enemy")
    {
        WhoseTurn = "Player";
        emit sendBattleSystemMessage("Enemy's Turn End.");
         printf("Enemy's Turn End.");
        emit sendRoundStart("Player", getRound()); // to rightUi::RoundChanged, BattleField::RoundChanged
    }
    updateMap();
}