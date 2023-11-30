#include "Headers/BattleField.h"
#include "Headers/GroundUnit.h"
#include "Headers/RoleUnit.h"
#include "Headers/BattleSystem.h"
#include "Headers/DataPackage.h"
#include "Headers/Algorithm.h"
#include "Headers/AnimationManager.h"

#include <QKeyEvent>
#include <QThread>
#include <QEventLoop>
#include <QTimer>

void waiter(int time = 1000)
{
    QEventLoop *eventloop = new QEventLoop;
    QTimer::singleShot(time, eventloop, &QEventLoop::quit);
    eventloop->exec();
}

BattleField::BattleField(QGraphicsScene &scene, QWidget *parent) :
    QGraphicsView(parent), m_Scene(&scene), roleOnFocus(nullptr), groundOnFocus(nullptr) 
{
    m_BlockLenght = FieldData::lenght;
    roleActionState = "MOVE";
    m_BattleSystem = new BattleSystem;
    connect(this, &BattleField::playerAllDied, [=](){ emit sendBattleSystemMessage("You Lose!"); emit WinOrLose("You Lose!"); });
    connect(this, &BattleField::enemyAllDied, [=](){ emit sendBattleSystemMessage("You Win!"); emit WinOrLose("You Win!"); });

    RoleAnimationManager = new AnimationManager;

}


void BattleField::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Plus :
        scale(1.2, 1.2);
        break;
    case Qt::Key_Minus :
        scale(1 / 1.2, 1 / 1.2);
        break;
    }
    QGraphicsView::keyPressEvent(event);
}



// Generating stuffs
void BattleField::generateBattleField(const int size)
{
    int space = 0.5;
    m_MapSize = size;
    double offset = (m_MapSize * m_BlockLenght + (m_MapSize-1)*space) / 2 - m_BlockLenght / 2;
    GroundArray = new GroundUnit*[m_MapSize];
    for(int i = 0; i < m_MapSize; ++i)
    {
        GroundArray[i] = new GroundUnit[m_MapSize];
    }

    for(int i = 0; i < m_MapSize; ++i)
    {
        for(int j = 0; j < m_MapSize; ++j)
        {
            GroundArray[i][j].setColor(colorUsed::None_Brust, colorUsed::None_Pen);
            // TODO: Set field to middle and autosize
            // Note: affected by scene
            
            double x = i * (m_BlockLenght + space) - offset;
            double y = j * (m_BlockLenght + space) - offset;
            GroundArray[i][j].setPos(x, y);
            GroundArray[i][j].setCoor(i, j);
            GroundArray[i][j].setZValue(-1);
            // groundCoor::GroundArray[i][j].init(item, i, j);
            // connect each item to battleField
            connect(&GroundArray[i][j], &GroundUnit::GroundIsSelected, this, &BattleField::GroundSelectedChange);
            m_Scene->addItem(&GroundArray[i][j]);
        }
    }
}

void BattleField::generatePlayer(int n)
{
    for(int i = 0; i < n; ++i)
    {
        RoleUnit *role;
        if(i%3 == 0)
        {
            role = new RoleUnit(characterStatus::archer, m_BlockLenght, m_BlockLenght);
        }
        else
        {
            role = new RoleUnit(characterStatus::soldier, m_BlockLenght, m_BlockLenght);
        }
        role->setOnMap(GroundArray[0][i].pos(), GroundArray[0][i].getCoorX(), GroundArray[0][i].getCoorY());
        GroundArray[0][i].getCoorInfo();
        // role.installEventFilter(this);
        role->setZValue(1);
        // connect each role to battleField
        connect(role, &RoleUnit::RoleIsSelected, this, &BattleField::RoleSelectedChange);
        connect(role, &RoleUnit::KillRole, this, &BattleField::RoleIsKilled);
        // connect to animation
        connect(role, &RoleUnit::statusGifRequest, RoleAnimationManager, &AnimationManager::StatusRequest);
        connect(role, &RoleUnit::stopGif, RoleAnimationManager, &AnimationManager::StopGif);
        connect(RoleAnimationManager, &AnimationManager::sendUpdatePng, role, &RoleUnit::PngUpdate);
        connect(RoleAnimationManager, &AnimationManager::sendUpdateGif, role, &RoleUnit::GifUpdate);

        m_Scene->addItem(role);
        PlayerList.push_back(role);
    }
    return;
}

