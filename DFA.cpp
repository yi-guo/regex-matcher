#include "DFA.h"

bool found(vector<int> states, int state)
{
    for (unsigned int i = 0; i < states.size(); i++) {
        if (states[i] == state)
            return true;
    }
    return false;
}

bool found(vector<string> texts, string text)
{
    for (unsigned int i = 0; i < texts.size(); i++) {
        if (texts[i] == text)
            return true;
    }
    return false;
}

bool exsited(Graph* graph, Vertex* vertex)
{
    if (graph->findVertex(vertex->getStates()) != NULL)
        return true;
    else
        return false;
}

void closure(Vertex* vertex, Graph* nfa)
{
    vector<int> states = vertex->getStates();
    for (unsigned int i = 0; i < states.size(); i++) {
        vector<Edge*> edges = nfa->findVertex(states[i])->getEdges();
        for (unsigned int j = 0; j < edges.size(); j++) {
            if (edges[j]->getText() == "epsilon" && !found(states, edges[j]->getTail()->getState())) {
                states.push_back(edges[j]->getTail()->getState());
                vertex->insertState(edges[j]->getTail()->getState());
            }
        }
    }
}

Vertex* move(Vertex* head, Graph* nfa, string text)
{
    Vertex* tail = new Vertex();
    vector<int> states = head->getStates();
    for (unsigned int i = 0; i < states.size(); i++) {
        Vertex* vertex = nfa->findVertex(states[i]);
        for (unsigned int j = 0; j < vertex->getEdges().size(); j++) {
            if (vertex->getEdges()[j]->getText() == text && !found(tail->getStates(), vertex->getEdges()[j]->getTail()->getState())) {
                tail->insertState(vertex->getEdges()[j]->getTail()->getState());
                closure(tail, nfa);
            }
        }
    }
    if (found(tail->getStates(), nfa->getTail()->getState()))
        tail->setFinal(true);
    return tail;
}

Graph* getDFA(string regEx)
{
    Graph* dfa = new Graph(1);
    if (!regEx.empty()) {
        Graph* nfa = getNFA(regEx);
        dfa->getHead()->insertState(1);
        closure(dfa->getHead(), nfa);

        vector<string> texts;
        for (unsigned int i = 0; i < nfa->getEdges().size(); i++) {
            string text = nfa->getEdges()[i]->getText();
            if (text != "epsilon" && !found(texts, text))
                texts.push_back(nfa->getEdges()[i]->getText());
        }

        vector<Vertex*> vertices = dfa->getVertices();
        while (!vertices.empty()) {
            Vertex* vertex = vertices[0];
            for (unsigned int i = 0; i < texts.size(); i++) {
                Vertex* newVertex = move(vertex, nfa, texts[i]);
                if (!newVertex->getStates().empty()) {
                    Edge* edge;
                    if (!exsited(dfa, newVertex)) {
                        newVertex->setState(dfa->size() + 1);
                        edge = new Edge(vertex, newVertex, texts[i]);
                        dfa->insertVertex(newVertex);
                        vertices.push_back(newVertex);
                    }
                    else {
                        edge = new Edge(vertex, dfa->findVertex(newVertex->getStates()), texts[i]);
                        delete newVertex;
                    }
                    dfa->insertEdge(edge);
                    vertex->insertEdge(edge);
                }
                else
                    delete newVertex;
            }
            vertices.erase(vertices.begin());
        }
    }
    else {
        dfa->getHead()->setFinal(true);
    }
    return dfa;

}
