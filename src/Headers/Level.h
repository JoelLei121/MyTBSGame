#ifndef LEVEL_H
#define LEVEL_H

#include <QWidget>
#include <QMainWindow>
class BattleField;
class QGraphicsScene;
class rightUi;
class QHBoxLayout;
class QCloseEvent;
class QSoundEffect;

class Level : public QMainWindow
{
    Q_OBJECT
private:
    BattleField *Field;
    QGraphicsScene *scene;
    rightUi *right;
    QHBoxLayout *mainLayout;
    QWidget *w;
public:
    Level(QMainWindow *parent = 0);
    void CreateBattleField(int player, int enemy, int map, int obstacle);
    QSoundEffect *bgm;
    
protected:
    void closeEvent(QCloseEvent *event);

signals:
    void backToMenu();

public slots:
    void startLevel(Level* lv);
    void StopEvent(bool t){this->setEnabled(!t);}

private slots:
    void ExitGame(QString msg);
};

#endif