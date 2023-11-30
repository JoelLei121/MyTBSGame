#include "Headers/Algorithm.h"
#include "Headers/GroundUnit.h"

#include <QVector>
#include <QPair>

QVector<QPair<int, int>> PathfindingAlgorithm::m_path = {};

// Node
void Node::setPos(int x, int y)
{
    pos_x = x;
    pos_y = y;
    F_cost = 10e6;
    parent = nullptr;
}

void Node::setG(int x)
{
    G_cost = x;
}

void Node::setH(int endX, int endY)
{
    H_cost = abs(pos_x - endX) + abs(pos_y - endY);
}

void Node::setParent(Node* p)
{
    parent = p;
}

bool Node::operator==(Node node)
{
    if(this->pos_x == node.pos_x && this->pos_y == node.pos_y)
    {
        return true;
    }
    return false;
}

// pathfinding
void PathfindingAlgorithm::init(int N, bool** array)
{
    // read PlayerList and EnemyList also
    m_dN = N;
    m_NodeMap = new Node*[m_dN];
    for(int i = 0; i < m_dN; ++i)
    {
        m_NodeMap[i] = new Node[m_dN];
    }

    for(int i = 0; i < m_dN; ++i)
    {
        for(int j = 0; j < m_dN; ++j)
        {
            m_NodeMap[i][j].setPos(i, j);
            m_NodeMap[i][j].setParent(nullptr);
            if(array[i][j])
            {
                m_NodeMap[i][j].setObstacle(true);
            }
            else
            {
                m_NodeMap[i][j].setObstacle(false);
            }
            // printf("%d\t", m_NodeMap[i][j].isObstacle());
        }
        // printf("\n");
    }
    m_path.resize(100);
    printf("init\n");
}

void PathfindingAlgorithm::setPoint(int startX, int StartY, int endX, int endY)
{
    m_startX = startX;
    m_startY = StartY;
    m_endX = endX;
    m_endY = endY;
    startNode = &m_NodeMap[m_startX][m_startY];
    endNode = &m_NodeMap[m_endX][m_endY];
    for(int i = 0; i < m_dN; ++i)
    {
        for(int j = 0; j < m_dN; ++j)
        {
            m_NodeMap[i][j].setH(endX, endY);
        }
    }
    printf("set point\n");
}

void PathfindingAlgorithm::FindPath()
{
    PathfindingAlgorithm::m_path.clear();
    QVector<Node*> open, closed;
    startNode->setG(0);
    // startNode->setParentDircetion('x');
    startNode->setF(startNode->getG() + startNode->getH());
    open.push_back(startNode);
    // printf("Algo: init\n");
    while(1)
    {
        // get current w/ lowest f_cost
        Node* currentNode = lowest(open);
        // lastDirection = currentNode->getParentDirection();

        // printf("(%d, %d) start\n", currentNode->x(), currentNode->y());

        // remove from open and add to closed
        int currentIndex = open.indexOf(currentNode);


        closed.push_back(currentNode);
        currentIndex = closed.indexOf(currentNode);
        open.removeAt(open.indexOf(currentNode));
        currentNode = closed[currentIndex];

        // if current is target then break
        if(currentNode->x() == m_endX && currentNode->y() == m_endY)
        {
            // reach to end point, output path
            break;
        }

        // foreach neighbour of current, if added to cloesd or is obstacle then skip, otherwise set f_cost
        QVector<Node*> nbNode;
        int i = currentNode->x(), j = currentNode->y();
        // printf("nbNode added %d\n", nbNode.size());
        if(i+1<m_dN)    nbNode.push_back(&m_NodeMap[i+1][j]);
        if(i-1>=0)    nbNode.push_back(&m_NodeMap[i-1][j]);
        if(j+1<m_dN)    nbNode.push_back(&m_NodeMap[i][j+1]);
        if(j-1>=0)    nbNode.push_back(&m_NodeMap[i][j-1]);


        for(int i = 0; i < nbNode.size(); ++i)
        {
            Node* neighbour = nbNode[i];
            // printf("Algo: dealing node (%d, %d)\n", neighbour->x(), neighbour->y());
            if(closed.indexOf(neighbour) == -1 && !neighbour->isObstacle())
            {
                // if f smaller, update
                // w here is equal to 1
                int F_fromCurrent = currentNode->getG()+1 + neighbour->getH();
                if( F_fromCurrent < neighbour->getF())
                {
                    neighbour->setG(currentNode->getG()+1);
                    neighbour->setF(F_fromCurrent);
                    neighbour->setParent(currentNode);
                    // printf("setting F: %d", neighbour->getF());
                    // printf(" (%d, %d) -> (%d, %d)\n", neighbour->getParent()->x(), neighbour->getParent()->y(), neighbour->x(), neighbour->y());
                }
                // if not in open, add to open
                if(open.indexOf(neighbour) == -1)
                {
                    // printf("Algo: neighbour added to open\n");
                    open.push_back(neighbour);
                }
            }
        }
    }

    // output path

    // checkParent();

    if(endNode->getParent() != nullptr)
    {
        // path found
        printf("path found.\n");
        PathReload(endNode);
        printf("reloaded\n");
    }
    else
    {
        printf("Error: path no found\n");
    }
}

void PathfindingAlgorithm::checkParent()
{
    for(int i = 0; i < m_dN; ++i)
    {
        for(int j = 0; j < m_dN; ++j)
        {
            Node* p = &m_NodeMap[i][j];
            if(p->getParent() == nullptr) printf(" (%d, %d) has no currents\n", p->x(), p->y());
            else printf(" (%d, %d) <- (%d, %d)\n", p->x(), p->y(), p->getParent()->x(), p->getParent()->y());
        }
        // printf("\n");
    }
}

Node* PathfindingAlgorithm::lowest(QVector<Node*> &list)
{
    int min = 10e6;
    Node *node = nullptr, *nodeOther = nullptr;
    // find same direct
    for(int i = 0; i < list.size(); ++i)
    {
        if(list[i]->getF() < min)
        {
            min = list[i]->getF();
            // list[i].getParentDirection() == lastDirection ? node = &list[i] : nodeOther = &list[i];
            node = list[i];
        }
    }

    // if(node == nullptr) node = nodeOther;
    return node;

}

void PathfindingAlgorithm::PathReload(Node* endNode)
{
    PathfindingAlgorithm::m_path.clear();
    Node* current = endNode;
    // if(current == nullptr)
    // {
    //     printf("fucked.\n");
    //     return;
    // }
    PathfindingAlgorithm::m_path.push_front(QPair<int, int>(endNode->x(), endNode->y()));
    printf("push (%d, %d)\n", endNode->x(), endNode->y());
    while(current->getParent() != nullptr)
    {
        // if(m_path.size() > 30) break;
        // if(current == nullptr)
        // {
        //     printf("fucked.\n");
        //     return;
        // }
        PathfindingAlgorithm::m_path.push_front(QPair<int, int>(current->x(), current->y()));
        // printf("push (%d, %d)\n", current->x(), current->y());
        current = current->getParent();
    }
    return;
}

void PathfindingAlgorithm::PrintPath()
{
    printf("work %d\n", PathfindingAlgorithm::m_path.size());
    for(int i = 0; i < PathfindingAlgorithm::m_path.size(); ++i)
    {
        printf("go to (%d, %d)\n", PathfindingAlgorithm::m_path[i].first, PathfindingAlgorithm::m_path[i].second);
    }
}