#include "edge.h"

// Constructor will change the corresponding neighbors connected with the edges to true
Edge::Edge(GraphWidget *newGraph, Node *sourceNode, Node *destNode, QColor edge_color)
    : graph(newGraph), source(sourceNode), dest(destNode), sourcePoint(sourceNode->getPoint()), destPoint(destNode->getPoint()), color(edge_color)
{
    dest->setNeighborTrue(getSourceNode()->getID() );
    source->setNeighborTrue(getDestNode()->getID() );
}

// Destructor will change the corresponding neighbors connected with the edges to false
Edge::~Edge()
{
    dest->setNeighborFalse(getSourceNode()->getID() );
    source->setNeighborFalse(getDestNode()->getID() );

    // remove from graphwidget edges
    graph->removeEdgeFromEdges(this);
}

Node* Edge::getSourceNode() const
{
    return source;
}

Node* Edge::getDestNode() const
{
    return dest;
}

void Edge::setColor(QColor newColor)
{
    color = newColor;
}

QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-0.5, -0.5, 0.5, 0.5);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);

    // Draw the line itself
    painter->setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    painter->setBrush(Qt::black);
}
