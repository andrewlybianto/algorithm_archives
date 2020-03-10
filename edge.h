#ifndef EDGE_H
#define EDGE_H

#include "libraries.h"

#include "node.h"
#include "graph_window.h"

class Node;
class GraphWidget;

class Edge : public QGraphicsItem
{
public:
    Edge(GraphWidget *newGraph, Node *sourceNode, Node *destNode);
    ~Edge() override;

    Node *getSourceNode() const;
    Node *getDestNode() const;

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
