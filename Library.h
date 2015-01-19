#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <set>

using namespace std;

bool        isOrdinary(char);
bool        isSpecial(char);
set<char>   getAlpha();
set<char>   getDigit();
set<char>   getAlnum();
set<char>   getUniverse();
set<char>   getComplement(set<char>);
set<char>   getCharSet(string);

#endif  // LIBRARY_H