void BattleField::generateEnemy(int n)
{
    for(int i = 0; i < n; ++i)
    {
        RoleUnit *role = new RoleUnit(characterStatus::skeleton, m_BlockLenght, m_BlockLenght);
        role->setOnMap(GroundArray[m_MapSize-1][m_MapSize-1-i].pos(), GroundArray[m_MapSize-1][m_MapSize-1-i].getCoorX(), GroundArray[m_MapSize-1][m_MapSize-1-i].getCoorY());
        // role.installEventFilter(this);
        role->setZValue(1);
        // connect each role to battleField
        connect(role, &RoleUnit::RoleIsSelected, this, &BattleField::RoleSelectedChange);
        connect(role, &RoleUnit::KillRole, this, &BattleField::RoleIsKilled);
                // connect to animation
        connect(role, &RoleUnit::statusGifRequest, RoleAnimationManager, &AnimationManager::StatusRequest);
        connect(role, &RoleUnit::stopGif, RoleAnimationManager, &AnimationManager::StopGif);
        connect(RoleAnimationManager, &AnimationManager::sendUpdatePng, role, &RoleUnit::PngUpdate);
        connect(RoleAnimationManager, &AnimationManager::sendUpdateGif, role, &RoleUnit::GifUpdate);
        m_Scene->addItem(role);
        EnemyList.push_back(role);
    }
}

void BattleField::generateObstacle(int type)
{
    switch (type)
    {
        case 1:
        {
            for(int i = 2; i < m_MapSize - 2; ++i)
            {
                GroundArray[i][i].setObstacle(true);
                GroundArray[i][m_MapSize-i-1].setObstacle(true);
                GroundArray[i][i].update();
                GroundArray[i][m_MapSize-i-1].update();
            }
        }
        break;
        
        case 2:
        default:
        {
            if(m_MapSize > 6)
            {
                for(int i = 2; i < m_MapSize - 2; ++i)
                {
                    GroundArray[3][i].setObstacle(true);
                    GroundArray[m_MapSize - 3 - 1][i].setObstacle(true);
                    GroundArray[3][i].update();
                    GroundArray[m_MapSize - 3 - 1][i].update();
                }
            }
        }
        break;
        
    }
}
// End of Generating stuffs

// bool BattleField::eventFilter(QObject *obj, QEvent *event)
// {
//     return false;
// }

void BattleField::clearField()
{
    for(int i = 0; i < m_MapSize; ++i)
    {
        for(int j = 0; j < m_MapSize; ++j)
        {
            GroundArray[i][j].setColor(colorUsed::None_Brust, colorUsed::None_Pen);
            GroundArray[i][j].update();
        }
    }
    return;
}

// about role movement
void BattleField::setRoleOnMap(RoleUnit* role, int x_idx, int y_idx)
{
    // msg
    emit sendBattleSystemMessage(QString("%1: %2 (%3, %4) is now move to (%5, %6)!").arg(role->getTeam()).arg(role->getClass()).arg(role->getCoorX()).arg(role->getCoorY()).arg(x_idx).arg(y_idx));
    // call function
    // try of pathfinding
    PathfindingAlgorithm pf;
    pf.init(m_MapSize, m_BattleSystem->ObstacleMap);
    pf.setPoint(role->getCoorX(), role->getCoorY(), x_idx, y_idx);
    pf.FindPath();
    pf.PrintPath();

    if(!pf.m_path.isEmpty())
    {
        for(int i = 0; i < pf.m_path.size(); ++i)
        {
            int x = pf.m_path[i].first;
            int y = pf.m_path[i].second;
            role->MoveTo(&GroundArray[x][y]);
            waiter(animation::TIME_PRE_BLOCK);
        }
        emit role->stopGif(role);
        role->setMoved(true);
    }
    // role->setOnMap(GroundArray[x_idx][y_idx].pos(), GroundArray[x_idx][y_idx].getCoorX(), GroundArray[x_idx][y_idx].getCoorY());
}

QPointF BattleField::getGPos(int x, int y)
{
    return GroundArray[x][y].pos();
}

int BattleField::getGCoorX(int x, int y)
{
    return GroundArray[x][y].getCoorX();
}

int BattleField::getGCoorY(int x, int y)
{
    return GroundArray[x][y].getCoorY();
}
// end of role movement

// about attacking
// check if role moved or attacked, including sending deny msg
bool BattleField::checkIfActive(RoleUnit* role)
{
    bool tmp = (roleActionState == "ATTACK" ? role->isAttacked() : role->isMoved());
    
    if(tmp == false)
    {
        return true;
    }
    else
    {
        emit sendBattleSystemMessage( QString("This role is unable to %1!").arg(QString::fromStdString(roleActionState)));
        return false;
    }
}

