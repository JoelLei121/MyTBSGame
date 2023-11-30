#include "Headers/LevelCreator.h"
#include "Headers/Level.h"
#include "Headers/BattleField.h"
#include <QTimer>

LevelCreator::LevelCreator(QMainWindow *parent) : QMainWindow(parent)
{

}

LevelCreator::~LevelCreator()
{
}

void LevelCreator::BulidLevelSelected(int i)
{
    switch (i)
    {
        case 1:
        {
            PackageUsed = &levelData::level_1;
            break;
        }
        
        case 2:
        default:
        {
            PackageUsed = &levelData::level_2;
            break;
        }
    }

    playerGenerate = PackageUsed->PlayerNumber;
    enemyGenerate = PackageUsed->EnemyNumber;
    ObstacleType = PackageUsed->ObstacleType;
    mapSize = PackageUsed->mapSize;
    CreateLevel();
}

void LevelCreator::CreateLevel()
{
    currentLevel = new Level;
    currentLevel->CreateBattleField(playerGenerate, enemyGenerate, mapSize, ObstacleType);
    connect(this, &LevelCreator::GameStart, currentLevel, &Level::startLevel);
    connect(currentLevel, &Level::backToMenu, this, &LevelCreator::backToMenu);
    QTimer::singleShot(100, this, [=](){ emit GameStart(currentLevel); });
}
