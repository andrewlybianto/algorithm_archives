#include "graph_window.h"

const int WIDTH = 400;
const int HEIGHT = 400;
const int NODE_SEPARATION = 50;
const int X_COORD = (-1)*WIDTH / 2;
const int Y_COORD = (-1)*HEIGHT / 2;
const int X_RANGE = (-1)*X_COORD / NODE_SEPARATION;
const int Y_RANGE = (-1)*Y_COORD / NODE_SEPARATION;
const int X_NUMBER_OF_POINTS = WIDTH / NODE_SEPARATION + 1;
const int Y_NUMBER_OF_POINTS = HEIGHT / NODE_SEPARATION + 1;

GraphWindow::GraphWindow(QWidget *parent)
{
    root = parent;

    setWindowTitle("Algorithm");

    graph = new GraphWidget();

    coordinates = new QVector<bool>((X_NUMBER_OF_POINTS * Y_NUMBER_OF_POINTS), false);

    content = new QWidget;
    layout = new QGridLayout(content);
    setLayout(layout);
    layout->addWidget(graph, 0 , 1);

    buttons = new QWidget;
    QGridLayout *button_layout = new QGridLayout(buttons);
    layout->addWidget(buttons, 0, 0);

    // objects
    objects = new std::vector<QWidget*>();

    x_coord = new QSpinBox;
    x_coord->setRange((-1)*X_RANGE, X_RANGE);
    x_coord->setPrefix("x: ");
    y_coord = new QSpinBox;
    y_coord->setRange((-1)*Y_RANGE, Y_RANGE);
    y_coord->setPrefix("y: ");
    QPushButton *add_node = new QPushButton("Add node");

    id = new QSpinBox;
    id->setPrefix("Select node: ");
    id->setRange(1, 100);
    QPushButton *del_node = new QPushButton("Delete node");

    from = new QSpinBox;
    from->setPrefix("From node: ");
    from->setRange(1, 100);
    to = new QSpinBox;
    to->setPrefix("To node: ");
    to->setRange(2, 100);
    add_edge_button = new QPushButton("Add edge");
    del_edge_button = new QPushButton("Delete edge");

    start_id = new QSpinBox;
    start_id->setPrefix("Select start node: ");
    start_id->setRange(1, 100);
    dfs_button = new QPushButton("DFS");
    bfs_button = new QPushButton("BFS");
    // QPushButton *dijkstra_button = new QPushButton("Dijkstra");
    QPushButton *reset_graph = new QPushButton("Reset graph");

    QPushButton *clear_button = new QPushButton("Clear graph");
    QPushButton *home = new QPushButton("Exit");

    objects->push_back(x_coord);
    objects->push_back(y_coord);
    objects->push_back(add_node);
    objects->push_back(id);
    objects->push_back(del_node);
    objects->push_back(from);
    objects->push_back(to);
    objects->push_back(add_edge_button);
    objects->push_back(del_edge_button);
    objects->push_back(start_id);
    objects->push_back(dfs_button);
    objects->push_back(bfs_button);
    // objects->push_back(dijkstra_button);
    objects->push_back(reset_graph);

    objects->push_back(clear_button);
    objects->push_back(home);

    int i = 0;

    for(std::vector<QWidget*>::const_iterator it = objects->begin(); it != objects->end(); ++it)
    {
        button_layout->addWidget(*it, i + 1, 0);
        ++i;
        button_layout->setAlignment(*it, Qt::AlignHCenter);
    }

    connect(add_node, SIGNAL(clicked()), this, SLOT(add_node()));
    connect(del_node, SIGNAL(clicked()), this, SLOT(del_node()));
    connect(add_edge_button, SIGNAL(clicked()), this, SLOT(add_edge()));
    connect(del_edge_button, SIGNAL(clicked()), this, SLOT(del_edge()));
    connect(dfs_button, SIGNAL(clicked()), this, SLOT(run_dfs()));
    connect(bfs_button, SIGNAL(clicked()), this, SLOT(run_bfs()));

    connect(reset_graph, SIGNAL(clicked()), this, SLOT(reset_graph()));
    connect(clear_button, SIGNAL(clicked()), this, SLOT(restart_widget()));
    connect(home, SIGNAL(clicked()), this, SLOT(destroy()));
}