void BattleField::removeRole(RoleUnit* role)
{
    if(role->getTeam() == "Player")
    {
        int i = PlayerList.indexOf(role);
        if(i != -1)
        {
            delete role;
            PlayerList.remove(i);
        }
        else
        {
            emit sendBattleSystemMessage("ERROR! Player NOT FOUND!");
        }
    }
    else if(role->getTeam() == "Enemy")
    {
        int i = EnemyList.indexOf(role);
        if(i != -1)
        {
            delete role;
            EnemyList.remove(i);
        }
        else
        {
            emit sendBattleSystemMessage("ERROR! Enemy NOT FOUND!");
        }
    }
    // check if win or lose
    if(PlayerList.isEmpty())
    {
        emit playerAllDied();
    }
    if(EnemyList.isEmpty())
    {
        emit enemyAllDied();
    }
}


// slots
// execute when ground is clicked
void BattleField::GroundSelectedChange(GroundUnit* block)
{
    printf("block (%d, %d) is clicked.\n", block->getCoorX(), block->getCoorY());
    printf("pos: (%d, %d)\n", static_cast<int>(block->pos().x()), static_cast<int>(block->pos().y()));
    // emit sendBattleSystemMessage( QString("block (%1, %2) is clicked. "
    // "pos: (%3, %4)").arg(block->getCoorX()).arg(block->getCoorY()).arg(static_cast<int>(block->pos().x())).arg(static_cast<int>(block->pos().y())) );
    
    QColor blockColor = block->getBrust();
    if(blockColor == colorUsed::MoveRange_Brust)
    {
        //block is on move range
        freezeUser(true);
        clearField();

        emit roleOnFocus->statusGifRequest(roleOnFocus, "MOVE");
        setRoleOnMap(roleOnFocus, block->getCoorX(), block->getCoorY());

        waiter(animation::TIME_PRE_BLOCK * (abs(roleOnFocus->getCoorX() - block->getCoorX()) + abs(roleOnFocus->getCoorY() - block->getCoorY())));
        freezeUser(false);
        // roleOnFocus = nullptr;
        // emit sendRoleOnFocus(roleOnFocus);
    }
    else if(blockColor == colorUsed::None_Brust)
    {
        clearField();
        roleOnFocus = nullptr;
        emit sendRoleOnFocus(roleOnFocus);
    }
    groundOnFocus = block;
    m_BattleSystem->updateMap();
}

// execute when role is clicked
void BattleField::RoleSelectedChange(RoleUnit* role)
{
    // printf("role (%d, %d) is selected.\n", role->getCoorX(), role->getCoorY());

    if(roleOnFocus != nullptr)
    {
        if(roleOnFocus->getTeam() == role->getTeam())
        {
            // same team
            if(checkIfActive(role))
            {
                clearField();
                // printf("selecting a teammate.\n");
                roleOnFocus = role;
                setRoleRange(role, roleActionState);
                // msg
                emit sendBattleSystemMessage( QString("%1: %2 (%3, %4) is selected.").arg(roleOnFocus->getTeam()).arg(roleOnFocus->getClass()).arg(roleOnFocus->getCoorX()).arg(roleOnFocus->getCoorY()) );
            }            
        }
        else if(roleActionState == "ATTACK")
        {
            if(checkIfActive(roleOnFocus))
            {
                // attacking
                int del_x = abs(role->getCoorX() - roleOnFocus->getCoorX());
                int del_y = abs(role->getCoorY() - roleOnFocus->getCoorY());
                if(roleOnFocus->m_data.AttackRange() >= del_x + del_y)
                {
                    printf("you are trying to attack.\n");
                    freezeUser(true);
                    clearField();
                    emit sendBattleSystemMessage( QString("%1: %2 (%3, %4) Attack!").arg(roleOnFocus->getTeam()).arg(roleOnFocus->getClass()).arg(roleOnFocus->getCoorX()).arg(roleOnFocus->getCoorY()) );

                    // attack animation for attacker
                    roleOnFocus->Attack(role);
                    // emit roleOnFocus->statusGifRequest(roleOnFocus, "ATTACK");
                    // // with finished
                    // roleOnFocus->setAttacked(true);
                    // printf("attack!");

                    // // attacked
                    // role->damaged(roleOnFocus->m_data.getAttackDamage()); 
                    // waiter(animation::DAMAGED_DELAY);
                    // if(role->checkIfDead())
                    // {
                    //     emit role->statusGifRequest(role, "DIE");
                    //     emit sendBattleSystemMessage(role->getClass() + " was Killed!");
                    //     waiter(animation::DIE_DELAY);
                    //     removeRole(role);
                    // }
                    // roleOnFocus = nullptr;
                    // emit sendRoleOnFocus(roleOnFocus);
                    
                    freezeUser(false);
                }
                else
                {
                    printf("Too far to attack!\n");
                    emit sendBattleSystemMessage( QString("Too far to attack!"));
                }
            }
        }
    }
    else
    {
        printf("first!\n");
        // init
        if(role->getTeam() != m_BattleSystem->getWhoseTurn())
        {
            emit sendRoleOnFocus(role);
            roleOnFocus = nullptr;
            return;
        }
        else if(checkIfActive(role))
        {
            roleOnFocus = role;
            setRoleRange(role, roleActionState);
            // msg
            emit sendBattleSystemMessage( QString("%1: %2 (%3, %4) is selected.").arg(roleOnFocus->getTeam()).arg(roleOnFocus->getClass()).arg(role->getCoorX()).arg(role->getCoorY()) );
        }
    }
    emit sendRoleOnFocus(roleOnFocus); 
    m_BattleSystem->updateMap();
}

