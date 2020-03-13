#ifndef NODE_H
#define NODE_H

#include "libraries.h"

#include "graph_window.h"
#include "edge.h"

class Edge;
class GraphWidget;

class Node : public QGraphicsItem
{
public:
    Node(GraphWidget *graphWidget, qreal new_x, qreal new_y, QColor node_color);
    ~Node() override;

    QPoint getPoint() const;

    void setColor(QColor newColor);
    void setID(int i);
    int getID() const;

    void add_edge(Node* target, QColor edge_color);

    Edge* get_edge(int targetID);

    void setNeighborTrue(size_t id);
    void setNeighborFalse(size_t id);

    void neighbors_push_false();
    QVector<bool> get_neighbors() const;

    void setExploredTrue();
    void setExploredFalse();

    bool getExplored() const;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:

private:
    QVector<bool> neighbors;
    GraphWidget *graph;
    QPoint point;
    QColor color;
    int id;
    bool explored;
};
#endif // NODE_H