GraphWindow::~GraphWindow()
{
    delete coordinates;
    delete objects;
    delete buttons;
    delete graph;
    delete layout;
    delete content;
    close();
}

void GraphWindow::forceUpdate()
{
    graph->getScene()->update();
}

void GraphWindow::restart_widget()
{
    this->~GraphWindow();

    GraphWindow *gw = new GraphWindow();
    gw->show();
}

void GraphWindow::reset_graph()
{
    graph->everyoneUnexplored();
    graph->everyoneDefaultEdgeColor();
    forceUpdate();
}

void GraphWindow::add_node()
{
    int x = (x_coord->value() + X_RANGE) * X_NUMBER_OF_POINTS;
    int y = y_coord->value() + Y_RANGE;

    if(!coordinates->at(x + y))
    {
        graph->addNode((x_coord->value()) * NODE_SEPARATION, (-1)* (y_coord->value()) * NODE_SEPARATION);
        coordinates->replace(x + y, true);
    }

    forceUpdate();
}

void GraphWindow::del_node()
{
    // BUG when deleting ID 1 and adding again

    QPoint point = graph->getNode(id->value())->getPoint();

    int x = (point.rx() / NODE_SEPARATION + X_RANGE) * X_NUMBER_OF_POINTS;
    int y = (-1)*point.ry() / NODE_SEPARATION + Y_RANGE;

    graph->getNode(id->value())->~Node();
    coordinates->replace(x + y, false);
    forceUpdate();
}

void GraphWindow::add_edge()
{
    if(from->value() == to->value())
        return;

    for(auto it = graph->getEdges().begin(); it != graph->getEdges().end(); ++it)
    {
        if( (*it)->getSourceNode()->getID() == from->value() && (*it)->getDestNode()->getID() == to->value() )
        {
            return;
        }

        if( (*it)->getSourceNode()->getID() == to->value() && (*it)->getDestNode()->getID() == from->value() )
        {
            return;
        }
    }

    graph->getNode(from->value())->add_edge(graph->getNode(to->value()));
    forceUpdate();
}

void GraphWindow::del_edge()
{
    if(from->value() == to->value())
        return;

    graph->getNode(from->value())->get_edge(to->value())->~Edge();
    forceUpdate();
}

void GraphWindow::run_dfs()
{
    graph->everyoneUnexplored();
    graph->everyoneDefaultEdgeColor();

    graph->DFS(start_id->value());
    forceUpdate();
}

void GraphWindow::run_bfs()
{
    graph->everyoneUnexplored();
    graph->everyoneDefaultEdgeColor();

    graph->BFS(start_id->value());
    forceUpdate();
}

void GraphWindow::destroy()
{
    this->~GraphWindow();
}

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    n = 0;
    scene = new QGraphicsScene(this);
    scene->setSceneRect(X_COORD, Y_COORD, WIDTH, HEIGHT);
    setScene(scene);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(WIDTH, HEIGHT);
    setWindowTitle(tr("Simulate"));
}

GraphWidget::~GraphWidget()
{
    delete scene;
    close();
}

QGraphicsScene* GraphWidget::getScene() const
{
    return scene;
}

