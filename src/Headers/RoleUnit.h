// TODO: 
// 1. getpos(): return a pos so that the character could stand on the ground unit
// 2. draw or update a status bar to show details of characters
// 3. a container for pictures or gifs
// 4. a status class for storing data
// 5. the size must be affected by the size of ground unit

#ifndef ROLEUNIT_H
#define ROLEUNIT_H

#include "Headers/RoleUnitStatus.h"
#include <QGraphicsObject>


class RoleUnit : public QGraphicsObject{
    Q_OBJECT
private:
    // about size
    double m_dWidth;
    double m_dHeight;
    QRect m_Fill;
    QPixmap *pixmap;

    // about data
    RoleUnitData m_data;

    // about coordinate
    int m_CoorX;
    int m_CoorY;

    // about battle
    bool m_bMoved;
    bool m_bAttacked;

public:
    // about painting
    RoleUnit(characterStatus::dataPack dp, double width, double height);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // about position
    void printPos(){ printf("(%d, %d)\n", static_cast<int>(x()), static_cast<int>(y())); }

    // about coordinate
    int getCoorX(){ return m_CoorX; }
    int getCoorY(){ return m_CoorY; }
    void setOnMap(QPointF pos, int index_x, int index_y);
    void MoveTo(GroundUnit *block);

    // about data
    QString getTeam(){ return QString::fromStdString((m_data.getTeam())); }
    QString getClass(){ return QString::fromStdString(m_data.getClass()); }
    int getMaxHP(){ return m_data.getMaxHP(); }
    int getHP(){ return m_data.getHP(); }
    int getATK(){ return m_data.getAttackDamage(); }
    int getAtkRange(){ return m_data.AttackRange(); }
    int getMoveRange(){ return m_data.MoveRange(); }
    int getDistance(RoleUnit* role);

    // about battle
    bool isMoved(){ return m_bMoved; }
    bool isAttacked(){ return m_bAttacked; }
    void setMoved(bool t){ m_bMoved = t; update();}
    void setAttacked(bool t){ m_bAttacked = t; update(); }
    
    // about attacking
    void damaged(int attackDamage);
    bool checkIfDead();

    // about AI
    void Attack(RoleUnit* target);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void RoleIsSelected(RoleUnit*);
    // about animation
    void statusGifRequest(RoleUnit*, QString);
    void stopGif(RoleUnit*);
    void KillRole(RoleUnit*);

public slots:
    void Iced();
    void TurnStartInit();
    // for png
    void PngUpdate(RoleUnit* role, QString pic);
    // for gif
    void GifUpdate(RoleUnit* role, QPixmap pic);

    friend class BattleField;
};


// Hint: create dialog
#endif


// 1. eventFilter on scene
// 2. when click, get Role
// 3. 