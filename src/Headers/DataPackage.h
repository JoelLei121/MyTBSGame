#ifndef DATAPACKAGE_H
#define DATAPACKAGE_H

#include <QColor>
#include <QPointF>
#include <QPixmap>

class GroundUnit;

namespace colorUsed
{
    // Ground: None, onRange, Player, Enemy, RoadBlock
    // brust
    const QColor Player_Brust = Qt::blue;
    const QColor None_Brust = QColor(0, 0, 0, 0);
    const QColor MoveRange_Brust =  Qt::green;
    const QColor AttackRange_Brust =  QColor(255, 0, 0, 200);
    const QColor Enemy_Brust = QColor(250, 10, 10, 200);
    const QColor Selected_Brust = QColor(0, 179, 218, 255);
    const QColor Obstacle_Brust = QColor(240, 164, 54, 255);
    // pen
    const QColor Selected_Pen = Qt::black;
    const QColor None_Pen = Qt::white;
}

namespace levelData
{
    class dataPack
    {
    public:
        int PlayerNumber;
        int EnemyNumber;
        int ObstacleType;
        int mapSize;
        dataPack(int pn, int en, int ot, int mp = 10) : PlayerNumber(pn), EnemyNumber(en), mapSize(mp), ObstacleType(ot){}
    };

    const dataPack level_1 = dataPack(2, 3, 1, 8);
    const dataPack level_2 = dataPack(5, 5, 2, 10);
}

namespace characterStatus
{
    namespace skill{}

    class dataPack{
    public:
        int MaxHealth;
        int attackDamage;
        int moveRange;
        int attackRange;
        std::string team;
        std::string classType;
        char* picSource; 
        dataPack(int h, int ad, int m, int ar, std::string t, std::string ct, char* mp) : MaxHealth(h), attackDamage(ad), moveRange(m), 
            attackRange(ar), team(t), classType(ct), picSource(mp){}
        
    };

    const dataPack soldier = dataPack(60, 25, 3, 2, "Player", "soldier", ":/characters/soldier/stand.png");
    const dataPack archer = dataPack(30, 25, 4, 3, "Player", "archer", ":/characters/archer/stand.png");
    const dataPack skeleton = dataPack(25, 25, 2, 2, "Enemy", "skeleton", ":/characters/skeleton/stand.png");
}

namespace FieldData
{
    const double lenght = 70;
    const int size = 10; // not used
}

namespace animation
{
    const int TIME_PRE_BLOCK = 200;
    const int ATTACK_DELAY = 1200;
    const int DAMAGED_DELAY = 400;
    const int DIE_DELAY = 800;
}


#endif