#include "Headers/AnimationManager.h"
#include "Headers/RoleUnit.h"



AnimationManager::AnimationManager(QObject* parent) : QObject(parent)
{
    gif_container = new QMovie;
    currentPixmap = new QPixmap;
    waiter = new QEventLoop;
    connect(gif_container, &QMovie::finished, [=](){ /*printf("animation stopped!\n");*/ StopGif(RoleInteracting); }); // get role
}

AnimationManager::~AnimationManager()
{
    delete gif_container;
    delete currentPixmap;
}

// role gif
void AnimationManager::StatusRequest(RoleUnit* role, QString status)
{
    RoleInteracting = role;
    QString s;
    QString roleClass = role->getClass();
    s += ":/characters/";
    s += roleClass;
    // if(roleClass == "soldier")
    // {
    //     s +=":/characters/soldier";
    // }
    // else if(role->getClass() == "archer")
    // {
    //     s += ":/characters/archer";
    // }
    // else if(roleClass == "skeleton")
    // {
    //     s += ":/characters/skeleton";
    // }
    currentPixmap->load(s + "/stand.png");

    InteractingStatus = status;
    if(status == "MOVE")
    {
        s += "/move.gif";
        gif_container->setSpeed(150);
    }
    else if(status == "ATTACK")
    {
        s += "/attack.gif";
        gif_container->setSpeed(100);
    }
    else if(status == "DIE")
    {
        s += "/die.gif";
        gif_container->setSpeed(100);
    }
    loadGif(s);
}

void AnimationManager::loadGif(QString address)
{
    gif_container->setFileName(address);
    connect(gif_container, &QMovie::updated, [=](){ emit sendUpdateGif(RoleInteracting, gif_container->currentPixmap()); });
    gif_container->start();
    // printf("start animation!\n");
    
}

void AnimationManager::StopGif(RoleUnit* role)
{
    // how to deal with die situation?
    gif_container->stop();
    disconnect(gif_container, &QMovie::updated, nullptr, nullptr);
    QString s;
    s += ":/characters/";
    s += role->getClass();
    // if(role->getClass() == "soldier")
    // {
    //     s +=":/characters/soldier";
    // }
    // else if(role->getClass() == "archer")
    // {
    //     s += ":/characters/archer";
    // }
    // else if(role->getClass() == "skeleton")
    // {
    //     s += ":/characters/skeleton";
    // }
    if(InteractingStatus != "DIE")
    {
        s += "/stand.png";
    }
    else
    {
        s += "/dead.png";
    }
    emit sendUpdatePng(RoleInteracting, s);
    RoleInteracting = nullptr;
}