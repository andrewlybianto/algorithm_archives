#include "edge.h"

Edge::Edge(GraphWidget *newGraph, Node *sourceNode, Node *destNode)
    : graph(newGraph), source(sourceNode), dest(destNode), sourcePoint(sourceNode->getPoint()), destPoint(destNode->getPoint()), color(DEFAULT_EDGE_COLOR)
{
    dest->setNeighborTrue(getSourceNode()->getID() );
    source->setNeighborTrue(getDestNode()->getID() );

    qDebug() << "source" << getSourceNode()->getID();
    for(size_t i = 0; i < getSourceNode()->get_neighbors().size(); ++i)
    {
        if(getSourceNode()->get_neighbors()[i])
        {
            qDebug() << i + 1 << "true";
        }
        else
        {
            qDebug() << i + 1 << "false";
        }
    }
    qDebug() << "dest" << getDestNode()->getID();
    for(size_t i = 0; i < getDestNode()->get_neighbors().size(); ++i)
    {
        if(getDestNode()->get_neighbors()[i])
        {
            qDebug() << i + 1 << "true";
        }
        else
        {
            qDebug() << i + 1 << "false";
        }
    }
}

Edge::~Edge()
{
    dest->setNeighborFalse(getSourceNode()->getID() );
    source->setNeighborFalse(getDestNode()->getID() );

    //remove from graphwidget edges
    graph->removeEdgeFromEdges(this);

    qDebug() << "source" << getSourceNode()->getID();
    for(size_t i = 0; i < getSourceNode()->get_neighbors().size(); ++i)
    {
        if(getSourceNode()->get_neighbors()[i])
        {
            qDebug() << i + 1 << "true";
        }
        else
        {
            qDebug() << i + 1 << "false";
        }
    }
    qDebug() << "dest" << getDestNode()->getID();
    for(size_t i = 0; i < getDestNode()->get_neighbors().size(); ++i)
    {
        if(getDestNode()->get_neighbors()[i])
        {
            qDebug() << i + 1 << "true";
        }
        else
        {
            qDebug() << i + 1 << "false";
        }
    }
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
