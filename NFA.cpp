#include "NFA.h"

Graph* getConcatenation(string text)
{
    Graph* graph = new Graph(2);
    Edge* edge = new Edge(graph->getHead(), graph->getTail(), text);
    graph->insertEdge(edge);
    graph->getHead()->insertEdge(edge);
    graph->getTail()->setFinal(true);
    return graph;
}

Graph* getAlternation(Graph* top, Graph* bottom)
{
    Graph* graph = new Graph(2);

    top->updateVertices(1);
    bottom->updateVertices(top->size() + 1);
    graph->getTail()->setState(graph->getTail()->getState() + top->size() + bottom->size());

    for (unsigned int i = 0; i < top->getVertices().size(); i++)
        graph->insertVertex(top->getVertices()[i]);
    for (unsigned int i = 0; i < bottom->getVertices().size(); i++)
        graph->insertVertex(bottom->getVertices()[i]);
    for (unsigned int i = 0; i < top->getEdges().size(); i++)
        graph->insertEdge(top->getEdges()[i]);
    for (unsigned int i = 0; i < bottom->getEdges().size(); i++)
        graph->insertEdge(bottom->getEdges()[i]);

    Edge* headToTop    = new Edge(graph->getHead(),  top->getHead(),    "epsilon");
    Edge* headToBottom = new Edge(graph->getHead(),  bottom->getHead(), "epsilon");
    Edge* topToTail    = new Edge(top->getTail(),    graph->getTail(),  "epsilon");
    Edge* bottomToTail = new Edge(bottom->getTail(), graph->getTail(),  "epsilon");

    graph->insertEdge(headToTop);
    graph->insertEdge(headToBottom);
    graph->insertEdge(topToTail);
    graph->insertEdge(bottomToTail);

    graph->getHead()->insertEdge(headToTop);
    graph->getHead()->insertEdge(headToBottom);

    top->getTail()->insertEdge(topToTail);
    bottom->getTail()->insertEdge(bottomToTail);

    top->getTail()->setFinal(false);
    bottom->getTail()->setFinal(false);
    graph->getTail()->setFinal(true);

    return graph;
}

Graph* getZeroOrMore(Graph* graph)
{
    Vertex* head = new Vertex(1);
    Vertex* tail = new Vertex(graph->size() + 2);

    Edge* headToGraph = new Edge(head,              graph->getHead(),   "epsilon");
    Edge* graphToTail = new Edge(graph->getTail(),  tail,               "epsilon");
    Edge* headToTail  = new Edge(head,              tail,               "epsilon");
    Edge* centerBack  = new Edge(graph->getTail(),  graph->getHead(),   "epsilon");

    graph->updateVertices(1);

    graph->insertVertex(head);
    graph->insertVertex(tail);
    graph->insertEdge(headToGraph);
    graph->insertEdge(graphToTail);
    graph->insertEdge(headToTail);
    graph->insertEdge(centerBack);

    head->insertEdge(headToGraph);
    head->insertEdge(headToTail);
    graph->getTail()->insertEdge(graphToTail);
    graph->getTail()->insertEdge(centerBack);

    tail->setFinal(true);
    graph->getTail()->setFinal(false);

    graph->setHead(head);
    graph->setTail(tail);

    return graph;
}

Graph* getOneOrMore(Graph* graph)
{
    Vertex* head = new Vertex(1);
    Vertex* tail = new Vertex(graph->size() + 2);

    Edge* headToGraph = new Edge(head,              graph->getHead(),   "epsilon");
    Edge* graphToTail = new Edge(graph->getTail(),  tail,               "epsilon");
    Edge* centerBack  = new Edge(graph->getTail(),  graph->getHead(),   "epsilon");

    graph->updateVertices(1);

    graph->insertVertex(head);
    graph->insertVertex(tail);
    graph->insertEdge(headToGraph);
    graph->insertEdge(graphToTail);
    graph->insertEdge(centerBack);

    head->insertEdge(headToGraph);
    graph->getTail()->insertEdge(graphToTail);
    graph->getTail()->insertEdge(centerBack);

    tail->setFinal(true);
    graph->getTail()->setFinal(false);

    graph->setHead(head);
    graph->setTail(tail);

    return graph;
}

Graph* getZeroOrOne(Graph* graph)
{
    Edge* newEdge = new Edge(graph->getHead(), graph->getTail(), "epsilon");
    graph->insertEdge(newEdge);
    graph->getHead()->insertEdge(newEdge);
    return graph;
}

Graph* getNFA(vector<Token*> tokens)
{
    Graph* nfa = new Graph();
    for (unsigned int i = 1; i < tokens.size(); i++) {
        Graph* graph;
        switch (tokens[i - 1]->getType()) {
            case 2:
            case 3:
            case 5:
            case 6:
                graph = getConcatenation(tokens[i - 1]->getText());
                switch (tokens[i]->getType()) {
                    case 10:
                        graph = getZeroOrMore(graph);
                        break;
                    case 11:
                        graph = getOneOrMore(graph);
                        break;
                    case 12:
                        graph = getZeroOrOne(graph);
                        break;
                    default:
                        nfa->append(graph);
                }
                break;

            case 8:
            {
                vector<Token*> subTokens;
                int count = 1;
                unsigned int index = i;
                for (unsigned int j = i; j < tokens.size(); j++) {
                    switch (tokens[j]->getType()) {
                        case 8:
                            count++;
                            break;
                        case 9:
                            count--;
                            break;
                    }
                    if (count == 0) {
                        index = j - 1;
                        break;
                    }
                    subTokens.push_back(tokens[j]);
                }
                subTokens.push_back(tokens[tokens.size() - 1]);
                graph = getNFA(subTokens);
                while (index < tokens.size() - 2 && (tokens[index + 2]->getType() == 10 || tokens[index + 2]->getType() == 11 || tokens[index + 2]->getType() == 12)) {
                    switch (tokens[index + 2]->getType()) {
                        case 10:
                            graph = getZeroOrMore(graph);
                            break;
                        case 11:
                            graph = getOneOrMore(graph);
                            break;
                        case 12:
                            graph = getZeroOrOne(graph);
                            break;
                    }
                    index++;
                }
                i = index + 2;
                nfa->append(graph);
                break;
            }

            case 10:
            case 11:
            case 12:
                switch (tokens[i]->getType()) {
                    case 10:
                        graph = getZeroOrMore(graph);
                        break;
                    case 11:
                        graph = getOneOrMore(graph);
                        break;
                    case 12:
                        graph = getZeroOrOne(graph);
                        break;
                    default:
                        nfa->append(graph);
                }
                break;

            case 13:
            {
                vector<Token*> subTokens(tokens.begin() + i, tokens.end());
                graph = getNFA(subTokens);
                nfa = getAlternation(nfa, graph);
                i = tokens.size() - 1;
                break;
            }

            case 14:
            case 15:
                break;
        }
    }
    return nfa;
}

Graph* getNFA(string regEx)
{
    Parser* parser = new Parser(new Scanner(regEx, "automaton.txt"));
    Graph* nfa = getNFA(parser->getTokens());
    delete parser;
    return nfa;
}
