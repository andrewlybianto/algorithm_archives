#include "graph_window.h"

// Width of widget
const int WIDTH = 400;

// Height of widget
const int HEIGHT = 400;

// The distance between nodes to avoid collision, including size of nodes
const int NODE_SEPARATION = 50;

// The maximum x coordinate
const int X_COORD = (-1)*WIDTH / 2;

// The maximum y coordinate
const int Y_COORD = (-1)*HEIGHT / 2;

// Transform the x coordinate to account collision
const int X_RANGE = (-1)*X_COORD / NODE_SEPARATION;

// Transform the y coordinate to account collision
const int Y_RANGE = (-1)*Y_COORD / NODE_SEPARATION;

// The number of points that a node can occupy the x axis
const int X_NUMBER_OF_POINTS = WIDTH / NODE_SEPARATION + 1;

// The number of points that a node can occupy the y axis
const int Y_NUMBER_OF_POINTS = HEIGHT / NODE_SEPARATION + 1;

// The total number of points that a node can occupy in the widget
const int TOT_NUMBER_OF_POINTS = X_NUMBER_OF_POINTS * Y_NUMBER_OF_POINTS;

// Constructor will create a window that consists of both the buttons and graph widget
GraphWindow::GraphWindow(QWidget *parent, const std::vector<QLineEdit*>& col)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setParent(parent);

    qsrand(time(NULL));

    setWindowTitle("Algorithm");

    GraphWidget *graph = new GraphWidget(this, col);
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QGridLayout *layout = new QGridLayout;
    widget->setLayout(layout);
    layout->addWidget(graph, 0 , 1);

    QWidget *interface_section = new QWidget;
    QGridLayout *interface_layout = new QGridLayout(interface_section);
    layout->addWidget(interface_section, 0, 0);

    QSpinBox *x_coord = new QSpinBox;
    x_coord->setRange((-1)*X_RANGE, X_RANGE);
    x_coord->setPrefix("x: ");
    QSpinBox *y_coord = new QSpinBox;
    y_coord->setRange((-1)*Y_RANGE, Y_RANGE);
    y_coord->setPrefix("y: ");
    QPushButton *add_node = new QPushButton("Add node");

    QSpinBox *id = new QSpinBox;
    id->setPrefix("Select node: ");
    id->setRange(1, TOT_NUMBER_OF_POINTS);
    QPushButton *del_node = new QPushButton("Delete node");

    QSpinBox *from_node = new QSpinBox;
    from_node->setPrefix("From node: ");
    from_node->setRange(1, TOT_NUMBER_OF_POINTS);
    QSpinBox *to_node = new QSpinBox;
    to_node->setPrefix("To node: ");
    to_node->setRange(2, TOT_NUMBER_OF_POINTS);
    QPushButton *add_edge_button = new QPushButton("Add edge");
    QPushButton *del_edge_button = new QPushButton("Delete edge");

    QSpinBox *source_id = new QSpinBox;
    source_id->setPrefix("Select start node: ");
    source_id->setRange(1, TOT_NUMBER_OF_POINTS);
    QPushButton *dfs_button = new QPushButton("DFS");
    QPushButton *bfs_button = new QPushButton("BFS");
    QSpinBox *random_nodes = new QSpinBox;
    random_nodes->setPrefix("Select number of random nodes: ");
    random_nodes->setRange(2, TOT_NUMBER_OF_POINTS);
    QPushButton *random_button = new QPushButton("Randomize graph");
    QPushButton *reset_graph = new QPushButton("Reset graph");

    QPushButton *home = new QPushButton("Exit");

    // Set interface positions
    std::vector<QWidget*> objects;
    objects.push_back(x_coord);
    objects.push_back(y_coord);
    objects.push_back(add_node);
    objects.push_back(id);
    objects.push_back(del_node);
    objects.push_back(from_node);
    objects.push_back(to_node);
    objects.push_back(add_edge_button);
    objects.push_back(del_edge_button);
    objects.push_back(source_id);
    objects.push_back(dfs_button);
    objects.push_back(bfs_button);
    objects.push_back(random_nodes);
    objects.push_back(random_button);
    objects.push_back(reset_graph);

    objects.push_back(home);

    int i = 0;
    for(auto it : objects)
    {
        interface_layout->addWidget(it, i + 1, 0);
        ++i;
        interface_layout->setAlignment(it, Qt::AlignHCenter);
    }

    // Set interface functions
    connect(x_coord, SIGNAL(valueChanged(int)), graph, SIGNAL(x_coord_changed(int)));
    connect(y_coord, SIGNAL(valueChanged(int)), graph, SIGNAL(y_coord_changed(int)));
    connect(id, SIGNAL(valueChanged(int)), graph, SIGNAL(selected_id_changed(int)));
    connect(from_node, SIGNAL(valueChanged(int)), graph, SIGNAL(from_node_changed(int)));
    connect(to_node, SIGNAL(valueChanged(int)), graph, SIGNAL(to_node_changed(int)));
    connect(source_id, SIGNAL(valueChanged(int)), graph, SIGNAL(source_id_changed(int)));
    connect(random_nodes, SIGNAL(valueChanged(int)), graph, SIGNAL(random_nodes_changed(int)));

    connect(add_node, SIGNAL(clicked()), graph, SLOT(add_node()));
    connect(del_node, SIGNAL(clicked()), graph, SLOT(del_node()));
    connect(add_edge_button, SIGNAL(clicked()), graph, SLOT(add_edge()));
    connect(del_edge_button, SIGNAL(clicked()), graph, SLOT(del_edge()));
    connect(dfs_button, SIGNAL(clicked()), graph, SLOT(run_dfs()));
    connect(bfs_button, SIGNAL(clicked()), graph, SLOT(run_bfs()));

    connect(random_button, SIGNAL(clicked()), graph, SLOT(generate_random_graph()));
    connect(reset_graph, SIGNAL(clicked()), graph, SLOT(reset_graph()));
    connect(home, SIGNAL(clicked()), this, SLOT(close()));
}

