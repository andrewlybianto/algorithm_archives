#ifndef GRAPH_WINDOW_H
#define GRAPH_WINDOW_H

#include "libraries.h"

#include "edge.h"
#include "node.h"

class GraphWidget;
class Node;
class Edge;

// Window consisting of the interface and graph
class GraphWindow : public QMainWindow
{
    Q_OBJECT
public:
    // Creates a new window with object colors defined from settings
    GraphWindow(QWidget *parent = nullptr, const std::vector<QLineEdit*>& color_fields = {});
};

// Widget consisting of the graph
class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    // Creates a widget for the graph with object colors defined from GraphWindow
    GraphWidget(QWidget *parent = nullptr, const std::vector<QLineEdit*>& color_fields = {});
    ~GraphWidget() override;

    void addNode(qreal new_x, qreal new_y);

    Node* getNode(int id) const;

    // Informs every node that the node with ID pos + 1 is no longer its neighbor, used when a node is deleted
    void everyoneFalse(int pos);

    // Informs every node to push_back a false into its neighbors vector, used when a node is constructed and is not neighbors with any current node
    void everyonePushFalse();

    // Switches the ID, used when a node is deleted
    void flipIsIDTaken(size_t i);

    // Sets nodes[i] to nullptr, used when a node is deleted
    void nullNodes(size_t i);

    QVector<Edge*> getEdges() const;

    // Pushes an edge object to this graph
    void addEdgeToEdges(Edge* toAdd);

    // Removes an edge object from this graph
    void removeEdgeFromEdges(Edge* toRemove);

    int getN() const;

    // Checks whether edge is valid to be added or deleted
    bool isValidEdge(int from, int to);

    // Runs depth first search starting from startID
    void DFS(int startID);

    // Reset all nodes' explored to false
    void setAllExploredFalse();

    // Reset all edges' color to before running algorithm
    void setAllDefaultEdgeColor();

    // Runs breadth first search starting from startID
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

    // Adds a node to the widget with the current value of x and y spinboxes
    // Unable to add a node if occupied
    bool add_node();

    // Deletes a node from the widget with the current value of ID spinbox
    // Deletes any corresponding edges that is incident to the node
    // Unable to delete a nonexistent node
    bool del_node();

    // Adds an edge to the widget with the current value of from and to ID spinboxes
    // Unable to add to or from a nonexistent node, and unable to add the same edge
    bool add_edge();

    // Deletes an edge from the widget with the current value of from and to ID spinboxes
    // Unable to delete to or from a nonexistent node, and unable to delete a nonexistent edge
    bool del_edge();

    // Resets the graph to the status before any algorithm is run
    void reset_graph();

    void run_dfs();
    void run_bfs();

    // Generates a random graph with user selected number of nodes and edges
    // Requires a new window to be run in
    // Due to the nature of the algorithm, selecting a large number of nodes
    // and edges may slow the program due to the depth of the loop
    // needed to generate the graph
    // Note that the number of edges is limited from V-1 to V(V-1)/2 by theorem
    void generate_random_graph();

private:
    // Stores value of spinboxes, only updates when spinbox changes value
    int x_coord;
    int y_coord;
    int selected_id;
    int from_node;
    int to_node;
    int source_id;
    int random_nodes;

    // Possible locations that the node can occupy, used to avoid collision
    QVector<bool> coordinates;

    QVector<Node*> nodes;
    // Possible ID numbers for nodes
    QVector<bool> isIDTaken;
    QVector<Edge*> edges;

    int n;

    const std::vector<QLineEdit*> color_fields;
};

#endif // GRAPH_WINDOW_H