void GraphWidget::addNode(qreal new_x, qreal new_y)
{
    ++n;

    //construct node
    Node *node = new Node(this, new_x, new_y);

    //set ID
    if(isIDTaken.empty())
    {
        node->setID(1);
        isIDTaken.push_back(true);
        nodes.push_back(node);
        node->neighbors_push_false();
    }
    else
    {
        bool found = false;

        for(size_t i = 0; i < isIDTaken.size(); ++i)
        {
            if(!isIDTaken[i])
            {
                node->setID(i+1);
                isIDTaken[i] = true;
                found = true;
                nodes[i] = node;
                everyoneFalse(i);
                break;
            }
        }

        if(!found)
        {
            node->setID(isIDTaken.size()+1);
            isIDTaken.push_back(true);
            nodes.push_back(node);
            everyonePushFalse();
        }
    }

    //display node
    scene->addItem(node);
    node->setPos(new_x, new_y);
}

Node* GraphWidget::getNode(int id) const
{
    return nodes[id - 1];
}

void GraphWidget::everyoneFalse(int pos)
{
    for(size_t i = 0; i < nodes.size(); ++i)
    {
        if(nodes[i] != nullptr)
        {
            nodes[i]->setNeighborFalse(pos);
        }
    }
}

void GraphWidget::everyonePushFalse()
{
    for(size_t i = 0; i < nodes.size(); ++i)
    {
        if(nodes[i] != nullptr)
        {
            nodes[i]->neighbors_push_false();
        }
    }
}

void GraphWidget::flipIsIDTaken(size_t i)
{
    if(isIDTaken[i])
    {
        isIDTaken[i] = false;
    }
    else
    {
        isIDTaken[i] = true;
    }
}

void GraphWidget::nullNodes(size_t i)
{
    nodes[i] = nullptr;
}

QVector<Edge*> GraphWidget::getEdges() const
{
    return edges;
}

void GraphWidget::addEdgeToEdges(Edge* toAdd)
{
    edges.push_back(toAdd);
}

void GraphWidget::addEdgeToScene(Edge* toAdd)
{
    scene->addItem(toAdd);
}

void GraphWidget::removeEdgeFromEdges(Edge* toRemove)
{
    edges.erase(std::remove(edges.begin(), edges.end(), toRemove), edges.end());
}

int GraphWidget::getN() const
{
    return n;
}

QVector<Node*> GraphWidget::getNodes() const
{
    return nodes;
}

void GraphWidget::DFS(int startID)
{
    getNode(startID)->setExploredTrue();
    for(size_t i = 0; i < getNode(startID)->get_neighbors().size(); ++i)
    {
        if(getNode(startID)->get_neighbors()[i] && !(getNode(i+1)->getExplored()))
        {
            qDebug() << startID;
            getNode(startID)->get_edge(i+1)->setColor("red");
            getNode(i+1)->setExploredTrue();
            DFS(i+1);
        }
    }
}

void GraphWidget::everyoneUnexplored()
{
    for(auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        (*it)->setExploredFalse();
    }
}

void GraphWidget::everyoneDefaultEdgeColor()
{
    for(auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        for(int i = 0; i < (*it)->get_neighbors().size(); ++i)
        {
            auto v = (*it)->get_neighbors();
            if(v[i] == true)
            {
                (*it)->get_edge(i+1)->setColor(DEFAULT_EDGE_COLOR);
            }
        }
    }
}

void GraphWidget::BFS(int startID)
{
    //create a queue for BFS
    QQueue<int> q;

    //mark the current node as explored and enqueue it
    getNode(startID)->setExploredTrue();
    q.enqueue(startID);

    while(!q.empty())
    {
        qDebug() << "Kawhi Leonard" << q.size();
        int s = q.dequeue();

        for(size_t i = 0; i < getNode(s)->get_neighbors().size(); ++i)
        {
            qDebug() << "Paul George";
            if((getNode(s)->get_neighbors()[i]) && !(getNode(i+1)->getExplored()))
            {
                qDebug() << "Lou Williams";
                getNode(s)->get_edge(i+1)->setColor(DEFAULT_ALGO_COLOR);
                getNode(i+1)->setExploredTrue();
                q.enqueue(i+1);
            }
        }
    }
}