GraphWidget::GraphWidget(QWidget *parent, const std::vector<QLineEdit*>& col)
    : QGraphicsView(parent), color_fields(col)
{
    x_coord = 0;
    y_coord = 0;
    selected_id = 1;
    from_node = 1;
    to_node = 2;
    source_id = 1;
    random_nodes = 2;

    setParent(parent);

    setStyleSheet("background-color: white;");

    n = 0;
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(X_COORD, Y_COORD, WIDTH, HEIGHT);
    setScene(scene);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(WIDTH, HEIGHT);
    setWindowTitle(tr("Simulate"));

    coordinates = QVector<bool>(TOT_NUMBER_OF_POINTS, false);

    // Set spinbox values
    connect(this, SIGNAL(x_coord_changed(int)), this, SLOT(set_x_coord(int)));
    connect(this, SIGNAL(y_coord_changed(int)), this, SLOT(set_y_coord(int)));
    connect(this, SIGNAL(selected_id_changed(int)), this, SLOT(set_selected_id(int)));
    connect(this, SIGNAL(from_node_changed(int)), this, SLOT(set_from_node(int)));
    connect(this, SIGNAL(to_node_changed(int)), this, SLOT(set_to_node(int)));
    connect(this, SIGNAL(source_id_changed(int)), this, SLOT(set_source_id(int)));
    connect(this, SIGNAL(random_nodes_changed(int)), this, SLOT(set_random_nodes(int)));
}

GraphWidget::~GraphWidget()
{
    for(auto i : nodes)
    {
        delete i;
        i = nullptr;
    }
}

void GraphWidget::set_x_coord(int x)
{
    x_coord = x;
}

void GraphWidget::set_y_coord(int y)
{
    y_coord = y;
}

void GraphWidget::set_selected_id(int id)
{
    selected_id = id;
}

void GraphWidget::set_from_node(int id)
{
    from_node = id;
}

void GraphWidget::set_to_node(int id)
{
    to_node = id;
}

void GraphWidget::set_source_id(int id)
{
    source_id = id;
}

void GraphWidget::set_random_nodes(int value)
{
    random_nodes = value;
}

bool GraphWidget::add_node()
{
    int x_index = (x_coord + X_RANGE) * X_NUMBER_OF_POINTS;
    int y_index = y_coord + Y_RANGE;

    if(!coordinates[x_index + y_index])
    {
        addNode(x_coord * NODE_SEPARATION, (-1)* y_coord * NODE_SEPARATION);
        coordinates.replace(x_index + y_index, true);
        update();
        return true;
    }
    return false;
}

bool GraphWidget::del_node()
{
    if(selected_id > nodes.size() || (!nodes[selected_id - 1]) )
        return false;

    QPoint point = getNode(selected_id)->getPoint();

    int x_index = (point.rx() / NODE_SEPARATION + X_RANGE) * X_NUMBER_OF_POINTS;
    int y_index = (-1)*point.ry() / NODE_SEPARATION + Y_RANGE;

    getNode(selected_id)->~Node();
    coordinates.replace(x_index + y_index, false);
    update();
    return true;
}

bool GraphWidget::add_edge()
{
    if(!isValidEdge(from_node, to_node))
        return false;

    for(auto it : edges)
    {
        if(it->getSourceNode()->getID() == from_node && it->getDestNode()->getID() == to_node)
        {
            return false;
        }

        if(it->getSourceNode()->getID() == to_node && it->getDestNode()->getID() == from_node)
        {
            return false;
        }
    }

    getNode(from_node)->add_edge(getNode(to_node), QColor(color_fields[1]->text()));
    update();
    return true;
}

bool GraphWidget::del_edge()
{
    if(!isValidEdge(from_node, to_node))
        return false;

    if(getNode(from_node)->get_edge(to_node) == nullptr)
        return false;

    if(getNode(to_node)->get_edge(from_node) == nullptr)
        return false;

    getNode(from_node)->get_edge(to_node)->~Edge();
    update();
    return true;
}

void GraphWidget::reset_graph()
{
    setAllExploredFalse();
    setAllDefaultEdgeColor();
    update();
}

void GraphWidget::run_dfs()
{
    setAllExploredFalse();
    setAllDefaultEdgeColor();
    DFS(source_id);
    update();
}

