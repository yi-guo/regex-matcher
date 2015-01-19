#ifndef SCANNER_H
#define SCANNER_H

#include "Library.h"
#include "Graph.h"

#include <fstream>

class Token
{
public:
    Token();
    Token(string, int);
    ~Token();
    void    addChar(char);
    string  getText();
    void    setType(int);
    int     getType();

private:
    string  text;
    int     type;
};

class Scanner
{
public:
    Scanner();
    Scanner(string, Graph*);
    Scanner(string, string);
    ~Scanner();
    void         setInput(string);
    string       getInput();
    void         setAutomaton(Graph*);
    void         setAutomaton(string);
    Graph*       getAutomaton();
    Token*       readNext();
    Token*       peekNext();

private:
    unsigned int index;
    string       input;
    Graph*       automaton;

    Token*       next();
    Vertex*      getNextState(Vertex*, char);
    bool         match(string, char);
};


#endif  // SCANNER_H
