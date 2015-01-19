#ifndef MATCH_H
#define MATCH_H

#include "DFA.h"

#include <iostream>

struct Match {
    bool flag;
    int  begin;
    int  end;
    Match();
    string toString();
};

bool            matches(string, string);
Match           firstMatch(string, string);
string          replaceFirst(string, string, string);
string          replaceAll(string, string, string);
string          findFirst(string, string);
vector<string>  findAll(string, string);

#endif  // MATCH_H