void GraphWidget::run_bfs()
{
    setAllExploredFalse();
    setAllDefaultEdgeColor();
    BFS(source_id);
    update();
}

void GraphWidget::generate_random_graph()
{
    if(!(nodes.empty()))
    {
        QErrorMessage *err = new QErrorMessage;
        err->showMessage("Please open a new window!");
        return;
    }

    // Prompt for number of edges
    QDialog *prompt = new QDialog;
    QWidget *content = new QWidget;
    QGridLayout *layout = new QGridLayout(content);
    prompt->setLayout(layout);
    QLabel *info = new QLabel;
    info->setText("Select number of edges:");
    QSpinBox *edges = new QSpinBox;

    // Theorem from graph theory
    edges->setRange(random_nodes - 1, random_nodes * (random_nodes - 1) / 2);
    QPushButton *ok_button = new QPushButton("Confirm");

    layout->addWidget(info, 0, 0);
    layout->addWidget(edges, 1, 0);
    layout->addWidget(ok_button, 2, 0);
    connect(ok_button, SIGNAL(clicked()), prompt, SLOT(close()));
    prompt->exec();

    int num_edges = edges->value();
    int counter_nodes = 0;
    int counter_edges = 0;

    while(counter_nodes < random_nodes)
    {
        int temp_x = x_coord;
        int temp_y = y_coord;
        x_coord = (qrand() % X_NUMBER_OF_POINTS) - X_RANGE;
        y_coord = (qrand() % Y_NUMBER_OF_POINTS) - Y_RANGE;

        if(add_node())
            counter_nodes++;

        x_coord = temp_x;
        y_coord = temp_y;
    }

    while(counter_edges < num_edges)
    {
        int temp_from = from_node;
        int temp_to = to_node;
        from_node = (qrand() % random_nodes) + 1;
        to_node = (qrand() % random_nodes) + 1;

        if(add_edge())
            counter_edges++;

        from_node = temp_from;
        to_node = temp_to;
    }

    update();
}

void GraphWidget::addNode(qreal new_x, qreal new_y)
{
    ++n;

    // Construct node
    Node *node = new Node(this, new_x, new_y, QColor(color_fields[0]->text()));

    // Set ID
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

        for(int i = 0; i < isIDTaken.size(); ++i)
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

    // Display node
    scene()->addItem(node);
    node->setPos(new_x, new_y);
}

Node* GraphWidget::getNode(int id) const
{
    return nodes[id - 1];
}

void GraphWidget::everyoneFalse(int pos)
{
    for(auto it : nodes)
    {
        if(it != nullptr)
        {
            it->setNeighborFalse(pos + 1);
        }
    }
}

void GraphWidget::everyonePushFalse()
{
    for(auto it : nodes)
    {
        if(it != nullptr)
        {
            it->neighbors_push_false();
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

void GraphWidget::removeEdgeFromEdges(Edge* toRemove)
{
    edges.erase(std::remove(edges.begin(), edges.end(), toRemove), edges.end());
}

int GraphWidget::getN() const
{
    return n;
}

bool GraphWidget::isValidEdge(int from, int to)
{
    if(from == to)
        return false;

    if(from > nodes.size() || (!nodes[from - 1]))
        return false;

    if(to > nodes.size() || (!nodes[to - 1]))
        return false;

    return true;
}

void GraphWidget::DFS(int startID)
{
    getNode(startID)->setExploredTrue();
    for(int i = 0; i < getNode(startID)->get_neighbors().size(); ++i)
    {
        if(getNode(startID)->get_neighbors()[i] && !(getNode(i+1)->getExplored()))
        {
            getNode(startID)->get_edge(i+1)->setColor(QColor(color_fields[2]->text()));
            getNode(i+1)->setExploredTrue();
            DFS(i+1);
        }
    }
}

void GraphWidget::setAllExploredFalse()
{
    for(auto it : nodes)
    {
        if(it != nullptr)
        {
            it->setExploredFalse();
        }
    }
}

void GraphWidget::setAllDefaultEdgeColor()
{
    for(auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        if((*it) != nullptr)
        {
            for(int i = 0; i < (*it)->get_neighbors().size(); ++i)
            {
                auto v = (*it)->get_neighbors();
                if(v[i] == true)
                {
                   (*it)->get_edge(i+1)->setColor(QColor(color_fields[1]->text()));
                }
            }
        }
    }
}

void GraphWidget::BFS(int startID)
{
    // Create a queue for BFS
    QQueue<int> q;

    // Mark the current node as explored and enqueue it
    getNode(startID)->setExploredTrue();
    q.enqueue(startID);

    while(!q.empty())
    {
        int s = q.dequeue();

        for(int i = 0; i < getNode(s)->get_neighbors().size(); ++i)
        {
            if((getNode(s)->get_neighbors()[i]) && !(getNode(i+1)->getExplored()))
            {
                getNode(s)->get_edge(i+1)->setColor(QColor(color_fields[2]->text()));
                getNode(i+1)->setExploredTrue();
                q.enqueue(i+1);
            }
        }
    }
}
