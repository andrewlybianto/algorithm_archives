#include "node.h"

Node::Node(GraphWidget *graphWidget, qreal new_x, qreal new_y)
    : graph(graphWidget), color(DEFAULT_NODE_COLOR)
{
    point.setX(new_x);
    point.setY(new_y);

    QVector<bool> dummy(graph->getN() - 1, false);
    neighbors = dummy;
}

Node::~Node()
{
    //every node that has this as a neighbor should not anymore
    for(size_t i = 0; i < neighbors.size(); ++i)
    {
        if(neighbors[i])
        {
            //destroy edge
            get_edge(i+1)->~Edge();

            graph->getNode(i+1)->setNeighborFalse(getID());
        }
    }

    //deal with nodes and ids
    graph->nullNodes(getID()-1);
    graph->flipIsIDTaken(getID()-1);
}

void Node::add_edge(Node* target)
{
    Edge *edge = new Edge(graph, this, target);
    graph->addEdgeToEdges(edge);
    graph->addEdgeToScene(edge);
}

Edge* Node::get_edge(int targetID)
{
    for(size_t i = 0; i < graph->getEdges().size(); ++i)
    {
        if(graph->getEdges()[i]->getSourceNode() == this && graph->getEdges()[i]->getDestNode() == graph->getNode(targetID))
        {
            return graph->getEdges()[i];
        }
        else if(graph->getEdges()[i]->getDestNode() == this && graph->getEdges()[i]->getSourceNode() == graph->getNode(targetID))
        {
            return graph->getEdges()[i];
        }
    }
}

void Node::setNeighborTrue(size_t id)
{
    neighbors[id-1] = true;
}

void Node::setNeighborFalse(size_t id)
{
    neighbors[id-1] = false;
}

void Node::neighbors_push_false()
{
    neighbors.push_back(false);
}

QVector<bool> Node::get_neighbors() const
{
    return neighbors;
}

QPoint Node::getPoint() const
{
    return point;
}

void Node::setColor(QColor newColor)
{
    color = newColor;
}

void Node::setID(int i)
{
    id = i;
}

int Node::getID() const
{
    return id;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(QPen(Qt::black, 0));
    painter->setBrush(color);
    painter->drawEllipse(-10, -10, 20, 20);
    QPoint displace = QPoint(-3,-15);
    QFont font = painter->font() ;

    /* twice the size than the current font size */
    font.setPointSize(font.pointSize() * 2);

    /* set the modified font to the painter */
    painter->setFont(font);
    painter->drawText(displace, QString::number(id));
}

void Node::setExploredTrue()
{
    explored = true;
}

void Node::setExploredFalse()
{
    explored = false;
}

bool Node::getExplored() const
{
    return explored;
}
