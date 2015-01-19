#ifndef DFA_H
#define DFA_H

#include "NFA.h"

bool    found(vector<int>, int);
bool    found(vector<string>, string);
bool    exsited(Graph*, Vertex*);
void    closure(Vertex*, Graph*);
Vertex* move(Vertex*, Graph*, string);
Graph*  getDFA(string);

#endif  // DFA_H
