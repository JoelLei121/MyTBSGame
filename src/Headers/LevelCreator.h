#ifndef LEVELCREATER_H
#define LEVELCREATER_H

#include "Headers/DataPackage.h"

#include <QMainWindow>
class BattleField;
class Level;

class LevelCreator : public QMainWindow
{
    Q_OBJECT

private:
    int playerGenerate;
    int enemyGenerate;
    int mapSize;
    int ObstacleType;
    const levelData::dataPack *PackageUsed;
    Level *currentLevel;
    void CreateLevel();

public:
    LevelCreator(QMainWindow *parent = 0);
    ~LevelCreator();

signals:
    void GameStart(Level*);
    void backToMenu();

public slots:
    void BulidLevelSelected(int i);
};

#endif