#include "Headers/GroundUnit.h"
#include "Headers/DataPackage.h"

#include <iostream>
#include <QPainter>
#include <QCursor>
#include <QKeyEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>

// GroundUnit* GroundUnit::selectingBlock = nullptr;

// constructer
GroundUnit::GroundUnit()
{
    brushColor = colorUsed::None_Brust;
    penColor = colorUsed::None_Pen;
    Obstacle = false;
    setFlag(QGraphicsItem::ItemIsFocusable);
    m_dSide = FieldData::lenght;
}

// override boundingRect()
QRectF GroundUnit::boundingRect() const 
{
    qreal adjust = 0.5;
    return QRectF(-(m_dSide/2) - adjust, -(m_dSide/2) - adjust, m_dSide + adjust, m_dSide + adjust);
}

// override paint(), showing how to paint a ground unit
void GroundUnit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // if(Obstacle == true)
    // {
    //     painter->setBrush(QBrush(QPixmap(":/background/brick.png")));
    // }
    // else
    // {
    //     painter->setBrush(brushColor);
    //     painter->setPen(QPen(penColor, Qt::MiterJoin));
    // }
    painter->setBrush(brushColor);
    painter->setPen(QPen(penColor, Qt::MiterJoin));
    // draw a rectengle
    painter->drawRect(-(m_dSide/2), -(m_dSide/2), m_dSide, m_dSide);
}

// testing function, use to send it's index on map
std::string GroundUnit::getCoorInfo()
{
    std::string s = "(" + std::to_string(m_CoorX) + ", " + std::to_string(m_CoorY) + ")\n";
    return s;
}

void GroundUnit::mousePressEvent(QGraphicsSceneMouseEvent *event)
{   
    if(!Obstacle)
    {
        emit GroundIsSelected(this);
    }
}

void GroundUnit::setColor(QColor brust, QColor pen)
{
    if(Obstacle)
    {
        brushColor = colorUsed::Obstacle_Brust;
        penColor = colorUsed::None_Pen;
    }
    else
    {
        brushColor = brust;
        penColor = pen;
    }
}


