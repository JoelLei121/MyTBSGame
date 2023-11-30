#ifndef GROUNDUNIT_H
#define GROUNDUNIT_H

#include <QGraphicsObject>

// enum BlockStatus {None = 1, onRange, Player, Enemy, Selected, RoadBlock};

class GroundUnit : public QGraphicsObject {
    Q_OBJECT
public:
    // about itself
    GroundUnit();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setColor(QColor brust, QColor pen);
    QColor getBrust(){ return brushColor; }
    QColor getPen(){ return penColor; }

    // about coordinate
    void setCoor(int x, int y){ m_CoorX = x; m_CoorY = y;}
    int getCoorX(){ return m_CoorX;}
    int getCoorY(){ return m_CoorY;}
    std::string getCoorInfo();
    
    bool isObstacle(){ return Obstacle; }
    void setObstacle(bool t){ Obstacle = t; }

private:
    // BlockStatus m_Status;
    // color
    QColor brushColor;
    QColor penColor;

    // size
    double m_dSide;

    // coordinate
    int m_CoorX;
    int m_CoorY;

    // obstacle
    bool Obstacle;

protected:
    //
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    // void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    // void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:
    // isSelected: to send the index to battleField
    void GroundIsSelected(GroundUnit*);
};

#endif
