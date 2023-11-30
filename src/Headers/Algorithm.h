#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QVector>
#include <QPair>
class GroundUnit;

class Node
{
private:
    int pos_x;      // x position on map 
    int pos_y;      // y position on map
    int G_cost;     // distance form starting node
    int H_cost;     // distance form end node
    int F_cost;
    bool m_bObstacle;
    Node* parent;
    
public:
    void setF(int w){ F_cost = w; };    // total weights
    void setG(int x);                   // weights to startpoint
    void setH(int endX, int endY);      // weights to endpoint
    int getF(){ return F_cost; }
    int getG(){ return G_cost; }
    int getH(){ return H_cost; }
    void setPos(int x, int y);
    int x(){ return pos_x; }
    int y(){ return pos_y; }
    Node* getParent(){ return parent; }
    void setParent(Node* p);
    void setObstacle(bool t){ m_bObstacle = t; }
    bool isObstacle(){ return m_bObstacle; }
    bool operator==(Node node);
};

class PathfindingAlgorithm
{
private:
    int m_dN;
    int m_startX;
    int m_startY;
    int m_endX;
    int m_endY;
    Node** m_NodeMap;
    Node* startNode;
    Node* endNode;
    Node* lowest(QVector<Node*> &list);
        void checkParent();

public:
    void init(int N, bool** array);
    void setPoint(int startX, int StartY, int endX, int endY);
    void FindPath();
    static QVector<QPair<int, int>> m_path;
    void PathReload(Node* endNode);
    void PrintPath();
    char lastDirection;
    ~PathfindingAlgorithm()
    {
        for(int i = 0; i < m_dN; ++i)
        {
            delete[] m_NodeMap[i];
        }
        delete[] m_NodeMap;
        m_path.clear();
    }
};



#endif