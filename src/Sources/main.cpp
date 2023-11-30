#include "Headers/MenuUi.h"
#include "Headers/LevelUi.h"
#include "Headers/LevelCreator.h"
#include "Headers/tutorial.h"
#include <QApplication>

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QSoundEffect>

// int side = 90;
// void BuildLevel();

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // about level
    LevelCreator *levelCreator = new LevelCreator;

    MenuUi *menu = new MenuUi;
    LevelUi *level = new LevelUi;
    Tutorial *tutorial = new Tutorial;

    QObject::connect(menu, MenuUi::SelectLevel, [=](){ level->show(); menu->hide();});
    QObject::connect(menu, MenuUi::Tutorial, [=](){ tutorial->show(); menu->hide();});
    QObject::connect(tutorial, Tutorial::backToMenu, menu, &MenuUi::backToMenu);
    
    QObject::connect(levelCreator, &LevelCreator::backToMenu, menu, &MenuUi::backToMenu);
    QObject::connect(level, &LevelUi::backToMenu, menu, &MenuUi::backToMenu);
    QObject::connect(level, LevelUi::sendLevelCreationRequset, levelCreator, LevelCreator::BulidLevelSelected);
    QObject::connect(level, LevelUi::sendLevelCreationRequset, [&menu](){menu->bgm->stop(); });

    menu->bgm->play();
    menu->show();
    return app.exec();
}
