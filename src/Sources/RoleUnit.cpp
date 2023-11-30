#include "Headers/RoleUnit.h"
#include "Headers/DataPackage.h"
#include "Headers/GroundUnit.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QGraphicsEffect>

#include <QEventLoop>
#include <QTimer>
#include <QSoundEffect>

extern void waiter(int time = 1000);



// initializing
RoleUnit::RoleUnit(characterStatus::dataPack dp, double width, double height) : m_data(dp), m_dWidth(width), m_dHeight(height)
{
    m_Fill = QRect(-((m_dWidth)/2), -((m_dHeight)/2), (m_dWidth), (m_dHeight));
    pixmap = new QPixmap(width, height);
    pixmap->load(dp.picSource);
    pixmap->rect();

    // connect to animation
}

QRectF RoleUnit::boundingRect() const
{
    qreal adjust = 1;
    return QRectF(-(m_dWidth/2) - adjust, -(m_dHeight/2) - adjust, m_dWidth + adjust, m_dHeight + adjust);
}

void RoleUnit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // local pos
    painter->drawPixmap(m_Fill, *pixmap);
    if(m_bAttacked && m_bMoved)
    {
        painter->setBrush(QColor(0, 0, 0, 100));
        painter->drawRect(m_Fill);
    }
}
// end of initializing

// event
void RoleUnit::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit(RoleIsSelected(this));
    // emit statusGifRequest(this, "ATTACK");
    event->accept();
}
// end of event

// position setting
void RoleUnit::setOnMap(QPointF pos, int index_x, int index_y)
{
    setPos(pos);
    m_CoorX = index_x;
    m_CoorY = index_y;
}

void RoleUnit::MoveTo(GroundUnit *block)
{
    QPropertyAnimation *moving = new QPropertyAnimation(this, "pos");
    moving->setStartValue(this->pos());
    moving->setEndValue(block->pos());
    moving->setDuration(animation::TIME_PRE_BLOCK * (abs(m_CoorX - block->getCoorX()) + abs(m_CoorY - block->getCoorY())) );
    connect(moving, &QPropertyAnimation::finished, [=](){ delete moving; } );
    // it could cause a error if not release pointer
    moving->start();
    m_CoorX = block->getCoorX();
    m_CoorY = block->getCoorY();
    // setOnMap(block->pos(), block->getCoorX(), block->getCoorY());
}
// end of position setting

// about data

// about battle
void RoleUnit::damaged(int attackDamage)
{
    // do some animation
    int x = pos().x(), y = pos().y();
    QSoundEffect *hurtSound = new QSoundEffect(this);
    hurtSound->setSource(QUrl::fromLocalFile(":/bgm/hurt.wav"));
    hurtSound->setLoopCount(1);
    hurtSound->setVolume(0.8);
    hurtSound->play();


    QPropertyAnimation *shock = new QPropertyAnimation(this, "opacity");
    shock->setKeyValueAt(0, 1);

    shock->setKeyValueAt(0.1, 1);
    shock->setKeyValueAt(0.11, 0);
    shock->setKeyValueAt(0.24, 0);
    shock->setKeyValueAt(0.25, 1);

    shock->setKeyValueAt(0.4, 1);
    shock->setKeyValueAt(0.41, 0);
    shock->setKeyValueAt(0.54, 0);
    shock->setKeyValueAt(0.55, 1);

    shock->setKeyValueAt(0.7, 1);
    shock->setKeyValueAt(0.71, 0);
    shock->setKeyValueAt(0.84, 0);
    shock->setKeyValueAt(0.85, 1);

    shock->setKeyValueAt(1, 1);
    shock->setDuration(animation::DAMAGED_DELAY);
    shock->start();
    connect(shock, &QPropertyAnimation::finished, [=](){ delete shock; } );
    m_data.damaged(attackDamage);
    
}

bool RoleUnit::checkIfDead()
{
    if(m_data.isDead())
    {
        // die animation here
        // statusGifRequest(this, "DIE");
        return true;
    }
    else
    {
        return false;
    }
}

int RoleUnit::getDistance(RoleUnit* role)
{
    return (abs(m_CoorX - role->getCoorX()) + abs(m_CoorY - role->getCoorY()));
}

void RoleUnit::Attack(RoleUnit* target)
{
    QSoundEffect *atkSound = new QSoundEffect(this);
    atkSound->setSource(QUrl::fromLocalFile(":/bgm/attack.wav"));
    atkSound->setLoopCount(1);
    atkSound->setVolume(0.8);
    atkSound->play();

    // attack animation for attacker
    emit statusGifRequest(this, "ATTACK");
    waiter(animation::ATTACK_DELAY);
    // with finished
    setAttacked(true);
    printf("attack!");

    // attacked
    target->damaged(this->m_data.getAttackDamage()); 
    waiter(animation::DAMAGED_DELAY);  


    if(target->checkIfDead())
    {
        emit KillRole(target);
    }
    
}


// slots
void RoleUnit::TurnStartInit()
{
    m_bMoved = false;
    m_bAttacked = false;
}

void RoleUnit::Iced()
{
    m_bMoved = true;
    m_bAttacked = true;
}

void RoleUnit::PngUpdate(RoleUnit* role, QString pic)
{
    if(role == this)
    {
        // printf("animation receive\n");
        pixmap->load(pic);
        this->update();
    }
}

void RoleUnit::GifUpdate(RoleUnit* role, QPixmap pic)
{
    if(role == this)
    {
        // printf("animation receive\n");
        pixmap = new QPixmap(pic);
        this->update();
    }
}