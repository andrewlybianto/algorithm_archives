#ifndef NODE_H
#define NODE_H

#include "libraries.h"

#include "graph_window.h"
#include "edge.h"

// Forward reference
class Edge;
class GraphWidget;

// Node as an object
class Node : public QGraphicsItem
{
public:
    // Create an instance of a node on graphWidget with coordinates new_x and new_y
    // and with color node_color
    Node(GraphWidget *graphWidget, qreal new_x, qreal new_y, QColor node_color);

    // Destructor will remove any edges incident to this node
    ~Node() override;

    QPoint getPoint() const;

    void setColor(QColor newColor);
    void setID(int i);
    int getID() const;

    void add_edge(Node* target, QColor edge_color);

    // Returns an edge from this node to the node with targetID if it exists
    // Returns nullptr otherwise
    Edge* get_edge(int targetID);

    void setNeighborTrue(size_t id);
    void setNeighborFalse(size_t id);

    // Pushes a false to the neighbors of this node, used when a new node is created in the graph
    void neighbors_push_false();

    QVector<bool> get_neighbors() const;

    void setExploredTrue();
    void setExploredFalse();

    bool getExplored() const;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QVector<bool> neighbors;
    GraphWidget *graph;
    QPoint point;
    QColor color;
    int id;
    bool explored;
};
#endif // NODE_H

