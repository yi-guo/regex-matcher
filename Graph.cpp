#include "Graph.h"

bool comparator(Vertex* x, Vertex* y)
{
    return x->getState() < y->getState();
}

Vertex::Vertex()
{
    this->state = 0;
    this->final = false;
}

Vertex::Vertex(int state)
{
    this->state = state;
    this->final = false;
}

Vertex::~Vertex()
{
    for (unsigned int i = 0; i < edges.size(); i++)
        delete edges[i];
    edges.clear();
    states.clear();
}

void Vertex::setState(int state)
{
    this->state = state;
}

int Vertex::getState()
{
    return state;
}

void Vertex::insertState(int state)
{
    states.push_back(state);
    sort(states.begin(), states.end());
}

vector<int> Vertex::getStates()
{
    return states;
}

void Vertex::insertEdge(Edge* edge)
{
    edges.push_back(edge);
}

vector<Edge*> Vertex::getEdges()
{
    return edges;
}

void Vertex::setFinal(bool final)
{
    this->final = final;
}

bool Vertex::isFinal()
{
    return final;
}

string Vertex::toString()
{
    stringstream output;
    output << "State " << state;
    if (!states.empty()) {
        output << " - {";
        for (unsigned int i = 0; i < states.size(); i++) {
            output << states[i];
            if (i != states.size() - 1)
                output << ", ";
        }
        output << "}";
    }
    output << endl;
    return output.str();
}

Edge::Edge()
{
    this->text = string();
    this->head = this->tail = NULL;
}

Edge::Edge(Vertex* head, Vertex* tail, string text)
{
    this->text = text;
    this->head = head;
    this->tail = tail;
}

Edge::~Edge()
{
    ;
}

void Edge::setText(string text)
{
    this->text = text;
}

string Edge::getText()
{
    return text;
}

void Edge::setHead(Vertex* head)
{
    this->head = head;
}

Vertex* Edge::getHead()
{
    return head;
}

void Edge::setTail(Vertex* tail)
{
    this->tail = tail;
}

Vertex* Edge::getTail()
{
    return tail;
}

string Edge::toString()
{
    stringstream output;
    output << "From " << head->getState() << " to " << tail->getState() << ": " << text << endl;
    return output.str();
}

Graph::Graph()
{
    head = tail = NULL;
}

Graph::Graph(int size)
{
    for (int i = 1; i < size + 1; i++)
        vertices.push_back(new Vertex(i));
    if (size > 0) {
        head = vertices[0];
        tail = vertices[vertices.size() - 1];
    }
    else
        head = tail = NULL;
}

Graph::~Graph()
{
    for (unsigned int i = 0; i < vertices.size(); i++)
        delete vertices[i];
    vertices.clear();
    edges.clear();
}

void Graph::setHead(Vertex* head)
{
    this->head = head;
}

Vertex* Graph::getHead()
{
    return head;
}

void Graph::setTail(Vertex* tail)
{
    this->tail = tail;
}

Vertex* Graph::getTail()
{
    return tail;
}

void Graph::insertVertex(Vertex* vertex)
{
    vertices.push_back(vertex);
    sort(vertices.begin(), vertices.end(), comparator);
}

Vertex* Graph::findVertex(int state)
{
    for (unsigned int i = 0; i < vertices.size(); i++) {
        if (vertices[i]->getState() == state)
            return vertices[i];
    }
    return NULL;
}

Vertex* Graph::findVertex(vector<int> states)
{
    for (unsigned int i = 0; i < vertices.size(); i++) {
        if (vertices[i]->getStates() == states)
            return vertices[i];
    }
    return NULL;
}

void Graph::updateVertices(int value)
{
    for (unsigned int i = 0; i < vertices.size(); i++)
        vertices[i]->setState(vertices[i]->getState() + value);
}

vector<Vertex*> Graph::getVertices()
{
    return vertices;
}

void Graph::insertEdge(Edge* edge)
{
    edges.push_back(edge);
}

vector<Edge*> Graph::findEdges(Vertex* head, Vertex* tail)
{
    vector<Edge*> edges;
    if (head != NULL && tail != NULL) {
        for (unsigned int i = 0; i < this->edges.size(); i++) {
            if (this->edges[i]->getHead() == head && this->edges[i]->getTail() == tail)
                edges.push_back(this->edges[i]);
        }
    }
    else if (head != NULL && tail == NULL) {
        for (unsigned int i = 0; i < this->edges.size(); i++) {
            if (this->edges[i]->getHead() == head)
                edges.push_back(this->edges[i]);
        }
    }
    else if (head == NULL && tail != NULL) {
        for (unsigned int i = 0; i < this->edges.size(); i++) {
            if (this->edges[i]->getTail() == tail)
                edges.push_back(this->edges[i]);
        }
    }
    return edges;
}

vector<Edge*> Graph::getEdges()
{
    return edges;
}

unsigned int Graph::size()
{
    return vertices.size();
}

bool Graph::empty()
{
    return size() == 0;
}

void Graph::append(Graph* graph)
{
    if (!empty()) {
        vector<Edge*> edges = findEdges(NULL, tail);
        for (unsigned int i = 0; i < edges.size(); i++)
            edges[i]->setTail(graph->getHead());
        vertices.erase(vertices.end() - 1);
        delete tail;
        tail = graph->getTail();
        graph->updateVertices(size());
        for (unsigned int i = 0; i < graph->getVertices().size(); i++)
            this->vertices.push_back(graph->getVertices()[i]);
        for (unsigned int i = 0; i < graph->getEdges().size(); i++)
            this->edges.push_back(graph->getEdges()[i]);
    }
    else {
        head = graph->getHead();
        tail = graph->getTail();
        edges = graph->getEdges();
        vertices = graph->getVertices();
    }
}

string Graph::toString()
{
    stringstream output;
    for (unsigned int i = 0; i < vertices.size(); i++) {
        Vertex* vertex = vertices[i];
        if (vertex->isFinal())
            output << "*";
        output << vertex->toString();
        for (unsigned int j = 0; j < vertex->getEdges().size(); j++)
            output << vertex->getEdges()[j]->toString();
        output << endl;
    }
    output << "Vertices: " << vertices.size() << " Edges: " << edges.size() << endl;
    return output.str();
}