// receive signal from ui: btnAttack and btnMove
void BattleField::RoleActionSelectionChange(std::string status)
{
    roleActionState = status;
    clearField();
    if(roleOnFocus != nullptr && checkIfActive(roleOnFocus))
    {
        setRoleRange(roleOnFocus, roleActionState);
    }
    else
    {
        // to prevent invisible player action
        roleOnFocus = nullptr;
    }
    if(status == "ATTACK")
    {
        printf("BattleField: ATTACK!\n");
        emit sendBattleSystemMessage(QString("You choose to ATTACK!"));
    }
    else if(status == "MOVE")
    {
        printf("BattleField: MOVE!\n");
        emit sendBattleSystemMessage(QString("You choose to MOVE!"));
    }
}

void BattleField::setRoleRange(RoleUnit* role, std::string note)
{
    if(role == nullptr) return;
    // printf("working at (%d, %d)\n", x, y);
    int x = role->getCoorX(), y = role->getCoorY();
    int range = (note == "ATTACK" ? role->m_data.AttackRange() : role->m_data.MoveRange());
    QColor brust = (note == "ATTACK" ? colorUsed::AttackRange_Brust : colorUsed::MoveRange_Brust);

    int xTail = (x + range >= m_MapSize ? m_MapSize - 1 : x + range);
    int yTail = (y + range >= m_MapSize ? m_MapSize - 1 : y + range);

    for(int i = (x - range > 0 ? x - range : 0); i <= xTail; ++i)
    {
        for(int j = (y - range > 0 ? y - range : 0); j <= yTail; ++j)
        {
            int w = abs(x - i), h = abs(y - j);
            // printf("now: (%d, %d), %d\n", i, j, w + h);
            if(w + h <= range){
                GroundArray[i][j].setColor(brust, colorUsed::Selected_Pen);
                GroundArray[i][j].update();
            }
        }
    }
}

void BattleField::RoleIsKilled(RoleUnit* target)
{
    // ai
    emit target->statusGifRequest(target, "DIE");
    emit sendBattleSystemMessage(target->getClass() + " was Killed!");
    waiter(animation::DIE_DELAY);
    removeRole(target);
    roleOnFocus = nullptr;
    emit sendRoleOnFocus(roleOnFocus);
}

void BattleField::RoundChanged(QString team, int) // from BattleSystem::RoundEnd
{
    printf("\tBattleField: RoundChanged\n");
    // clear focus
    roleOnFocus = nullptr;
    clearField();
    emit sendRoleOnFocus(roleOnFocus);

    // init role
    if(team == "Player")
    {
        for(auto item : PlayerList)
        {
            item->TurnStartInit();
        }
        for(auto item : EnemyList)
        {
            item->Iced();
        }
    }
    else if(team == "Enemy")
    {
        for(auto item : EnemyList)
        {
            item->TurnStartInit();
        }
        for(auto item : PlayerList)
        {
            item->Iced();
        }
        if(!EnemyList.isEmpty())
        {
            freezeUser(true);
            for(auto enemy : EnemyList)
            {
                m_BattleSystem->AutoPlay(enemy, PlayerList);
                waiter(500);
            }
            freezeUser(false);
            emit AIdone();
        }
    }
}

//