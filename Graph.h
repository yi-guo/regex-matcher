#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Vertex;
class Edge;
class Graph;

class Vertex
{
public:
    Vertex();
    Vertex(int);
    ~Vertex();
    void            setState(int);
    int             getState();
    void            insertState(int);
    vector<int>     getStates();
    void            insertEdge(Edge*);
    vector<Edge*>   getEdges();
    void            setFinal(bool);
    bool            isFinal();
    string          toString();

private:
    int             state;
    vector<int>     states;
    vector<Edge*>   edges;
    bool            final;
};

class Edge
{
public:
    Edge();
    Edge(Vertex*, Vertex*, string);
    ~Edge();
    void        setText(string);
    string      getText();
    void        setHead(Vertex*);
    Vertex*     getHead();
    void        setTail(Vertex*);
    Vertex*     getTail();
    string      toString();

private:
    string      text;
    Vertex*     head;
    Vertex*     tail;
};

class Graph
{
public:
    Graph();
    Graph(int);
    ~Graph();
    void            setHead(Vertex*);
    Vertex*         getHead();
    void            setTail(Vertex*);
    Vertex*         getTail();
    void            insertVertex(Vertex*);
    Vertex*         findVertex(int);
    Vertex*         findVertex(vector<int>);
    void            updateVertices(int);
    vector<Vertex*> getVertices();
    void            insertEdge(Edge*);
    vector<Edge*>   findEdges(Vertex*, Vertex*);
    vector<Edge*>   getEdges();
    unsigned int    size();
    bool            empty();
    void            append(Graph*);
    string          toString();

private:
    Vertex*         head;
    Vertex*         tail;
    vector<Vertex*> vertices;
    vector<Edge*>   edges;
};

#endif  // GRAPH_H
