#ifndef NFA_H
#define NFA_H

#include "Parser.h"

Graph* getConcatenation(string);
Graph* getAlternation(Graph*, Graph*);
Graph* getZeroOrMore(Graph*);
Graph* getOneOrMore(Graph*);
Graph* getZeroOrOne(Graph*);
Graph* getNFA(string);
Graph* getNFA(vector<Token*>);

#endif	// NFA_H
