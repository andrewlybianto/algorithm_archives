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
const int TOT_NUMBER_OF_POINTS = X_NUMBER_OF_POINTS * Y_NUMBER_OF_POINTS;

GraphWindow::GraphWindow(QWidget *parent, std::vector<QLineEdit*>* color)
{
    setAttribute(Qt::WA_DeleteOnClose);

    qsrand(time(NULL));

    root = parent;
    object_color = color;

    setWindowTitle("Algorithm");

    graph = new GraphWidget(this, object_color);

    coordinates = new QVector<bool>(TOT_NUMBER_OF_POINTS, false);

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
    id->setRange(1, TOT_NUMBER_OF_POINTS);
    QPushButton *del_node = new QPushButton("Delete node");

    from = new QSpinBox;
    from->setPrefix("From node: ");
    from->setRange(1, TOT_NUMBER_OF_POINTS);
    to = new QSpinBox;
    to->setPrefix("To node: ");
    to->setRange(2, TOT_NUMBER_OF_POINTS);
    add_edge_button = new QPushButton("Add edge");
    del_edge_button = new QPushButton("Delete edge");

    start_id = new QSpinBox;
    start_id->setPrefix("Select start node: ");
    start_id->setRange(1, TOT_NUMBER_OF_POINTS);
    dfs_button = new QPushButton("DFS");
    bfs_button = new QPushButton("BFS");
    // QPushButton *dijkstra_button = new QPushButton("Dijkstra");
    random_nodes = new QSpinBox;
    random_nodes->setPrefix("Select number of random nodes: ");
    random_nodes->setRange(2, TOT_NUMBER_OF_POINTS);
    QPushButton *random_button = new QPushButton("Randomize graph");
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
    objects->push_back(random_nodes);
    objects->push_back(random_button);
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

    connect(random_button, SIGNAL(clicked()), this, SLOT(random_graph()));
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

bool GraphWindow::add_node(int x, int y)
{
    int x_index = (x + X_RANGE) * X_NUMBER_OF_POINTS;
    int y_index = y + Y_RANGE;

    if(!coordinates->at(x_index + y_index))
    {
        graph->addNode(x * NODE_SEPARATION, (-1)* y * NODE_SEPARATION);
        coordinates->replace(x_index + y_index, true);
        return true;
    }
    return false;
}

bool GraphWindow::add_edge(int from, int to)
{
    if(from == to)
        return false;

    for(auto it = graph->getEdges().begin(); it != graph->getEdges().end(); ++it)
    {
        if( (*it)->getSourceNode()->getID() == from && (*it)->getDestNode()->getID() == to )
        {
            return false;
        }

        if( (*it)->getSourceNode()->getID() == to && (*it)->getDestNode()->getID() == from )
        {
            return false;
        }
    }

    graph->getNode(from)->add_edge(graph->getNode(to), QColor(object_color->at(1)->text()));
    return true;
}

void GraphWindow::forceUpdate()
{
    graph->getScene()->update();
}

void GraphWindow::restart_widget()
{
    GraphWindow *gw = new GraphWindow(nullptr, object_color);
    this->~GraphWindow();

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
    add_node(x_coord->value(), y_coord->value());

    forceUpdate();
}

void GraphWindow::del_node()
{
    QPoint point = graph->getNode(id->value())->getPoint();

    int x = (point.rx() / NODE_SEPARATION + X_RANGE) * X_NUMBER_OF_POINTS;
    int y = (-1)*point.ry() / NODE_SEPARATION + Y_RANGE;

    graph->getNode(id->value())->~Node();
    coordinates->replace(x + y, false);
    forceUpdate();
}

void GraphWindow::add_edge()
{
    add_edge(from->value(), to->value());

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

void GraphWindow::random_graph()
{
    // CHECK sometimes crashes due to too many loop in adding edges, better algorithm needed

    int num_nodes = random_nodes->value();

    // prompt for number of edges
    QDialog *prompt = new QDialog(this);
    QWidget *content = new QWidget;
    QGridLayout *layout = new QGridLayout(content);
    prompt->setLayout(layout);
    QLabel *info = new QLabel;
    info->setText("Select number of edges:");
    QSpinBox *edges = new QSpinBox;
    // theorem from graph theory
    edges->setRange(num_nodes - 1, num_nodes * (num_nodes - 1) / 2);
    QPushButton *ok_button = new QPushButton("Confirm");

    layout->addWidget(info, 0, 0);
    layout->addWidget(edges, 1, 0);
    layout->addWidget(ok_button, 2, 0);
    connect(ok_button, SIGNAL(clicked()), prompt, SLOT(close()));
    prompt->exec();

    int num_edges = edges->value();
    int counter_nodes = 0;
    int counter_edges = 0;

    while(counter_nodes < num_nodes)
    {
        int x = (qrand() % X_NUMBER_OF_POINTS) - X_RANGE;
        int y = (qrand() % Y_NUMBER_OF_POINTS) - Y_RANGE;

        if(add_node(x, y))
            counter_nodes++;
    }

    while(counter_edges < num_edges)
    {
        int from = (qrand() % num_nodes) + 1;
        int to = (qrand() % num_nodes) + 1;

        if(add_edge(from, to))
            counter_edges++;
    }

    forceUpdate();
}

void GraphWindow::destroy()
{
    this->~GraphWindow();
}

GraphWidget::GraphWidget(QWidget *parent, std::vector<QLineEdit*>* object_color)
    : QGraphicsView(parent)
{
    root = parent;
    colors = object_color;

    setStyleSheet("background-color: white;");

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
    Node *node = new Node(this, new_x, new_y, QColor(colors->at(0)->text()));

    //set ID
    if(isIDTaken.empty())
    {
        node->setID(1);
        isIDTaken.push_back(true);
        nodes.push_back(node);
        node->neighbors_push_false();
    }
    else if(isIDTaken[0] == false)
    {
        node->setID(1);
        isIDTaken[0] = true;
        nodes[0] = node;
        if(!(nodes.size() == 1))
            everyoneFalse(0);
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
            nodes[i]->setNeighborFalse(pos + 1);
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
            getNode(startID)->get_edge(i+1)->setColor(QColor(colors->at(2)->text()));
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
                (*it)->get_edge(i+1)->setColor(QColor(colors->at(1)->text()));
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
                getNode(s)->get_edge(i+1)->setColor(QColor(colors->at(2)->text()));
                getNode(i+1)->setExploredTrue();
                q.enqueue(i+1);
            }
        }
    }
}
