#ifndef ANIMATION_H
#define ANIMATION_H

#include <QPropertyAnimation>
#include <QMovie>
#include <QPixmap>
#include <QEventLoop>

class RoleUnit;
class QMovie;

class AnimationManager : public QObject{
    Q_OBJECT
    // Q_PROPERTY(QRect geometry READ geometry WRITE setGeometry)

private:
    QEventLoop* waiter; 
    QMovie* gif_container;
    RoleUnit* RoleInteracting;
    QPixmap* currentPixmap;
    QString InteractingStatus;

    // load gif by gif_container, motivated by StatusRequest
    void loadGif(QString address);

public:
    AnimationManager(QObject* parent = 0);
    ~AnimationManager();
    // QPropertyAnimation attackAnimation;
    

signals:
    // return pixmap
    void sendUpdatePng(RoleUnit*, QString);
    void sendUpdateGif(RoleUnit*, QPixmap);

public slots:
    // status: MOVE, STAND, ATTACK, DIE
    void StatusRequest(RoleUnit* role, QString status);
    // stop automatically
    void StopGif(RoleUnit* role);

};

#endif