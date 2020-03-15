#ifndef EDGE_H
#define EDGE_H

#include "libraries.h"

#include "node.h"
#include "graph_window.h"

// Forward reference
class Node;
class GraphWidget;

// Edge as an object between two nodes
class Edge : public QGraphicsItem
{
public:
    // Create an instance of an edge on newGraph, starting from sourceNode to destNode
    // with color edge_color
    Edge(GraphWidget *newGraph, Node *sourceNode, Node *destNode, QColor edge_color);
    ~Edge() override;

    Node* getSourceNode() const;
    Node* getDestNode() const;

    void setColor(QColor newColor);

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    GraphWidget *graph;

    Node *source, *dest;

    QPointF sourcePoint;
    QPointF destPoint;

    QColor color;
};

#endif // EDGE_H
