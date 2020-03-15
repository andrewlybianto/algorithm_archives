#ifndef GRAPH_WINDOW_H
#define GRAPH_WINDOW_H

#include "libraries.h"

#include "edge.h"
#include "node.h"

class GraphWidget;
class Node;
class Edge;

class GraphWindow : public QMainWindow
{
    Q_OBJECT
public:
    GraphWindow(QWidget *parent = nullptr, const std::vector<QLineEdit*>& color_fields = {});
};

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = nullptr, const std::vector<QLineEdit*>& color_fields = {});
    ~GraphWidget() override;

    void addNode(qreal new_x, qreal new_y);

    Node* getNode(int id) const;

    void everyoneFalse(int pos);

    void everyonePushFalse();

    void flipIsIDTaken(size_t i);

    void nullNodes(size_t i);

    QVector<Edge*> getEdges() const;

    void addEdgeToEdges(Edge* toAdd);

    void removeEdgeFromEdges(Edge* toRemove);

    int getN() const;

    bool isValidEdge(int from, int to);


    //algorithms
    void DFS(int startID);

    void setAllExploredFalse();

    void setAllDefaultEdgeColor();

    void BFS(int startID);

signals:
    void x_coord_changed(int x);
    void y_coord_changed(int y);
    void selected_id_changed(int id);
    void from_node_changed(int id);
    void to_node_changed(int id);
    void source_id_changed(int id);
    void random_nodes_changed(int value);

public slots:
    void set_x_coord(int x);
    void set_y_coord(int y);
    void set_selected_id(int id);
    void set_from_node(int id);
    void set_to_node(int id);
    void set_source_id(int id);
    void set_random_nodes(int value);

    bool add_node();
    bool del_node();
    bool add_edge();
    bool del_edge();
    void reset_graph();
    void run_dfs();
    void run_bfs();
    void generate_random_graph();

private:
    int x_coord;
    int y_coord;
    int selected_id;
    int from_node;
    int to_node;
    int source_id;
    int random_nodes;

    QVector<bool> coordinates;

    QVector<Node*> nodes;
    QVector<bool> isIDTaken;
    QVector<Edge*> edges;

    int n;

    const std::vector<QLineEdit*> color_fields;
};

#endif // GRAPH_WINDOW_H
