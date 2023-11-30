#ifndef MENUUI_H
#define MENUUI_H

#include <QMainWindow>
class QGraphicsScene;
class QGraphicsView;
class QLabel;
class QPushButton;
class QGridLayout;
class QVBoxLayout;
class QSoundEffect;

class MenuUi : public QMainWindow
{
    Q_OBJECT

public:
    explicit MenuUi(QMainWindow *parent = nullptr);
    QSoundEffect *bgm;

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QImage *image;
    QBrush *brush;

    QLabel *WelcomeLabel;
    QPushButton *btnStartGame;
    QPushButton *btnTutorial;
    QPushButton *btnExit;
    QVBoxLayout *layout;

    QFont *LabelFont;
    QFont *ButtonFont;

signals:
    void SelectLevel();
    void Tutorial();

public slots:
    void backToMenu();

};

#endif