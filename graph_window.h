#ifndef GRAPH_WINDOW_H
#define GRAPH_WINDOW_H

#include "libraries.h"

#include "edge.h"
#include "node.h"

class GraphWidget;
class Node;
class Edge;

class GraphWindow : public QWidget
{
    Q_OBJECT
public:
    GraphWindow(QWidget *parent = nullptr);
    ~GraphWindow();

    void forceUpdate();

signals:

public slots:
    void add_node();
    void del_node();
    void add_edge();
    void del_edge();
    void reset_graph();
    void restart_widget();
    void run_dfs();
    void run_bfs();
    void destroy();

private:
    QVector <bool> *coordinates;

    QWidget *root;
    QWidget *content;
    QGridLayout *layout;
    GraphWidget *graph;

    QWidget *buttons;

    std::vector<QWidget*> *objects;

    QSpinBox *x_coord;
    QSpinBox *y_coord;
    QSpinBox *id;

    QSpinBox *from;
    QSpinBox *to;

    QPushButton *add_edge_button;
    QPushButton *del_edge_button;

    QSpinBox *start_id;

    QPushButton *dfs_button;
    QPushButton *bfs_button;
};

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = nullptr);
    ~GraphWidget();

    void addNode(qreal new_x, qreal new_y);

    Node* getNode(int id) const;

    void everyoneFalse(int pos);

    void everyonePushFalse();

    void flipIsIDTaken(size_t i);

    void nullNodes(size_t i);

    QVector<Edge*> getEdges() const;

    void addEdgeToEdges(Edge* toAdd);

    void addEdgeToScene(Edge* toAdd);

    void removeEdgeFromEdges(Edge* toRemove);

    int getN() const;

    QVector<Node*> getNodes() const;

    QGraphicsScene* getScene() const;

    //algorithms
    void DFS(int startID);

    void everyoneUnexplored();

    void everyoneDefaultEdgeColor();

    void BFS(int startID);

public slots:

protected:

private:
    QGraphicsScene *scene;
    QVector<Node*> nodes;
    QVector<bool> isIDTaken;
    QVector<Edge*> edges;
    int n;
};

#endif // GRAPH_WINDOW_H
